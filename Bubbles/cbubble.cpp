#include "Bubbles/cbubble.h"

#include "Connections/arrow.h"



CBubble::CBubble(QMenu *contextMenu, QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent), m_contextMenu(contextMenu),
      m_minSize(QSizeF(150,150)), m_order(0), m_locked(false),
      m_fontColor(Qt::black), m_color(Qt::white), m_lineColor(Qt::black)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    
    setCursor(Qt::PointingHandCursor);
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


void CBubble::removeArrow(Arrow *arrow)
{
    int index = m_links.indexOf(arrow);

    if (index != -1)
        m_links.removeAt(index);
}


void CBubble::removeArrows()
{
    foreach (Arrow *arrow, m_links) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}


void CBubble::addArrow(Arrow *arrow)
{
    m_links.append(arrow);
}


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
        foreach (Arrow *arrow, m_links)
            arrow->updatePosition();
    }

    return value;
}

void CBubble::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen outline = (isSelected() ? QPen(QColor(255,200,0), 2) : QPen(m_lineColor, 1.5));
    painter->setPen(outline);
    painter->setBrush(QBrush(m_color));
    painter->drawPolygon(m_polygon, Qt::WindingFill);
}


void CBubble::SetFont(const QFont &font)
{
    if(font != m_font)
    {
        m_font = font;
        update();
    }
}

void CBubble::SetFontColor(const QColor &color)
{
    if(color != m_fontColor)
    {
        m_fontColor = color;
        update();
    }
}

void CBubble::SetColor(const QColor &color)
{
    if(color != m_color)
    {
        m_color = color;
        update();
    }
}

void CBubble::SetLineColor(const QColor &color)
{
    if(color != m_lineColor)
    {
        m_lineColor = color;
        update();
    }
}
