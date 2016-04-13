#include "cgraphicsscene.h"
#include "Connections/arrow.h"

#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QWheelEvent>

#include "Bubbles/cstorybubble.h"
#include "Bubbles/cconditionbubble.h"
#include "Connections/clink.h"
#include "Connections/cline.h"


CGraphicsScene::CGraphicsScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    m_itemMenu = itemMenu;
    m_mode = Cursor;
    m_line = 0;
    m_itemColor = Qt::white;
    m_textColor = Qt::black;
    m_lineColor = Qt::black;
    m_rubberBand = false;

    setBackgroundBrush(QBrush(Qt::gray));//QColor(86,96,123)));
}

void CGraphicsScene::setLineColor(const QColor &color)
{
    m_lineColor = color;
    foreach (QGraphicsItem *item, selectedItems())
    {
        CBubble *bbl = qgraphicsitem_cast<CBubble *>(item);
        if(bbl)
            bbl->SetLineColor(m_lineColor);
    }
}

void CGraphicsScene::setTextColor(const QColor &color)
{
    m_textColor = color;
    foreach (QGraphicsItem *item, selectedItems())
    {
        CBubble *bbl = qgraphicsitem_cast<CBubble *>(item);
        if(bbl)
            bbl->SetFontColor(m_textColor);
    }
}

void CGraphicsScene::setItemColor(const QColor &color)
{
    m_itemColor = color;
    foreach (QGraphicsItem *item, selectedItems())
    {
        CBubble *bbl = qgraphicsitem_cast<CBubble *>(item);
        if(bbl)
            bbl->SetColor(m_itemColor);
    }
}

void CGraphicsScene::setFont(const QFont &font)
{
    if(m_font != font)
    {
        m_font = font;

        foreach (QGraphicsItem *item, items())
        {
            CBubble *bbl = qgraphicsitem_cast<CBubble *>(item);
            if(bbl)
                bbl->SetFont(m_font);
        }
    }
}

void CGraphicsScene::setMode(Mode mode)
{
    m_mode = mode;
}

void CGraphicsScene::linkClicked(CLink *)
{
    setMode(InsertLine);
    
}

void CGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton)
    {
        CBubble *item;
        switch (m_mode) {
        case InsertStory:
            item = new CStoryBubble(m_itemMenu, mouseEvent->scenePos(), m_font, m_textColor, m_lineColor);
            connect(item, SIGNAL(Selected(QGraphicsItem*)), this, SIGNAL(itemSelected(QGraphicsItem*)));
            addItem(item);
            emit itemInserted(item);
            break;

        case InsertChoice:

            break;

        case InsertCondition:
            item = new CConditionBubble(m_itemMenu, mouseEvent->scenePos(), m_font, m_textColor, m_lineColor);
            connect(item, SIGNAL(Selected(QGraphicsItem*)), this, SIGNAL(itemSelected(QGraphicsItem*)));
            addItem(item);
            emit itemInserted(item);
            break;

        case InsertLine:
            m_line = new CLine(mouseEvent->scenePos(), mouseEvent->scenePos());
            addItem(m_line);
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
    //    if (line != 0 && myMode == InsertLine)
    //    {
    //        QList<QGraphicsItem *> startItems = items(line->line().p1());
    //        if (startItems.count() && startItems.first() == line)
    //            startItems.removeFirst();
    //        QList<QGraphicsItem *> endItems = items(line->line().p2());
    //        if (endItems.count() && endItems.first() == line)
    //            endItems.removeFirst();

    //        removeItem(line);
    //        delete line;

    //        if (startItems.count() > 0 && endItems.count() > 0 &&
    //                startItems.first() != endItems.first())
    //        {
    //            CBubble *startItem = qgraphicsitem_cast<CBubble *>(startItems.first());
    //            CBubble *endItem = qgraphicsitem_cast<CBubble *>(endItems.first());
    //            if(startItem && endItem)
    //            {
    //                Arrow *arrow = new Arrow(startItem, endItem);
    //                arrow->setColor(myLineColor);
    //                startItem->addArrow(arrow);
    //                endItem->addArrow(arrow);
    //                arrow->setZValue(-1000.0);
    //                addItem(arrow);
    //                arrow->updatePosition();
    //            }
    //        }
    //    }

    m_line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);

    m_rubberBand = false;

    emit leftReleased();
}
