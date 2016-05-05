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
    : m_ShiftHeld(false)
{
    setWindowTitle(tr("Chronicler"));
    setUnifiedTitleAndToolBarOnMac(true);

    // Load the settings...
    m_settingsView = new CSettingsView(settings);
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
    m_dock->setVisible(false);
    m_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    // load the last dock widget area from the settings
    Qt::DockWidgetArea area = static_cast<Qt::DockWidgetArea>(m_settingsView->settings()->value("MainWindow/DockArea",
                                                                                                static_cast<int>(Qt::LeftDockWidgetArea)).toInt());
    addDockWidget(area, m_dock);

    m_homepage = new CHomepage(this, m_settingsView, m_newProjectAction, m_openProjectAction, m_importProjectAction);

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

void CMainWindow::NewProject()
{
    // TODO popup new project dialog
}

void CMainWindow::OpenProject()
{
    // TODO popup ".chron" file picker
}

void CMainWindow::ImportProject()
{
    // TODO popup "startup.txt" file picker
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
    m_settingsView->settings()->setValue("MainWindow/DockArea", static_cast<int>(area));
}

void CMainWindow::PointerToolBarAreaChanged(bool)
{
    // reflect this change in the settings.
    m_settingsView->settings()->setValue("MainWindow/ToolBarArea", static_cast<int>(toolBarArea(m_pointerToolBar)));
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
    m_deleteAction = new QAction(QIcon(":/images/icn_trash.png"), tr("&Delete"), this);
    m_deleteAction->setShortcut(tr("Delete"));
    m_deleteAction->setToolTip(tr("Delete selected bubble(S)"));
    connect(m_deleteAction, SIGNAL(triggered()), this, SLOT(DeleteItem()));

    m_exitAction = new QAction(QIcon(":/images/icn_exit.png"), tr("E&xit"), this);
    m_exitAction->setShortcuts(QKeySequence::Quit);
    m_exitAction->setToolTip(tr("Quit program"));
    connect(m_exitAction, SIGNAL(triggered()), this, SLOT(close()));

    m_settingsAction = new QAction(QIcon(":/images/icn_settings"), tr("&Settings"), this);
    m_settingsAction->setShortcut(tr("Ctrl+P"));
    connect(m_settingsAction, SIGNAL(triggered(bool)), this, SLOT(ShowSettings()));

    m_aboutAction = new QAction(QIcon(":/images/icn_info"), tr("A&bout"), this);
    connect(m_aboutAction, SIGNAL(triggered()), this, SLOT(ShowAbout()));

    m_newProjectAction = new QAction(QIcon(":/images/icn_new"), tr("New Project"), this);
    m_newProjectAction->setShortcut(tr("Ctrl+N"));
    m_newProjectAction->setToolTip("Create New Project");
    connect(m_newProjectAction, SIGNAL(triggered(bool)), this, SLOT(NewProject()));

    m_openProjectAction = new QAction(QIcon(":/images/icn_load"), tr("Open Project"), this);
    m_openProjectAction->setShortcut(tr("Ctrl+O"));
    m_openProjectAction->setToolTip("Open Existing Project");
    connect(m_openProjectAction, SIGNAL(triggered(bool)), this, SLOT(OpenProject()));

    m_importProjectAction = new QAction(QIcon(":/images/icn_loadcs"), tr("Import Project"), this);
    m_importProjectAction->setToolTip("Import ChoiceScript Project");
    connect(m_importProjectAction, SIGNAL(triggered(bool)), this, SLOT(ImportProject()));

    m_showHomepageAction = new QAction(QIcon(":/images/icn_home"), tr("Show &homepage"), this);
    connect(m_showHomepageAction, SIGNAL(triggered(bool)), this, SLOT(ShowHomepage()));
}


void CMainWindow::CreateMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_newProjectAction);
    m_fileMenu->addAction(m_openProjectAction);
    m_fileMenu->addAction(m_importProjectAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_settingsAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);

    m_editMenu = menuBar()->addMenu(tr("&Edit"));
    m_editMenu->addAction(m_deleteAction);

    m_viewMenu = menuBar()->addMenu(tr("&View"));
    m_viewMenu->addAction(m_showHomepageAction);

    m_helpMenu = menuBar()->addMenu(tr("&Help"));
    m_helpMenu->addAction(m_aboutAction);
}


void CMainWindow::CreateToolbars()
{
    QToolButton *tb_pointer = new QToolButton();
    tb_pointer->setCheckable(true);
    tb_pointer->setChecked(true);
    tb_pointer->setIcon(QIcon(":/images/icn_pointer.png"));
    tb_pointer->setToolTip("Selection tool");
    QToolButton *tb_link = new QToolButton();
    tb_link->setCheckable(true);
    tb_link->setIcon(QIcon(":/images/icn_link.png"));
    tb_link->setToolTip("Link tool");


    QToolButton *tb_story = new QToolButton();
    tb_story->setCheckable(true);
    tb_story->setIcon(QIcon(":/images/icn_story.png"));
    tb_story->setToolTip("Story bubble");
    QToolButton *tb_condition = new QToolButton();
    tb_condition->setCheckable(true);
    tb_condition->setIcon(QIcon(":/images/icn_condition.png"));
    tb_condition->setToolTip("Condition bubble");
    QToolButton *tb_action = new QToolButton();
    tb_action->setCheckable(true);
    tb_action->setIcon(QIcon(":/images/icn_action.png"));
    tb_action->setToolTip("Action bubble");
    QToolButton *tb_choice = new QToolButton();
    tb_choice->setCheckable(true);
    tb_choice->setIcon(QIcon(":/images/icn_choice2.png"));
    tb_choice->setToolTip("Choice bubble");

    m_pointerTypeGroup = new QButtonGroup(this);
    m_pointerTypeGroup->addButton(tb_pointer, int(CGraphicsScene::Cursor));
    m_pointerTypeGroup->addButton(tb_link, int(CGraphicsScene::InsertLine));
    m_pointerTypeGroup->addButton(tb_story, int(CGraphicsScene::InsertStory));
    m_pointerTypeGroup->addButton(tb_condition, int(CGraphicsScene::InsertCondition));
    m_pointerTypeGroup->addButton(tb_choice, int(CGraphicsScene::InsertChoice));
    m_pointerTypeGroup->addButton(tb_action, int(CGraphicsScene::InsertAction));
    connect(m_pointerTypeGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(PointerGroupClicked(int)));


    Qt::ToolBarArea area = static_cast<Qt::ToolBarArea>(m_settingsView->settings()->value("MainWindow/ToolBarArea", static_cast<int>(Qt::RightToolBarArea)).toInt());
    m_pointerToolBar = new QToolBar("Pointer type");
    m_pointerToolBar->addWidget(tb_pointer);
    m_pointerToolBar->addWidget(tb_link);
    m_pointerToolBar->addWidget(tb_story);
    m_pointerToolBar->addWidget(tb_choice);
    m_pointerToolBar->addWidget(tb_action);
    m_pointerToolBar->addWidget(tb_condition);
    m_pointerToolBar->setIconSize(QSize(32,32));
    addToolBar(area, m_pointerToolBar);
    connect(m_pointerToolBar, SIGNAL(topLevelChanged(bool)),
            this, SLOT(PointerToolBarAreaChanged(bool)));
}
