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


CGraphicsScene::CGraphicsScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent), m_itemMenu(itemMenu), m_mode(Cursor), m_line(0), m_rubberBand(false)
{
    float maxsize = 15000.0;//std::numeric_limits<float>::max();
    float minsize = -15000.0/2;//-std::numeric_limits<float>::max()/2;
    setSceneRect(QRectF(minsize, minsize, maxsize, maxsize));
    setBackgroundBrush(QBrush(Qt::gray));//QColor(86,96,123)));
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
            if(items(mouseEvent->scenePos()).length() > 0)
            {
                m_line = new CLine(mouseEvent->scenePos(), mouseEvent->scenePos());
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
    if (m_mode == InsertLine && m_line != 0)
        m_line->setEnd(mouseEvent->scenePos());
    else if (m_mode == Cursor)
        QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void CGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_line != 0 && m_mode == InsertLine)
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

        removeItem(m_line);
        delete m_line;
        m_line = 0;

        if(startItem && endItem)
        {
            addItem(new CConnection(startItem, endItem, this));
        }
    }

    QGraphicsScene::mouseReleaseEvent(mouseEvent);

    m_rubberBand = false;

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
