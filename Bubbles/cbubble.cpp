#include "Bubbles/cbubble.h"

#include <QGraphicsPixmapItem>
#include <QList>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>




CBubble::CBubble(QMenu *contextMenu, const QPointF &pos, const Chronicler::CPalette &palette, const QFont &font,
                 QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent), m_contextMenu(contextMenu),
      m_minSize(QSizeF(150,150)), m_order(0), m_locked(false),
      m_font(font), m_palette(palette)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    
    setCursor(Qt::PointingHandCursor);
    setPos(pos);
}

void CBubble::setLabel(QString label)
{
    m_label = label;
}


void CBubble::mouseMoveEvent(QGraphicsSceneMouseEvent *evt)
{
    QGraphicsPolygonItem::mouseMoveEvent(evt);
    setCursor(Qt::ClosedHandCursor);
}

void CBubble::mouseReleaseEvent(QGraphicsSceneMouseEvent *evt)
{
    QGraphicsPolygonItem::mouseReleaseEvent(evt);
    setCursor(Qt::PointingHandCursor);
}


//void CBubble::removeArrow(Arrow *arrow)
//{
//    int index = m_links.indexOf(arrow);

//    if (index != -1)
//        m_links.removeAt(index);
//}


//void CBubble::removeArrows()
//{
//    foreach (Arrow *arrow, m_links) {
//        arrow->startItem()->removeArrow(arrow);
//        arrow->endItem()->removeArrow(arrow);
//        scene()->removeItem(arrow);
//        delete arrow;
//    }
//}


//void CBubble::addArrow(Arrow *arrow)
//{
//    m_links.append(arrow);
//}


void CBubble::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    //scene()->clearSelection();
    //setSelected(true);
    m_contextMenu->exec(event->screenPos());
}


QVariant CBubble::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged && value.toBool())
        emit Selected(this);
    else if (change == QGraphicsItem::ItemPositionChange)
    {
//        foreach (Arrow *arrow, m_links)
//            arrow->updatePosition();
    }

    return value;
}

void CBubble::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //QColor(255,200,0) - select
    QPen outline = (isSelected() ? QPen(m_palette.select, 2) : QPen(m_palette.line, 1.5));
    painter->setPen(outline);
    painter->setBrush(QBrush(m_palette.fill));
    painter->drawPolygon(m_polygon, Qt::WindingFill);
}


void CBubble::setFont(const QFont &font)
{
    if(font != m_font)
    {
        m_font = font;
        update();
    }
}

void CBubble::setPalette(const Chronicler::CPalette &palette)
{
    m_palette = palette;
    update();
}

//void CBubble::setFontColor(const QColor &color)
//{
//    if(color != m_fontColor)
//    {
//        m_fontColor = color;
//        update();
//    }
//}

//void CBubble::setColor(const QColor &color)
//{
//    if(color != m_color)
//    {
//        m_color = color;
//        update();
//    }
//}

//void CBubble::setLineColor(const QColor &color)
//{
//    if(color != m_lineColor)
//    {
//        m_lineColor = color;
//        update();
//    }
//}
