#include "cgraphicsscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QButtonGroup>
#include <QAbstractButton>
#include <QDockWidget>
#include <QtMath>
#include <QKeyEvent>

#include "cgraphicsview.h"

#include "Properties/cdockmanager.h"

#include "Bubbles/cstartbubble.h"
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
    float maxsize = 25000.0;
    float minsize = -maxsize/2;
    setSceneRect(QRectF(minsize, minsize, maxsize, maxsize));
    setBackgroundBrush(QBrush(Qt::gray));

    m_line = new CLine(QPointF(), QPointF());
    m_line->hide();
    addItem(m_line);

    connect(this, SIGNAL(itemSelected(QGraphicsItem*)), this, SLOT(ItemSelected(QGraphicsItem*)));

    m_startBubble = dynamic_cast<CStartBubble *>(AddBubble(Chronicler::Start, sceneRect().center(), false));
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

QList<CBubble *> CGraphicsScene::bubbles()
{
    return m_bubbles;
}

QList<CConnection *> CGraphicsScene::connections()
{
    return m_connections;
}

// Grabs the bubble with the highest z value at the given point
CBubble *CGraphicsScene::BubbleAt(const QPointF &point, bool choiceAllowed)
{
    CBubble *bubble = 0;

    for(CBubble *current : m_bubbles)
    {
        if(current->polygon().containsPoint(point - current->scenePos(), Qt::WindingFill))
        {
            if(!bubble || current->zValue() > bubble->zValue())
                bubble = current;
        }
    }

    if(choiceAllowed)
    {
        // Grab the choice out of the choice bubble
        CChoiceBubble *cbbl = dynamic_cast<CChoiceBubble *>(bubble);
        if(cbbl)
        {
            for(CChoice *choice : cbbl->choiceBubbles())
            {
                if(choice->polygon().containsPoint(point - choice->scenePos(), Qt::WindingFill))
                {
                    bubble = choice;
                    break;
                }
            }
        }
    }
    return bubble;
}

QDataStream &operator <<(QDataStream &ds, const CGraphicsScene &scene)
{
    ds << scene.m_name << scene.m_bubbles.length();
    for(CBubble *bbl : scene.m_bubbles)
        bbl->Write(ds);

    return ds;
}

QDataStream &operator >>(QDataStream &ds, CGraphicsScene &scene)
{
    int len;
    ds >> len;

    for(int i = 0; i < len; ++i)
    {
        int t;
        ds >> t;

        CBubble *bbl;
        if(t == int(Chronicler::Start))
            bbl = scene.m_startBubble;
        else
            bbl = scene.AddBubble(Chronicler::BubbleType(t), QPointF(), false);

        bbl->Read(ds);
    }

    for(CConnection *connection : scene.m_connections)
        connection->ConnectToUIDs();

    for(CBubble *bbl : scene.m_bubbles)
        bbl->setSelected(false);

    return ds;
}

void CGraphicsScene::setMode(Mode mode)
{
    m_mode = mode;

    if(views().size())
    {
        views().first()->setDragMode(QGraphicsView::ScrollHandDrag);

        if(mode == Cursor)
        {
            shared().pointerTypeGroup->button(int(CGraphicsScene::Cursor))->setChecked(true);
            m_line->hide();
        }
        else if(mode == InsertConnection)
            views().first()->setDragMode(QGraphicsView::NoDrag);
    }
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
            clickItem = BubbleAt(mouseEvent->scenePos(), true);
            if(clickItem)
            {
                Anchor out = clickItem->OutputAnchorAtPosition(mouseEvent->scenePos());
                if(out != Anchor::None)
                {
                    m_line->setStart(mouseEvent->scenePos());
                    m_line->setStartAnchor(out);
                    m_line->setPalette(clickItem->getPalette());
                    m_line->show();
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
        CBubble *hoverItem = BubbleAt(mouseEvent->scenePos());
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
        if(m_line->isVisible())
        {
            m_line->hide();

            CBubble *startItem = BubbleAt(m_line->start(), true);
            CBubble *endItem = BubbleAt(m_line->end());

            if(startItem && endItem && startItem != endItem)
            {
                Anchor start_anchor = m_line->startAnchor();
                Anchor end_anchor = m_line->endAnchor();
                if(start_anchor != Anchor::None && end_anchor != Anchor::None)
                    AddConnection(startItem, endItem, start_anchor, end_anchor);
            }
        }

        if(!(mouseEvent->modifiers() & Qt::ShiftModifier))
            setMode(Cursor);
    }

    m_rubberBand = false;

    // calculate selected bubbles...
    QGraphicsScene::mouseReleaseEvent(mouseEvent);

    // set the dock as the active widget
    if(!shared().dock->isHidden())
        shared().dock->activateWindow();

    // update the bubble properties with the selected bubble
    QList<QGraphicsItem *> selected = selectedItems();
    if(selected.size() == 1)
    {
        CBubble *bbl = dynamic_cast<CBubble *>(selected.first());
        shared().dockManager->setBubble(bbl);
    }
    else
        shared().dockManager->setBubble(0, (selected.size() > 0)); // switch to project view if none selected

    emit leftReleased();
}

void CGraphicsScene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Shift)
        views().first()->setDragMode(QGraphicsView::RubberBandDrag);
    else if(event->key() == Qt::Key_Escape)
        setMode(Cursor);
}

void CGraphicsScene::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Shift && !m_line->isVisible())
        setMode(Cursor);
}


CBubble * CGraphicsScene::AddBubble(BubbleType type, const QPointF &pos, bool shift)
{
    CBubble *bbl;
    if(type == Chronicler::Story)
        bbl = new CStoryBubble(pos, m_palette, m_font);
    else if(type == Chronicler::Condition)
        bbl = new CConditionBubble(pos, m_palette, m_font);
    else if(type == Chronicler::Action)
        bbl = new CActionBubble(pos, m_palette, m_font);
    else if(type == Chronicler::Choice)
        bbl = new CChoiceBubble(pos, m_palette, m_font);
    else if(type == Chronicler::Start)
        bbl = new CStartBubble(pos, m_palette, m_font);

    connect(bbl, SIGNAL(Selected(QGraphicsItem*)), this, SIGNAL(itemSelected(QGraphicsItem*)));
    addItem(bbl);
    bbl->setSelected(true);

    m_bubbles.append(bbl);

    if(!shift)
        setMode(CGraphicsScene::Cursor);

    emit itemInserted(bbl);

    return bbl;
}

CConnection *CGraphicsScene::AddConnection(CBubble *start, CBubble *end, Anchor start_anchor, Anchor end_anchor)
{
    m_connections.append(new CConnection(start, end, start_anchor, end_anchor, this));
    addItem(m_connections.last());

    return m_connections.last();
}

CConnection *CGraphicsScene::AddConnection()
{
    m_connections.append(new CConnection(this));
    addItem(m_connections.last());

    return m_connections.last();
}

void CGraphicsScene::RemoveBubble(CBubble *bubble)
{
    m_bubbles.removeAll(bubble);
}

void CGraphicsScene::RemoveConnection(CConnection *connection)
{
    m_connections.removeAll(connection);
}

