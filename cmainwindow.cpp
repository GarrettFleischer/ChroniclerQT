#include "cmainwindow.h"

#include <QtWidgets>
#include <QStringListModel>
#include <QtGlobal>
#include <limits>
#include <QTabWidget>
#include <QFileInfo>
#include <QSettings>
#include <QMessageBox>

#include "Connections/arrow.h"
#include "cgraphicsscene.h"
#include "Bubbles/cstorybubble.h"
#include "cgraphicsview.h"
#include "chomepage.h"
#include "Properties/cdockmanager.h"
#include "csettingsview.h"


const int InsertTextButton = 10;


CMainWindow::CMainWindow(QSettings *settings)
    : m_ShiftHeld(false), m_settings(settings), m_settingsView(0)
{
    setWindowTitle(tr("Chronicler"));
    setUnifiedTitleAndToolBarOnMac(true);

    // Load the settings...
    m_settingsView = new CSettingsView(m_settings);
    connect(m_settingsView, SIGNAL(SettingsChanged()),
            this, SLOT(SettingsChanged()));
    SettingsChanged();

    CreateActions();
    CreateMenus();

    m_scene = new CGraphicsScene(m_editMenu, this);
    connect(m_scene, SIGNAL(itemInserted(CBubble*)),
            this, SLOT(ItemInserted(CBubble*)));
    connect(m_scene, SIGNAL(itemSelected(QGraphicsItem*)),
            this, SLOT(ItemSelected(QGraphicsItem*)));
    connect(m_scene, SIGNAL(leftReleased()),
            this, SLOT(SceneLeftReleased()));
    connect(m_scene, SIGNAL(leftPressed()),
            this, SLOT(SceneLeftPressed()));

    m_view = new CGraphicsView(m_scene);

    QStringList lst = QStringList() << "*set" << "*action" << "*create" << "*if" << "*elseif" << "${name}" << "${title}" << "${strength}";
    QStringListModel * lstModel = new QStringListModel(lst, this);

    m_dock = new QDockWidget("Project", this);
    connect(m_dock, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),
            this, SLOT(DockAreaChanged(Qt::DockWidgetArea)));

    m_dockManager = new CDockManager(lstModel, m_dock);
    m_dock->setWidget(m_dockManager);

    // load the last dock widget area from the settings
    Qt::DockWidgetArea area = static_cast<Qt::DockWidgetArea>(m_settings->value("MainWindow/DockArea", static_cast<int>(Qt::LeftDockWidgetArea)).toInt());
    addDockWidget(area, m_dock);

    m_dock->setVisible(false);
    m_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    m_homepage = new CHomepage(this, m_settingsView);

    m_tabView = new QTabWidget(this);
    m_tabView->setMovable(true);
    m_tabView->setTabsClosable(true);
    connect(m_tabView, SIGNAL(tabCloseRequested(int)),
            this, SLOT(TabClosed(int)));

    m_tabView->addTab(m_homepage,"Homepage");

    setCentralWidget(m_tabView);

    CreateToolbars();


}

void CMainWindow::LoadProject(const QString &filepath)
{
    setWindowTitle("Chronicler - " + QFileInfo(filepath).baseName());

    m_dock->setVisible(true);
    m_dock->setWindowTitle(QFileInfo(filepath).fileName());
    m_tabView->addTab(m_view, "startup.scn");
    m_tabView->setCurrentWidget(m_view);

    m_tabView->removeTab(m_tabView->indexOf(m_homepage));
}


void CMainWindow::keyPressEvent(QKeyEvent *evt)
{
    QMainWindow::keyPressEvent(evt);

    if(evt->key() == Qt::Key_Shift)
        m_ShiftHeld = true;
    else if(evt->key() == Qt::Key_Delete)
        DeleteItem();
}

void CMainWindow::keyReleaseEvent(QKeyEvent *evt)
{
    QMainWindow::keyReleaseEvent(evt);

    if(evt->key() == Qt::Key_Shift)
    {
        m_ShiftHeld = false;
        m_pointerTypeGroup->button(int(CGraphicsScene::Cursor))->setChecked(true);
        m_scene->setMode(CGraphicsScene::Cursor);
        m_view->setDragMode(QGraphicsView::ScrollHandDrag);
    }
}


void CMainWindow::DeleteItem()
{
    m_dockManager->setBubble(0);

    foreach (QGraphicsItem *item, m_scene->selectedItems())
        delete item;
}


void CMainWindow::PointerGroupClicked(int id)
{
    m_scene->setMode(CGraphicsScene::Mode(id));

    if(id == int(CGraphicsScene::InsertLine))
        m_view->setDragMode(QGraphicsView::NoDrag);
    else
        m_view->setDragMode(QGraphicsView::ScrollHandDrag);
}


void CMainWindow::ItemInserted(CBubble *)
{
    if(!m_ShiftHeld)
    {
        m_pointerTypeGroup->button(int(CGraphicsScene::Cursor))->setChecked(true);
        m_scene->setMode(CGraphicsScene::Cursor);
        m_view->setDragMode(QGraphicsView::ScrollHandDrag);
    }
}


void CMainWindow::ItemSelected(QGraphicsItem *selectedItem)
{
    if(!m_scene->isRubberBandSelecting())
    {
        foreach (QGraphicsItem *item, m_scene->items())
            item->setZValue(item->zValue() - qPow(1, -10));

        selectedItem->setZValue(1);
    }
}

void CMainWindow::ShowSettings()
{
    if(m_tabView->indexOf(m_settingsView) == -1)
        m_tabView->insertTab(0, m_settingsView, "Settings");

    m_tabView->setCurrentWidget(m_settingsView);
}

void CMainWindow::ShowHomepage()
{
    if(m_tabView->indexOf(m_homepage) == -1)
        m_tabView->insertTab(0, m_homepage, "Homepage");

    m_tabView->setCurrentWidget(m_homepage);
}


void CMainWindow::SceneLeftPressed()
{
    //m_dockManager->setBubble(0, true);
}

void CMainWindow::SceneLeftReleased()
{
    if(!m_dock->isHidden())
        m_dock->activateWindow();
    QList<QGraphicsItem *> selected = m_scene->selectedItems();
    if(selected.size() == 1)
    {
        CBubble *bbl = dynamic_cast<CBubble *>(selected.first());
        m_dockManager->setBubble(bbl);
    }
    else
        m_dockManager->setBubble(0);
}

void CMainWindow::TabClosed(int index)
{
    if(m_tabView->widget(index) == m_settingsView)
    {
        if(m_settingsView->pendingChanges())
        {
            QCheckBox dontShow("Remember my choice and don't show again.");
            dontShow.blockSignals(true);
            QMessageBox msgBox;
            msgBox.setText("Settings have been modified.");
            msgBox.setInformativeText("Do you wish to apply these changes?");
            msgBox.setStandardButtons(QMessageBox::Apply | QMessageBox::Discard | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Apply);
            msgBox.setCheckBox(&dontShow);
            int ret = msgBox.exec();

            if(ret == QMessageBox::Apply)
                m_settingsView->ApplyPendingChanges();
            else if(ret == QMessageBox::Cancel)
                return;

            // TODO
            // save dontShow value...
        }
    }

    m_tabView->removeTab(index);
}

void CMainWindow::DockAreaChanged(Qt::DockWidgetArea area)
{
    // reflect this change in the settings.
    m_settings->setValue("MainWindow/DockArea", static_cast<int>(area));
}

void CMainWindow::ToolBarAreaChanged(bool)
{
    // reflect this change in the settings.
    m_settings->setValue("MainWindow/ToolBarArea", static_cast<int>(toolBarArea(m_pointerToolBar)));
}

void CMainWindow::SettingsChanged()
{
    // Update font and font color.
    setFont(m_settingsView->font());

    QPalette pal = palette();
    pal.setColor(QPalette::WindowText, m_settingsView->fontColor());
    pal.setColor(QPalette::Text, m_settingsView->fontColor());
    setPalette(pal);
}

void CMainWindow::ShowAbout()
{
    QMessageBox::about(this, tr("About Chronicler-Next"), tr("<b>Insert legal stuff here...</b>"));
}


void CMainWindow::CreateActions()
{
    m_deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this);
    m_deleteAction->setShortcut(tr("Delete"));
    m_deleteAction->setStatusTip(tr("Delete selected bubble(S)"));
    connect(m_deleteAction, SIGNAL(triggered()), this, SLOT(DeleteItem()));

    m_exitAction = new QAction(tr("E&xit"), this);
    m_exitAction->setShortcuts(QKeySequence::Quit);
    m_exitAction->setStatusTip(tr("Quit program"));
    connect(m_exitAction, SIGNAL(triggered()), this, SLOT(close()));

    m_settingsAction = new QAction(tr("&Settings"), this);
    m_settingsAction->setShortcut(tr("Ctrl+P"));
    connect(m_settingsAction, SIGNAL(triggered(bool)), this, SLOT(ShowSettings()));

    m_aboutAction = new QAction(tr("A&bout"), this);
    connect(m_aboutAction, SIGNAL(triggered()), this, SLOT(ShowAbout()));

    m_showHomepageAction = new QAction(tr("Show &homepage"), this);
    connect(m_showHomepageAction, SIGNAL(triggered(bool)), this, SLOT(ShowHomepage()));
}


void CMainWindow::CreateMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_exitAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_showHomepageAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_settingsAction);

    m_editMenu = menuBar()->addMenu(tr("&Edit"));
    m_editMenu->addAction(m_deleteAction);

    m_aboutMenu = menuBar()->addMenu(tr("&Help"));
    m_aboutMenu->addAction(m_aboutAction);
}


void CMainWindow::CreateToolbars()
{
    QToolButton *pointerButton = new QToolButton();
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));
    QToolButton *connectionPointerButton = new QToolButton();
    connectionPointerButton->setCheckable(true);
    connectionPointerButton->setIcon(QIcon(":/images/linepointer.png"));


    QToolButton *storyBubbleToolButton = new QToolButton();
    storyBubbleToolButton->setCheckable(true);
    storyBubbleToolButton->setIcon(QIcon(":/images/icn_story.png"));
    QToolButton *conditionBubbleToolButton = new QToolButton();
    conditionBubbleToolButton->setCheckable(true);
    conditionBubbleToolButton->setIcon(QIcon(":/images/icn_condition.png"));
    QToolButton *actionBubbleToolButton = new QToolButton();
    actionBubbleToolButton->setCheckable(true);
    actionBubbleToolButton->setIcon(QIcon(":/images/icn_choice.png"));


    m_pointerTypeGroup = new QButtonGroup(this);
    m_pointerTypeGroup->addButton(pointerButton, int(CGraphicsScene::Cursor));
    m_pointerTypeGroup->addButton(connectionPointerButton, int(CGraphicsScene::InsertLine));
    m_pointerTypeGroup->addButton(storyBubbleToolButton, int(CGraphicsScene::InsertStory));
    m_pointerTypeGroup->addButton(conditionBubbleToolButton, int(CGraphicsScene::InsertCondition));
    m_pointerTypeGroup->addButton(actionBubbleToolButton, int(CGraphicsScene::InsertChoice));
    connect(m_pointerTypeGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(PointerGroupClicked(int)));


    Qt::ToolBarArea area = static_cast<Qt::ToolBarArea>(m_settings->value("MainWindow/ToolBarArea", static_cast<int>(Qt::RightToolBarArea)).toInt());
    m_pointerToolBar = new QToolBar("Pointer type");
    m_pointerToolBar->addWidget(pointerButton);
    m_pointerToolBar->addWidget(connectionPointerButton);
    m_pointerToolBar->addWidget(storyBubbleToolButton);
    m_pointerToolBar->addWidget(conditionBubbleToolButton);
    m_pointerToolBar->addWidget(actionBubbleToolButton);
    addToolBar(area, m_pointerToolBar);
    connect(m_pointerToolBar, SIGNAL(topLevelChanged(bool)),
            this, SLOT(ToolBarAreaChanged(bool)));
}
