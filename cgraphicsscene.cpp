#include "cgraphicsscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QButtonGroup>
#include <QAbstractButton>
#include <QDockWidget>
#include <QtMath>
#include <QKeyEvent>

#include "cgraphicsview.h"

#include "Properties/cdockmanager.h"

#include "Bubbles/cstorybubble.h"
#include "Bubbles/cconditionbubble.h"
#include "Bubbles/cchoicebubble.h"
#include "Bubbles/cchoice.h"
#include "Bubbles/cactionbubble.h"
#include "Connections/cline.h"
#include "Connections/cconnection.h"

#include "Misc/chronicler.h"
using Chronicler::Anchor;
using Chronicler::shared;


CGraphicsScene::CGraphicsScene(const QString &name, QObject *parent)
    : QGraphicsScene(parent), m_name(name), m_mode(Cursor), m_line(0), m_rubberBand(false)
{
    float maxsize = 20000.0;//std::numeric_limits<float>::max();
    float minsize = -20000.0/2;//-std::numeric_limits<float>::max()/2;
    setSceneRect(QRectF(minsize, minsize, maxsize, maxsize));
    setBackgroundBrush(QBrush(Qt::gray));//QColor(86,96,123)));

    m_line = new CLine(QPointF(), QPointF());

    connect(this, SIGNAL(itemSelected(QGraphicsItem*)), this, SLOT(ItemSelected(QGraphicsItem*)));
}

void CGraphicsScene::setFont(const QFont &font)
{
    m_font = font;

    for(QGraphicsItem *item : items())
    {
        CBubble *bbl = qgraphicsitem_cast<CBubble *>(item);
        if(bbl)
            bbl->setFont(m_font);
    }
}

QString CGraphicsScene::name()
{
    return m_name;
}

void CGraphicsScene::setName(const QString &name)
{
    m_name = name;
    emit nameChanged();
}

void CGraphicsScene::setPalette(const CPalette &palette)
{
    m_palette = palette;
    for(QGraphicsItem *item : selectedItems())
    {
        CBubble *bbl = qgraphicsitem_cast<CBubble *>(item);
        if(bbl)
            bbl->setPalette(m_palette);
    }
}

void CGraphicsScene::setMode(Mode mode)
{
    m_mode = mode;

    views().first()->setDragMode(QGraphicsView::ScrollHandDrag);

    if(mode == Cursor)
        shared().pointerTypeGroup->button(int(CGraphicsScene::Cursor))->setChecked(true);
    else if(mode == InsertConnection)
        views().first()->setDragMode(QGraphicsView::NoDrag);
}

void CGraphicsScene::ItemSelected(QGraphicsItem *selectedItem)
{
    if(!m_rubberBand)
    {
        // decrease all z values by a ridiculously small number
        // to preserve current stacking order & help prevent integer overflow
        foreach (QGraphicsItem *item, items())
            item->setZValue(item->zValue() - qPow(1, -10));

        // bring the selected item to the front
        selectedItem->setZValue(1);
    }
}

void CGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton)
    {
        if(!(mouseEvent->modifiers() & Qt::ControlModifier) && selectedItems().size() == 1)
            for(QGraphicsItem *item : items())
                item->setSelected(false);

        CBubble *clickItem = 0;
        QList<QGraphicsItem *> clickItems = items(mouseEvent->scenePos());

        switch (m_mode)
        {
        case InsertStory:
            AddBubble(Chronicler::Story, mouseEvent->scenePos(), (mouseEvent->modifiers() & Qt::ShiftModifier));
            break;

        case InsertCondition:
            AddBubble(Chronicler::Condition, mouseEvent->scenePos(), (mouseEvent->modifiers() & Qt::ShiftModifier));
            break;

        case InsertChoice:
            AddBubble(Chronicler::Choice, mouseEvent->scenePos(), (mouseEvent->modifiers() & Qt::ShiftModifier));
            break;

        case InsertAction:
            AddBubble(Chronicler::Action, mouseEvent->scenePos(), (mouseEvent->modifiers() & Qt::ShiftModifier));
            break;

        case InsertConnection:
            for (int i = 0; i < clickItems.length() && !clickItem; ++i)
                clickItem = qgraphicsitem_cast<CBubble *>(clickItems[i]);

            if(clickItem)
            {
                Anchor out = clickItem->OutputAnchorAtPosition(mouseEvent->scenePos());
                if(out != Anchor::None)
                {
                    m_line->setStart(mouseEvent->scenePos());
                    m_line->setStartAnchor(out);
                    m_line->setPalette(clickItem->getPalette());
                    addItem(m_line);
                }
            }
            break;

        case Cursor:
            break;
        }

        QGraphicsScene::mousePressEvent(mouseEvent);

        if(selectedItems().size() == 0)
            m_rubberBand = true;

        emit leftPressed();
    }
}

void CGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_mode == InsertConnection)
    {
        CBubble *hoverItem = 0;
        QList<QGraphicsItem *> hoverItems = items(mouseEvent->scenePos());

        while (hoverItems.count() && !hoverItem)
        {
            if(!dynamic_cast<CChoice *>(hoverItems.first()))
                hoverItem = qgraphicsitem_cast<CBubble *>(hoverItems.first());
            hoverItems.removeFirst();
        }
        if(hoverItem)
            m_line->setEndAnchor(hoverItem->InputAnchorAtPosition(mouseEvent->scenePos()));

        m_line->setEnd(mouseEvent->scenePos());
    }
    else if (m_mode == Cursor)
        QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void CGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_mode == InsertConnection)
    {
        CBubble *startItem = 0;
        CBubble *endItem = 0;

        QList<QGraphicsItem *> startItems = items(m_line->start());
        while (startItems.count() && !startItem)
        {
            startItem = qgraphicsitem_cast<CBubble *>(startItems.first());
            startItems.removeFirst();
        }
        QList<QGraphicsItem *> endItems = items(m_line->end());
        while (endItems.count() && !endItem)
        {
            if(!dynamic_cast<CChoice *>(endItems.first()))
                endItem = qgraphicsitem_cast<CBubble *>(endItems.first());
            endItems.removeFirst();
        }

        if(items().contains(m_line))
        {
            removeItem(m_line);

            if(startItem && endItem && startItem != endItem)
            {
                Anchor start_anchor = m_line->startAnchor();
                Anchor end_anchor = m_line->endAnchor();
                if(start_anchor != Anchor::None && end_anchor != Anchor::None)
                    addItem(new CConnection(startItem, endItem, start_anchor, end_anchor, this));
            }
        }
    }

    m_rubberBand = false;

    QGraphicsScene::mouseReleaseEvent(mouseEvent);

    if(!shared().dock->isHidden())
        shared().dock->activateWindow();
    QList<QGraphicsItem *> selected = selectedItems();
    if(selected.size() == 1)
    {
        CBubble *bbl = dynamic_cast<CBubble *>(selected.first());
        shared().dockManager->setBubble(bbl);
    }
    else
        shared().dockManager->setBubble(0, selected.size());

    emit leftReleased();
}

void CGraphicsScene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Shift)
        views().first()->setDragMode(QGraphicsView::RubberBandDrag);
}


void CGraphicsScene::AddBubble(BubbleType type, const QPointF &pos, bool shift)
{
    CBubble *bbl;
    if(type == Chronicler::Story)
        bbl = new CStoryBubble(pos, m_palette, m_font);
    else if(type == Chronicler::Condition)
        bbl = new CConditionBubble(pos, m_palette, m_font);
    else if(type == Chronicler::Action)
        bbl = new CActionBubble(pos, m_palette, m_font);
    else
        bbl = new CChoiceBubble(pos, m_palette, m_font);

    connect(bbl, SIGNAL(Selected(QGraphicsItem*)), this, SIGNAL(itemSelected(QGraphicsItem*)));
    addItem(bbl);
    bbl->setSelected(true);

    if(!shift)
        setMode(CGraphicsScene::Cursor);

    emit itemInserted(bbl);
}

