#include "cgraphicsscene.h"
#include "Connections/arrow.h"

#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QWheelEvent>

#include "Bubbles/cstorybubble.h"
#include "Bubbles/cconditionbubble.h"
#include "Bubbles/cchoicebubble.h"
#include "Connections/clink.h"
#include "Connections/cline.h"


CGraphicsScene::CGraphicsScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent), m_itemMenu(itemMenu), m_mode(Cursor), m_line(0), m_rubberBand(false)
{
    float maxsize = 15000.0;//std::numeric_limits<float>::max();
    float minsize = -15000.0/2;//-std::numeric_limits<float>::max()/2;
    setSceneRect(QRectF(minsize, minsize, maxsize, maxsize));
    setBackgroundBrush(QBrush(Qt::gray));//QColor(86,96,123)));
}

//void CGraphicsScene::setLineColor(const QColor &color)
//{
//    m_lineColor = color;
//    foreach (QGraphicsItem *item, selectedItems())
//    {
//        CBubble *bbl = qgraphicsitem_cast<CBubble *>(item);
//        if(bbl)
//            bbl->setLineColor(m_lineColor);
//    }
//}

//void CGraphicsScene::setTextColor(const QColor &color)
//{
//    m_textColor = color;
//    foreach (QGraphicsItem *item, selectedItems())
//    {
//        CBubble *bbl = qgraphicsitem_cast<CBubble *>(item);
//        if(bbl)
//            bbl->setFontColor(m_textColor);
//    }
//}

//void CGraphicsScene::setItemColor(const QColor &color)
//{
//    m_itemColor = color;
//    foreach (QGraphicsItem *item, selectedItems())
//    {
//        CBubble *bbl = qgraphicsitem_cast<CBubble *>(item);
//        if(bbl)
//            bbl->setColor(m_itemColor);
//    }
//}

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

//void CGraphicsScene::linkClicked(CLink *)
//{
//    setMode(InsertLine);
    
//}

void CGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton)
    {
        switch (m_mode) {
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
