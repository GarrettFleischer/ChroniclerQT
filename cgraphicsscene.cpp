#include "cgraphicsscene.h"

#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QWheelEvent>

#include "Bubbles/cstorybubble.h"
#include "Bubbles/cconditionbubble.h"
#include "Bubbles/cchoicebubble.h"
#include "Bubbles/cchoice.h"
#include "Bubbles/cactionbubble.h"
#include "Connections/cline.h"
#include "Connections/cconnection.h"

#include "Misc/chronicler.h"
using Chronicler::Anchor;

#include <QDebug>


CGraphicsScene::CGraphicsScene(QObject *parent)
    : QGraphicsScene(parent), m_mode(Cursor), m_line(0), m_rubberBand(false)
{
    float maxsize = 20000.0;//std::numeric_limits<float>::max();
    float minsize = -20000.0/2;//-std::numeric_limits<float>::max()/2;
    setSceneRect(QRectF(minsize, minsize, maxsize, maxsize));
    setBackgroundBrush(QBrush(Qt::gray));//QColor(86,96,123)));

    m_line = new CLine(QPointF(), QPointF());
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
            AddBubble(Chronicler::Story, mouseEvent->scenePos());
            break;

        case InsertCondition:
            AddBubble(Chronicler::Condition, mouseEvent->scenePos());
            break;

        case InsertChoice:
            AddBubble(Chronicler::Choice, mouseEvent->scenePos());
            break;

        case InsertAction:
            AddBubble(Chronicler::Action, mouseEvent->scenePos());
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
    emit leftReleased();
}

void CGraphicsScene::AddBubble(BubbleType type, const QPointF &pos)
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

    emit itemInserted(bbl);
}
