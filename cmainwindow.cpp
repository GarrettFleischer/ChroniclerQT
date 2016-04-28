#include "cmainwindow.h"

#include <QtWidgets>
#include <QStringListModel>
#include <QtGlobal>
#include <limits>
#include <QTabWidget>
#include <QFileInfo>
#include <QSettings>

#include "Connections/arrow.h"
#include "cgraphicsscene.h"
#include "Bubbles/cstorybubble.h"
#include "cgraphicsview.h"
#include "chomepage.h"
#include "Properties/cdockmanager.h"
#include "csettingsview.h"


const int InsertTextButton = 10;


CMainWindow::CMainWindow(QSettings *settings)
    : m_ShiftHeld(false), m_settings(settings)
{
    setWindowTitle(tr("Chronicler-Next"));
    setUnifiedTitleAndToolBarOnMac(true);

    CreateActions();
    CreateMenus();


    m_scene = new CGraphicsScene(m_itemMenu, this);
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

    //m_dock->setVisible(false);
    m_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    CHomepage *home = new CHomepage(this, m_settings);


    m_tabView = new QTabWidget(this);
    m_tabView->setMovable(true);
    m_tabView->setTabsClosable(true);
    connect(m_tabView, SIGNAL(tabCloseRequested(int)),
            this, SLOT(TabClosed(int)));

    m_tabView->addTab(new CSettingsView(m_settings), "Settings");
    m_tabView->addTab(home,"Homepage");

    m_tabView->addTab(m_view, "startup.scn");


    setCentralWidget(m_tabView);

    CreateToolbars();
}

void CMainWindow::LoadProject(const QString &filepath)
{
    m_dock->setVisible(true);
    m_dock->setWindowTitle(QFileInfo(filepath).fileName());
    m_tabView->addTab(m_view, "startup.scn");
    m_tabView->setCurrentWidget(m_view);
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


void CMainWindow::FontChanged(const QFont &font)
{
        m_scene->setFont(font);
        m_dockManager->setFont(font);
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
    m_tabView->removeTab(index);
}

void CMainWindow::DockAreaChanged(Qt::DockWidgetArea area)
{
    // reflect this change in the settings.
    m_settings->setValue("MainWindow/DockArea", static_cast<int>(area));
}

void CMainWindow::ToolBarAreaChanged(bool)
{
    m_settings->setValue("MainWindow/ToolBarArea", static_cast<int>(toolBarArea(m_pointerToolBar)));
}


void CMainWindow::About()
{
    QMessageBox::about(this, tr("About Chronicler-Next"), tr("<b>Insert legal stuff here...</b>"));
}


void CMainWindow::CreateActions()
{
    m_deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this);
    m_deleteAction->setShortcut(tr("Delete"));
    m_deleteAction->setStatusTip(tr("Delete item from diagram"));
    connect(m_deleteAction, SIGNAL(triggered()), this, SLOT(DeleteItem()));

    m_exitAction = new QAction(tr("E&xit"), this);
    m_exitAction->setShortcuts(QKeySequence::Quit);
    m_exitAction->setStatusTip(tr("Quit Scenediagram example"));
    connect(m_exitAction, SIGNAL(triggered()), this, SLOT(close()));

    //    m_boldAction = new QAction(tr("Bold"), this);
    //    m_boldAction->setCheckable(true);
    //    QPixmap pixmap(":/images/bold.png");
    //    m_boldAction->setIcon(QIcon(pixmap));
    //    m_boldAction->setShortcut(tr("Ctrl+B"));
    //    connect(m_boldAction, SIGNAL(triggered()), this, SLOT(HandleFontChange()));

    //    m_italicAction = new QAction(QIcon(":/images/italic.png"), tr("Italic"), this);
    //    m_italicAction->setCheckable(true);
    //    m_italicAction->setShortcut(tr("Ctrl+I"));
    //    connect(m_italicAction, SIGNAL(triggered()), this, SLOT(HandleFontChange()));

    //    m_underlineAction = new QAction(QIcon(":/images/underline.png"), tr("Underline"), this);
    //    m_underlineAction->setCheckable(true);
    //    m_underlineAction->setShortcut(tr("Ctrl+U"));
    //    connect(m_underlineAction, SIGNAL(triggered()), this, SLOT(HandleFontChange()));

    m_aboutAction = new QAction(tr("A&bout"), this);
    connect(m_aboutAction, SIGNAL(triggered()), this, SLOT(About()));
}


void CMainWindow::CreateMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_exitAction);

    m_itemMenu = menuBar()->addMenu(tr("&Item"));
    m_itemMenu->addAction(m_deleteAction);
    m_itemMenu->addSeparator();

    m_aboutMenu = menuBar()->addMenu(tr("&Help"));
    m_aboutMenu->addAction(m_aboutAction);
}


void CMainWindow::CreateToolbars()
{
    //    m_fontCombo = new QFontComboBox();
    //    connect(m_fontCombo, SIGNAL(currentFontChanged(QFont)),
    //            this, SLOT(CurrentFontChanged(QFont)));
    //    m_fontCombo->setCurrentText("Times New Roman");


    //    m_fontSizeCombo = new QSpinBox;
    //    m_fontSizeCombo->setRange(8, 42);
    //    m_fontSizeCombo->setValue(11);
    //    connect(m_fontSizeCombo, SIGNAL(valueChanged(QString)),
    //            this, SLOT(FontSizeChanged(QString)));

    //    m_fontColorToolButton = new QToolButton;
    //    m_fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    //    m_fontColorToolButton->setMenu(CreateColorMenu(SLOT(TextColorChanged()), Qt::black));
    //    m_textAction = m_fontColorToolButton->menu()->defaultAction();
    //    m_fontColorToolButton->setIcon(CreateColorToolButtonIcon(":/images/textpointer.png", Qt::black));
    //    m_fontColorToolButton->setAutoFillBackground(true);
    //    connect(m_fontColorToolButton, SIGNAL(clicked()),
    //            this, SLOT(TextButtonTriggered()));

    //    m_fillColorToolButton = new QToolButton;
    //    m_fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    //    m_fillColorToolButton->setMenu(CreateColorMenu(SLOT(ItemColorChanged()), Qt::white));
    //    m_fillAction = m_fillColorToolButton->menu()->defaultAction();
    //    m_fillColorToolButton->setIcon(CreateColorToolButtonIcon(
    //                                       ":/images/floodfill.png", Qt::white));
    //    connect(m_fillColorToolButton, SIGNAL(clicked()),
    //            this, SLOT(FillButtonTriggered()));

    //    m_lineColorToolButton = new QToolButton;
    //    m_lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    //    m_lineColorToolButton->setMenu(CreateColorMenu(SLOT(LineColorChanged()), Qt::black));
    //    m_lineAction = m_lineColorToolButton->menu()->defaultAction();
    //    m_lineColorToolButton->setIcon(CreateColorToolButtonIcon(
    //                                       ":/images/linecolor.png", Qt::black));
    //    connect(m_lineColorToolButton, SIGNAL(clicked()),
    //            this, SLOT(LineButtonTriggered()));

    //    m_textToolBar = addToolBar(tr("Font"));
    //    m_textToolBar->addWidget(m_fontCombo);
    //    m_textToolBar->addWidget(m_fontSizeCombo);
    //    m_textToolBar->addAction(m_boldAction);
    //    m_textToolBar->addAction(m_italicAction);
    //    m_textToolBar->addAction(m_underlineAction);

    //    m_colorToolBar = addToolBar(tr("Color"));
    //    m_colorToolBar->addWidget(m_fontColorToolButton);
    //    m_colorToolBar->addWidget(m_fillColorToolButton);
    //    m_colorToolBar->addWidget(m_lineColorToolButton);

    QToolButton *pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));
    QToolButton *linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/images/linepointer.png"));


    QToolButton *storyBubbleToolButton = new QToolButton;
    storyBubbleToolButton->setCheckable(true);
    storyBubbleToolButton->setIcon(QIcon(":/images/icn_story.png"));
    QToolButton *conditionBubbleToolButton = new QToolButton;
    conditionBubbleToolButton->setCheckable(true);
    conditionBubbleToolButton->setIcon(QIcon(":/images/icn_condition.png"));
    QToolButton *actionBubbleToolButton = new QToolButton;
    actionBubbleToolButton->setCheckable(true);
    actionBubbleToolButton->setIcon(QIcon(":/images/icn_choice.png"));


    m_pointerTypeGroup = new QButtonGroup(this);
    m_pointerTypeGroup->addButton(pointerButton, int(CGraphicsScene::Cursor));
    m_pointerTypeGroup->addButton(linePointerButton, int(CGraphicsScene::InsertLine));
    m_pointerTypeGroup->addButton(storyBubbleToolButton, int(CGraphicsScene::InsertStory));
    m_pointerTypeGroup->addButton(conditionBubbleToolButton, int(CGraphicsScene::InsertCondition));
    m_pointerTypeGroup->addButton(actionBubbleToolButton, int(CGraphicsScene::InsertChoice));
    connect(m_pointerTypeGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(PointerGroupClicked(int)));

    Qt::ToolBarArea area = static_cast<Qt::ToolBarArea>(m_settings->value("MainWindow/ToolBarArea", static_cast<int>(Qt::TopToolBarArea)).toInt());
    m_pointerToolBar = new QToolBar("Pointer type");
    m_pointerToolBar->addWidget(pointerButton);
    m_pointerToolBar->addWidget(linePointerButton);
    m_pointerToolBar->addWidget(storyBubbleToolButton);
    m_pointerToolBar->addWidget(conditionBubbleToolButton);
    m_pointerToolBar->addWidget(actionBubbleToolButton);
    addToolBar(area, m_pointerToolBar);
    connect(m_pointerToolBar, SIGNAL(topLevelChanged(bool)),
            this, SLOT(ToolBarAreaChanged(bool)));
}


QIcon CMainWindow::CreateColorToolButtonIcon(const QString &imageFile, QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    // Draw icon centred horizontally on button.
    QRect target(4, 0, 42, 43);
    QRect source(0, 0, 42, 43);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);

    return QIcon(pixmap);
}


QIcon CMainWindow::CreateColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), color);

    return QIcon(pixmap);
}
