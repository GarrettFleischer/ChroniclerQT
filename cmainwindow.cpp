#include "cmainwindow.h"

#include "Connections/arrow.h"
#include "cgraphicsscene.h"
#include "Bubbles/cstorybubble.h"
#include "cgraphicsview.h"
#include "chomepage.h"
#include "Properties/cdockmanager.h"
#include "Properties/cpropertiesview.h"

#include <QtWidgets>
#include <QStringListModel>
#include <QtGlobal>
#include <limits>
#include <QTabWidget>



const int InsertTextButton = 10;


CMainWindow::CMainWindow()
    : m_ShiftHeld(false), m_scale(1)
{
    setWindowTitle(tr("Chronicler-Next"));
    setUnifiedTitleAndToolBarOnMac(true);

    CreateActions();
    CreateMenus();

    float maxsize = std::numeric_limits<float>::max();
    float minsize = -std::numeric_limits<float>::max()/2;
    m_scene = new CGraphicsScene(m_itemMenu, this);
    m_scene->setSceneRect(QRectF(minsize, minsize, maxsize, maxsize));
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
    m_dockManager = new CDockManager(lstModel, m_dock);
    m_dock->setWidget(m_dockManager);
    addDockWidget(Qt::LeftDockWidgetArea, m_dock);
    m_dock->setVisible(false);
    //m_properties = new CPropertiesView(lstModel, m_dock);
    m_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    CHomepage *home = new CHomepage(this);

    m_tabView = new QTabWidget(this);
    m_tabView->setTabsClosable(true);
    connect(m_tabView, SIGNAL(tabCloseRequested(int)),
            this, SLOT(TabClosed(int)));

    m_tabView->addTab(home,"Homepage");

    setCentralWidget(m_tabView);

    CreateToolbars();
    HandleFontChange();
}

void CMainWindow::LoadProject(const QString &filename)
{
    m_dock->setVisible(true);
    m_tabView->addTab(m_view, "startup.scn");
    m_tabView->setCurrentIndex(1);
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


void CMainWindow::CurrentFontChanged(const QFont &)
{
    HandleFontChange();
}


void CMainWindow::FontSizeChanged(const QString &)
{
    HandleFontChange();
}


void CMainWindow::SceneScaleChanged(const QString &scale)
{
    m_scale = scale.left(scale.indexOf(tr("%"))).toFloat() / 100.0;
    UpdateSceneScale();
}

void CMainWindow::UpdateSceneScale()
{
    //    QMatrix oldMatrix = view->matrix();
    //    QMatrix newMatrix;
    //    //view->resetMatrix();
    //   newMatrix.translate(oldMatrix.dx(), oldMatrix.dy());
    //   view->setMatrix(newMatrix);
    //view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    m_view->scale(m_scale, m_scale);
}


void CMainWindow::TextColorChanged()
{
    m_textAction = qobject_cast<QAction *>(sender());
    m_fontColorToolButton->setIcon(CreateColorToolButtonIcon(
                                       ":/images/textpointer.png",
                                       qvariant_cast<QColor>(m_textAction->data())));
    TextButtonTriggered();
}


void CMainWindow::ItemColorChanged()
{
    m_fillAction = qobject_cast<QAction *>(sender());
    m_fillColorToolButton->setIcon(CreateColorToolButtonIcon(
                                       ":/images/floodfill.png",
                                       qvariant_cast<QColor>(m_fillAction->data())));
    FillButtonTriggered();
}


void CMainWindow::LineColorChanged()
{
    m_lineAction = qobject_cast<QAction *>(sender());
    m_lineColorToolButton->setIcon(CreateColorToolButtonIcon(
                                       ":/images/linecolor.png",
                                       qvariant_cast<QColor>(m_lineAction->data())));
    LineButtonTriggered();
}


void CMainWindow::TextButtonTriggered()
{
    m_scene->setTextColor(qvariant_cast<QColor>(m_textAction->data()));
}


void CMainWindow::FillButtonTriggered()
{
    m_scene->setItemColor(qvariant_cast<QColor>(m_fillAction->data()));
}


void CMainWindow::LineButtonTriggered()
{
    m_scene->setLineColor(qvariant_cast<QColor>(m_lineAction->data()));
}


void CMainWindow::HandleFontChange()
{
    QFont font = m_fontCombo->currentFont();
    font.setPointSize(m_fontSizeCombo->value());
    font.setWeight(m_boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(m_italicAction->isChecked());
    font.setUnderline(m_underlineAction->isChecked());

    m_scene->setFont(font);
    //m_properties->setFont(font);
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
    m_dockManager->properties().setBubble(0);
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


void CMainWindow::About()
{
    QMessageBox::about(this, tr("About Chronicler-Next"), tr("<b>Insert legal stuff here...</b>"));
}


void CMainWindow::CreateActions()
{
    //    toFrontAction = new QAction(QIcon(":/images/bringtofront.png"),
    //                                tr("Bring to &Front"), this);
    //    toFrontAction->setShortcut(tr("Ctrl+F"));
    //    toFrontAction->setStatusTip(tr("Bring item to front"));
    //    connect(toFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront()));

    //    sendBackAction = new QAction(QIcon(":/images/sendtoback.png"), tr("Send to &Back"), this);
    //    sendBackAction->setShortcut(tr("Ctrl+B"));
    //    sendBackAction->setStatusTip(tr("Send item to back"));
    //    connect(sendBackAction, SIGNAL(triggered()), this, SLOT(sendToBack()));

    m_deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this);
    m_deleteAction->setShortcut(tr("Delete"));
    m_deleteAction->setStatusTip(tr("Delete item from diagram"));
    connect(m_deleteAction, SIGNAL(triggered()), this, SLOT(DeleteItem()));

    m_exitAction = new QAction(tr("E&xit"), this);
    m_exitAction->setShortcuts(QKeySequence::Quit);
    m_exitAction->setStatusTip(tr("Quit Scenediagram example"));
    connect(m_exitAction, SIGNAL(triggered()), this, SLOT(close()));

    m_boldAction = new QAction(tr("Bold"), this);
    m_boldAction->setCheckable(true);
    QPixmap pixmap(":/images/bold.png");
    m_boldAction->setIcon(QIcon(pixmap));
    m_boldAction->setShortcut(tr("Ctrl+B"));
    connect(m_boldAction, SIGNAL(triggered()), this, SLOT(HandleFontChange()));

    m_italicAction = new QAction(QIcon(":/images/italic.png"), tr("Italic"), this);
    m_italicAction->setCheckable(true);
    m_italicAction->setShortcut(tr("Ctrl+I"));
    connect(m_italicAction, SIGNAL(triggered()), this, SLOT(HandleFontChange()));

    m_underlineAction = new QAction(QIcon(":/images/underline.png"), tr("Underline"), this);
    m_underlineAction->setCheckable(true);
    m_underlineAction->setShortcut(tr("Ctrl+U"));
    connect(m_underlineAction, SIGNAL(triggered()), this, SLOT(HandleFontChange()));

    m_aboutAction = new QAction(tr("A&bout"), this);
    m_aboutAction->setShortcut(tr("Ctrl+B"));
    connect(m_aboutAction, SIGNAL(triggered()), this, SLOT(About()));
}


void CMainWindow::CreateMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_exitAction);

    m_itemMenu = menuBar()->addMenu(tr("&Item"));
    m_itemMenu->addAction(m_deleteAction);
    m_itemMenu->addSeparator();
    //    itemMenu->addAction(toFrontAction);
    //    itemMenu->addAction(sendBackAction);

    m_aboutMenu = menuBar()->addMenu(tr("&Help"));
    m_aboutMenu->addAction(m_aboutAction);
}


void CMainWindow::CreateToolbars()
{
    m_fontCombo = new QFontComboBox();
    connect(m_fontCombo, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(CurrentFontChanged(QFont)));
    m_fontCombo->setCurrentText("Times New Roman");


    m_fontSizeCombo = new QSpinBox;
    m_fontSizeCombo->setRange(8, 42);
    m_fontSizeCombo->setValue(11);
    connect(m_fontSizeCombo, SIGNAL(valueChanged(QString)),
            this, SLOT(FontSizeChanged(QString)));

    m_fontColorToolButton = new QToolButton;
    m_fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    m_fontColorToolButton->setMenu(CreateColorMenu(SLOT(TextColorChanged()), Qt::black));
    m_textAction = m_fontColorToolButton->menu()->defaultAction();
    m_fontColorToolButton->setIcon(CreateColorToolButtonIcon(":/images/textpointer.png", Qt::black));
    m_fontColorToolButton->setAutoFillBackground(true);
    connect(m_fontColorToolButton, SIGNAL(clicked()),
            this, SLOT(TextButtonTriggered()));

    m_fillColorToolButton = new QToolButton;
    m_fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    m_fillColorToolButton->setMenu(CreateColorMenu(SLOT(ItemColorChanged()), Qt::white));
    m_fillAction = m_fillColorToolButton->menu()->defaultAction();
    m_fillColorToolButton->setIcon(CreateColorToolButtonIcon(
                                       ":/images/floodfill.png", Qt::white));
    connect(m_fillColorToolButton, SIGNAL(clicked()),
            this, SLOT(FillButtonTriggered()));

    m_lineColorToolButton = new QToolButton;
    m_lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    m_lineColorToolButton->setMenu(CreateColorMenu(SLOT(LineColorChanged()), Qt::black));
    m_lineAction = m_lineColorToolButton->menu()->defaultAction();
    m_lineColorToolButton->setIcon(CreateColorToolButtonIcon(
                                       ":/images/linecolor.png", Qt::black));
    connect(m_lineColorToolButton, SIGNAL(clicked()),
            this, SLOT(LineButtonTriggered()));

    m_textToolBar = addToolBar(tr("Font"));
    m_textToolBar->addWidget(m_fontCombo);
    m_textToolBar->addWidget(m_fontSizeCombo);
    m_textToolBar->addAction(m_boldAction);
    m_textToolBar->addAction(m_italicAction);
    m_textToolBar->addAction(m_underlineAction);

    m_colorToolBar = addToolBar(tr("Color"));
    m_colorToolBar->addWidget(m_fontColorToolButton);
    m_colorToolBar->addWidget(m_fillColorToolButton);
    m_colorToolBar->addWidget(m_lineColorToolButton);

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

    m_sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    m_sceneScaleCombo->addItems(scales);
    m_sceneScaleCombo->setCurrentIndex(2);
    connect(m_sceneScaleCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(SceneScaleChanged(QString)));

    m_pointerToolBar = addToolBar(tr("Pointer type"));
    m_pointerToolBar->addWidget(pointerButton);
    m_pointerToolBar->addWidget(linePointerButton);
    m_pointerToolBar->addWidget(storyBubbleToolButton);
    m_pointerToolBar->addWidget(conditionBubbleToolButton);
    m_pointerToolBar->addWidget(actionBubbleToolButton);
    m_pointerToolBar->addWidget(m_sceneScaleCombo);
}


QMenu *CMainWindow::CreateColorMenu(const char *slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
    QStringList names;
    names << tr("black") << tr("white") << tr("red") << tr("blue")
          << tr("yellow");

    QMenu *colorMenu = new QMenu(this);
    for (int i = 0; i < colors.count(); ++i) {
        QAction *action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(CreateColorIcon(colors.at(i)));
        connect(action, SIGNAL(triggered()), this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor)
            colorMenu->setDefaultAction(action);
    }
    return colorMenu;
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
