#include "cgraphicsscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QButtonGroup>
#include <QAbstractButton>
#include <QDockWidget>
#include <QtMath>
#include <QKeyEvent>
#include <QMenu>
#include <QGuiApplication>
#include <QAction>

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

#include "Misc/Palette/cpalettebutton.h"
#include "Misc/Palette/cpaletteaction.h"

#include "Properties/cprojectview.h"

#include "cmainwindow.h"

#include "Misc/chronicler.h"
using Chronicler::Anchor;
using Chronicler::shared;

CGraphicsScene::CGraphicsScene(bool create_start, const QString &name, QObject *parent)
    : QGraphicsScene(parent), m_name(name), m_line(0), m_rubberBand(false)
{
    float maxsize = 25000.0;
    float minsize = -maxsize/2;
    setSceneRect(QRectF(minsize, minsize, maxsize, maxsize));
    setBackgroundBrush(QBrush(Qt::gray));

    m_line = new CLine(QPointF(), QPointF());
    m_line->hide();
    addItem(m_line);

    connect(this, SIGNAL(itemSelected(QGraphicsItem*)), this, SLOT(ItemSelected(QGraphicsItem*)));
    connect(this, SIGNAL(changed(QList<QRectF>)), this, SLOT(UpdateSceneRect()));

    if(create_start)
        m_startBubble = dynamic_cast<CStartBubble *>(AddBubble(Chronicler::Start, sceneRect().center(), false));
}

void CGraphicsScene::setFont(const QFont &font)
{
    m_font = font;

    for(QGraphicsItem *item : items())
    {
        CBubble *bbl = dynamic_cast<CBubble *>(item);
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

CLine *CGraphicsScene::getLine()
{
    return m_line;
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

QDataStream &CGraphicsScene::Serialize(QDataStream &ds)
{
    ds << m_name << static_cast<qint32>(m_bubbles.length());
    for(CBubble *bbl : m_bubbles)
        ds << *bbl;

    return ds;
}

QDataStream &CGraphicsScene::Deserialize(QDataStream &ds, const Chronicler::CVersion &version)
{
    Q_UNUSED(version)

    qint32 len, t;
    CBubble *bbl;
    ds >> len;

    for(int i = 0; i < len; ++i)
    {
        ds >> t;
        bbl = AddBubble(Chronicler::BubbleType(t), QPointF(), false);
        ds >> *bbl;

        if(bbl->getType() == Chronicler::Start)
            m_startBubble = dynamic_cast<CStartBubble*>(bbl);
    }

    for(CConnection *connection : m_connections)
        connection->ConnectToUIDs();

    for(CBubble *bbl : m_bubbles)
        bbl->setSelected(false);

    return ds;
}

void CGraphicsScene::ItemSelected(QGraphicsItem *selectedItem)
{
    if(!m_rubberBand)
    {
        // decrease all z values by a ridiculously small number
        // to preserve current stacking order & help prevent float overflow
        foreach (QGraphicsItem *item, items())
            item->setZValue(item->zValue() - qPow(1, -10));

        // bring the selected item to the front
        selectedItem->setZValue(1);
    }
}

void CGraphicsScene::SelectAll()
{
    for(QGraphicsItem *itm : items())
        itm->setSelected(true);
}

void CGraphicsScene::UpdateSceneRect()
{
    if(views().length())
    {
        QRectF ib = itemsBoundingRect();
        QRectF sb = sceneRect();
//        QRectF vb = views().first()->visibleRegion().boundingRect();
        qreal p = 1000;

        setSceneRect(QRectF(QPointF(qMin(ib.left() - p, sb.left()), qMin(ib.top() - p, sb.top())),
                            QPointF(qMax(ib.right() + p, sb.right()), qMax(ib.bottom() + p, sb.bottom()))));

//        if(selectedItems().length())
//            views().first()->ensureVisible(selectedItems().first(), 100, 100);
    }
}

void CGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        CBubble *clickItem = BubbleAt(event->scenePos(), true);

        if(!(event->modifiers() & Qt::ControlModifier) && (!clickItem || !clickItem->isSelected()))
            for(QGraphicsItem *item : items())
                item->setSelected(false);

        switch (shared().cursorMode)
        {
        case Chronicler::InsertStory:
            AddBubble(Chronicler::Story, event->scenePos(), (event->modifiers() & Qt::ShiftModifier));
            break;

        case Chronicler::InsertCondition:
            AddBubble(Chronicler::Condition, event->scenePos(), (event->modifiers() & Qt::ShiftModifier));
            break;

        case Chronicler::InsertChoice:
            AddBubble(Chronicler::Choice, event->scenePos(), (event->modifiers() & Qt::ShiftModifier));
            break;

        case Chronicler::InsertAction:
            AddBubble(Chronicler::Action, event->scenePos(), (event->modifiers() & Qt::ShiftModifier));
            break;

        case Chronicler::InsertConnection:
            if(clickItem)
            {
                Anchor out = clickItem->OutputAnchorAtPosition(event->scenePos());
                if(out != Anchor::None)
                {
                    m_line->setStart(event->scenePos());
                    m_line->setStartAnchor(out);
                    m_line->setPalette(clickItem->getPalette());
                    m_line->show();
                }
            }
            break;

        case Chronicler::Paint:
                if(clickItem)
                    clickItem->container()->setPalette(shared().paletteButton->getPalette());
            break;

        case Chronicler::Cursor:
            break;
        }

        QGraphicsScene::mousePressEvent(event);

        if(selectedItems().size() == 0)
            m_rubberBand = true;

        emit leftPressed();
    }
    else if(event->button() == Qt::RightButton)
    {
        if(shared().cursorMode == Chronicler::Paint)
        {
            CBubble *clickItem = BubbleAt(event->scenePos(), true);
            if(clickItem)
                shared().paletteButton->setCurrent(clickItem->getPalette());
        }
    }
}

void CGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (shared().cursorMode == Chronicler::InsertConnection)
    {
        CBubble *hoverItem = BubbleAt(event->scenePos());
        if(hoverItem)
            m_line->setEndAnchor(hoverItem->InputAnchorAtPosition(event->scenePos()));

        m_line->setEnd(event->scenePos());
    }
    else if (shared().cursorMode == Chronicler::Cursor)
        QGraphicsScene::mouseMoveEvent(event);
}

void CGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if (shared().cursorMode == Chronicler::InsertConnection)
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

            if(!(event->modifiers() & Qt::ShiftModifier))
                shared().setMode(Chronicler::Cursor);
        }

        m_rubberBand = false;

        // calculate selected bubbles...
        QGraphicsScene::mouseReleaseEvent(event);

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
}

void CGraphicsScene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Shift)
        views().first()->setDragMode(QGraphicsView::RubberBandDrag);
}

void CGraphicsScene::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Shift && !m_line->isVisible())
        shared().setMode(Chronicler::Cursor);
}


CBubble * CGraphicsScene::AddBubble(BubbleType type, const QPointF &pos, bool shift)
{
    CBubble *bbl = 0;
    if(type == Chronicler::Story)
        bbl = new CStoryBubble(pos, shared().defaultStory, m_font);
    else if(type == Chronicler::Condition)
        bbl = new CConditionBubble(pos, shared().defaultCondition, m_font);
    else if(type == Chronicler::Action)
        bbl = new CActionBubble(pos, shared().defaultAction, m_font);
    else if(type == Chronicler::Choice)
        bbl = new CChoiceBubble(pos, shared().defaultChoice, m_font);
    else if(type == Chronicler::Start)
        bbl = new CStartBubble(pos, shared().defaultStart, m_font);

    if(bbl)
    {
        connect(bbl, SIGNAL(Selected(QGraphicsItem*)), this, SIGNAL(itemSelected(QGraphicsItem*)));
        addItem(bbl);
        bbl->setSelected(true);

        m_bubbles.append(bbl);

        if(!shift)
            shared().setMode(Chronicler::Cursor);

        emit itemInserted(bbl);
    }

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

CStartBubble *CGraphicsScene::startBubble()
{
    return m_startBubble;
}



void CGraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if(shared().cursorMode == Chronicler::InsertConnection)
    {
        CBubble *clickItem = BubbleAt(event->scenePos(), true);
        if(clickItem)
            clickItem->RemoveLink(clickItem->OutputAnchorAtPosition(event->scenePos()));
    }
    else if(shared().cursorMode == Chronicler::Cursor)
    {
        CBubble *bbl = BubbleAt(event->scenePos());
        if(bbl && !bbl->isSelected())
        {
            bbl->scene()->clearSelection();
            bbl->setSelected(true);
        }

        shared().editMenu->exec(event->screenPos());
    }
}
