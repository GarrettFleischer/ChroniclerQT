#include "mainwindow.h"

#include "Connections/arrow.h"
#include "cgraphicsscene.h"
#include "Bubbles/cstorybubble.h"
#include "cgraphicsview.h"
#include "chomepage.h"

#include <QtWidgets>
#include <QStringListModel>
#include <QtGlobal>
#include <limits>
#include <QTabWidget>



const int InsertTextButton = 10;


MainWindow::MainWindow()
    : m_ShiftHeld(false), m_scale(1)
{
    setWindowTitle(tr("Chronicler-Next"));
    setUnifiedTitleAndToolBarOnMac(true);

    createActions();
    createMenus();

    float maxsize = std::numeric_limits<float>::max();
    float minsize = -std::numeric_limits<float>::max()/2;
    scene = new CGraphicsScene(itemMenu, this);
    scene->setSceneRect(QRectF(minsize, minsize, maxsize, maxsize));
    connect(scene, SIGNAL(itemInserted(CBubble*)),
            this, SLOT(itemInserted(CBubble*)));
    connect(scene, SIGNAL(itemSelected(QGraphicsItem*)),
            this, SLOT(itemSelected(QGraphicsItem*)));
    connect(scene, SIGNAL(leftReleased()),
            this, SLOT(sceneLeftReleased()));
    connect(scene, SIGNAL(leftPressed()),
            this, SLOT(sceneLeftPressed()));

    view = new CGraphicsView(scene);

    QStringList lst = QStringList() << "*set" << "*action" << "*create" << "*if" << "*elseif" << "${name}" << "${title}" << "${strength}";
    QStringListModel * lstModel = new QStringListModel(lst, this);

    dock = new QDockWidget("Project", this);
    properties = new CPropertiesView(lstModel, dock);
    dock->setWidget(properties);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    CHomepage *home = new CHomepage(this);

    tabView = new QTabWidget(this);
    tabView->addTab(home,"Homepage");
    tabView->addTab(view, "startup.scn");
    setCentralWidget(tabView);
    addDockWidget(Qt::LeftDockWidgetArea, dock);


    createToolbars();
    handleFontChange();
}

void MainWindow::LoadProject(const QString &filename)
{

}


void MainWindow::keyPressEvent(QKeyEvent *evt)
{
    QMainWindow::keyPressEvent(evt);

    if(evt->key() == Qt::Key_Shift)
        m_ShiftHeld = true;
    else if(evt->key() == Qt::Key_Delete)
        deleteItem();
}

void MainWindow::keyReleaseEvent(QKeyEvent *evt)
{
    QMainWindow::keyReleaseEvent(evt);

    if(evt->key() == Qt::Key_Shift)
    {
        m_ShiftHeld = false;
        pointerTypeGroup->button(int(CGraphicsScene::Cursor))->setChecked(true);
        scene->setMode(CGraphicsScene::Cursor);
        view->setDragMode(QGraphicsView::ScrollHandDrag);
    }
}


void MainWindow::deleteItem()
{
    properties->SetBubble(0);

    foreach (QGraphicsItem *item, scene->selectedItems())
        delete item;
}


void MainWindow::pointerGroupClicked(int id)
{
    scene->setMode(CGraphicsScene::Mode(id));

    if(id == int(CGraphicsScene::InsertLine))
        view->setDragMode(QGraphicsView::NoDrag);
    else
        view->setDragMode(QGraphicsView::ScrollHandDrag);
}


void MainWindow::itemInserted(CBubble *)
{
    if(!m_ShiftHeld)
    {
        pointerTypeGroup->button(int(CGraphicsScene::Cursor))->setChecked(true);
        scene->setMode(CGraphicsScene::Cursor);
        view->setDragMode(QGraphicsView::ScrollHandDrag);
    }
}


void MainWindow::currentFontChanged(const QFont &)
{
    handleFontChange();
}


void MainWindow::fontSizeChanged(const QString &)
{
    handleFontChange();
}


void MainWindow::sceneScaleChanged(const QString &scale)
{
    m_scale = scale.left(scale.indexOf(tr("%"))).toFloat() / 100.0;
    UpdateSceneScale();
}

void MainWindow::UpdateSceneScale()
{
    //    QMatrix oldMatrix = view->matrix();
    //    QMatrix newMatrix;
    //    //view->resetMatrix();
    //   newMatrix.translate(oldMatrix.dx(), oldMatrix.dy());
    //   view->setMatrix(newMatrix);
    //view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    view->scale(m_scale, m_scale);
}


void MainWindow::textColorChanged()
{
    textAction = qobject_cast<QAction *>(sender());
    fontColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/textpointer.png",
                                     qvariant_cast<QColor>(textAction->data())));
    textButtonTriggered();
}


void MainWindow::itemColorChanged()
{
    fillAction = qobject_cast<QAction *>(sender());
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/floodfill.png",
                                     qvariant_cast<QColor>(fillAction->data())));
    fillButtonTriggered();
}


void MainWindow::lineColorChanged()
{
    lineAction = qobject_cast<QAction *>(sender());
    lineColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/linecolor.png",
                                     qvariant_cast<QColor>(lineAction->data())));
    lineButtonTriggered();
}


void MainWindow::textButtonTriggered()
{
    scene->setTextColor(qvariant_cast<QColor>(textAction->data()));
}


void MainWindow::fillButtonTriggered()
{
    scene->setItemColor(qvariant_cast<QColor>(fillAction->data()));
}


void MainWindow::lineButtonTriggered()
{
    scene->setLineColor(qvariant_cast<QColor>(lineAction->data()));
}


void MainWindow::handleFontChange()
{
    QFont font = fontCombo->currentFont();
    font.setPointSize(fontSizeCombo->value());
    font.setWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(italicAction->isChecked());
    font.setUnderline(underlineAction->isChecked());

    scene->setFont(font);
    properties->setFont(font);
}


void MainWindow::itemSelected(QGraphicsItem *selectedItem)
{
    if(!scene->isRubberBandSelecting())
    {
        foreach (QGraphicsItem *item, scene->items())
            item->setZValue(item->zValue() - qPow(1, -10));

        selectedItem->setZValue(1);
    }
}


void MainWindow::sceneLeftPressed()
{
    properties->SetBubble(0);
}

void MainWindow::sceneLeftReleased()
{
    if(!dock->isHidden())
        dock->activateWindow();
    QList<QGraphicsItem *> selected = scene->selectedItems();
    if(selected.size() == 1)
    {
        CBubble *bbl = dynamic_cast<CBubble *>(selected.first());
        properties->SetBubble(bbl);
    }
}


void MainWindow::about()
{
    QMessageBox::about(this, tr("About Chronicler-Next"), tr("<b>Insert legal stuff here...</b>"));
}


void MainWindow::createActions()
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

    deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from diagram"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit Scenediagram example"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    boldAction = new QAction(tr("Bold"), this);
    boldAction->setCheckable(true);
    QPixmap pixmap(":/images/bold.png");
    boldAction->setIcon(QIcon(pixmap));
    boldAction->setShortcut(tr("Ctrl+B"));
    connect(boldAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    italicAction = new QAction(QIcon(":/images/italic.png"), tr("Italic"), this);
    italicAction->setCheckable(true);
    italicAction->setShortcut(tr("Ctrl+I"));
    connect(italicAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    underlineAction = new QAction(QIcon(":/images/underline.png"), tr("Underline"), this);
    underlineAction->setCheckable(true);
    underlineAction->setShortcut(tr("Ctrl+U"));
    connect(underlineAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    aboutAction = new QAction(tr("A&bout"), this);
    aboutAction->setShortcut(tr("Ctrl+B"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}


void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);

    itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    //    itemMenu->addAction(toFrontAction);
    //    itemMenu->addAction(sendBackAction);

    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(aboutAction);
}


void MainWindow::createToolbars()
{
    fontCombo = new QFontComboBox();
    connect(fontCombo, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(currentFontChanged(QFont)));
    fontCombo->setCurrentText("Times New Roman");


    fontSizeCombo = new QSpinBox;
    fontSizeCombo->setRange(8, 42);
    fontSizeCombo->setValue(11);
    connect(fontSizeCombo, SIGNAL(valueChanged(QString)),
            this, SLOT(fontSizeChanged(QString)));

    fontColorToolButton = new QToolButton;
    fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()), Qt::black));
    textAction = fontColorToolButton->menu()->defaultAction();
    fontColorToolButton->setIcon(createColorToolButtonIcon(":/images/textpointer.png", Qt::black));
    fontColorToolButton->setAutoFillBackground(true);
    connect(fontColorToolButton, SIGNAL(clicked()),
            this, SLOT(textButtonTriggered()));

    fillColorToolButton = new QToolButton;
    fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()), Qt::white));
    fillAction = fillColorToolButton->menu()->defaultAction();
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/floodfill.png", Qt::white));
    connect(fillColorToolButton, SIGNAL(clicked()),
            this, SLOT(fillButtonTriggered()));

    lineColorToolButton = new QToolButton;
    lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    lineColorToolButton->setMenu(createColorMenu(SLOT(lineColorChanged()), Qt::black));
    lineAction = lineColorToolButton->menu()->defaultAction();
    lineColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/linecolor.png", Qt::black));
    connect(lineColorToolButton, SIGNAL(clicked()),
            this, SLOT(lineButtonTriggered()));

    textToolBar = addToolBar(tr("Font"));
    textToolBar->addWidget(fontCombo);
    textToolBar->addWidget(fontSizeCombo);
    textToolBar->addAction(boldAction);
    textToolBar->addAction(italicAction);
    textToolBar->addAction(underlineAction);

    colorToolBar = addToolBar(tr("Color"));
    colorToolBar->addWidget(fontColorToolButton);
    colorToolBar->addWidget(fillColorToolButton);
    colorToolBar->addWidget(lineColorToolButton);

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


    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(CGraphicsScene::Cursor));
    pointerTypeGroup->addButton(linePointerButton, int(CGraphicsScene::InsertLine));
    pointerTypeGroup->addButton(storyBubbleToolButton, int(CGraphicsScene::InsertStory));
    pointerTypeGroup->addButton(conditionBubbleToolButton, int(CGraphicsScene::InsertCondition));
    pointerTypeGroup->addButton(actionBubbleToolButton, int(CGraphicsScene::InsertChoice));
    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(pointerGroupClicked(int)));

    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(2);
    connect(sceneScaleCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(sceneScaleChanged(QString)));

    pointerToolBar = addToolBar(tr("Pointer type"));
    pointerToolBar->addWidget(pointerButton);
    pointerToolBar->addWidget(linePointerButton);
    pointerToolBar->addWidget(storyBubbleToolButton);
    pointerToolBar->addWidget(conditionBubbleToolButton);
    pointerToolBar->addWidget(actionBubbleToolButton);
    pointerToolBar->addWidget(sceneScaleCombo);
}


QMenu *MainWindow::createColorMenu(const char *slot, QColor defaultColor)
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
        action->setIcon(createColorIcon(colors.at(i)));
        connect(action, SIGNAL(triggered()), this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor)
            colorMenu->setDefaultAction(action);
    }
    return colorMenu;
}


QIcon MainWindow::createColorToolButtonIcon(const QString &imageFile, QColor color)
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


QIcon MainWindow::createColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), color);

    return QIcon(pixmap);
}
