#include "cgraphicsscene.h"
#include "Connections/arrow.h"

#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QWheelEvent>

#include "Bubbles/cstorybubble.h"
#include "Bubbles/cconditionbubble.h"
#include "Bubbles/cchoicebubble.h"
//#include "Connections/clink.h"
#include "Connections/cline.h"
#include "Connections/cconnection.h"

#include "Connections/clink.h"

#include "Misc/chronicler.h"
using Chronicler::Anchor;


CGraphicsScene::CGraphicsScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent), m_itemMenu(itemMenu), m_mode(Cursor), m_line(0), m_rubberBand(false)
{
    float maxsize = 20000.0;//std::numeric_limits<float>::max();
    float minsize = -20000.0/2;//-std::numeric_limits<float>::max()/2;
    setSceneRect(QRectF(minsize, minsize, maxsize, maxsize));
    setBackgroundBrush(QBrush(Qt::gray));//QColor(86,96,123)));

    m_line = new CLine(QPointF(), QPointF());
}

void CGraphicsScene::setFont(const QFont &font)
{
    if(m_font != font)
    {
        m_font = font;

        for(QGraphicsItem *item : items())
        {
            CBubble *bbl = qgraphicsitem_cast<CBubble *>(item);
            if(bbl)
                bbl->setFont(m_font);
        }
    }
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

        case InsertLine:
            for (int i = 0; i < clickItems.length() && !clickItem; ++i)
                clickItem = qgraphicsitem_cast<CBubble *>(clickItems[i]);

            if(clickItem)
            {
                m_line->setStart(mouseEvent->scenePos());
                addItem(m_line);
            }
            break;

        default:
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
    if (m_mode == InsertLine)
        m_line->setEnd(mouseEvent->scenePos());
    else if (m_mode == Cursor)
        QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void CGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_mode == InsertLine)
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
            endItem = qgraphicsitem_cast<CBubble *>(endItems.first());
            endItems.removeFirst();
        }

        if(items().contains(m_line))
            removeItem(m_line);

        if(startItem && endItem && startItem != endItem)
        {
            Anchor start_anchor = startItem->AnchorAtPosition(m_line->start());
            Anchor end_anchor = endItem->AnchorAtPosition(mouseEvent->scenePos());
            addItem(new CConnection(startItem, endItem, start_anchor, end_anchor, this));
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
        bbl = new CStoryBubble(m_itemMenu, pos, m_palette, m_font);
    else if(type == Chronicler::Condition)
        bbl = new CConditionBubble(m_itemMenu, pos, m_palette, m_font);
    else
        bbl = new CChoiceBubble(m_itemMenu, pos, m_palette, m_font);

    connect(bbl, SIGNAL(Selected(QGraphicsItem*)), this, SIGNAL(itemSelected(QGraphicsItem*)));
    addItem(bbl);

    emit itemInserted(bbl);
}
