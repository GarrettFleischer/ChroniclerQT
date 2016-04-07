#include "clink.h"



CLink::CLink(Direction direction, QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent), m_hover(false), m_direction(direction), m_scale(1),
      m_parent(dynamic_cast<CBubble *>(parent)), m_connection(NULL)
{
    UpdateShape();
    setAcceptHoverEvents(true);
    
    setFlag(QGraphicsItem::ItemIsSelectable);
    
    connect(this, SIGNAL(scaleChanged()), this, SLOT(UpdateShape()));
    m_anim = new QPropertyAnimation(this, "scale", this);
    m_anim->setDuration(50);
}

void CLink::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    m_anim->stop();
    m_anim->setEndValue(1.2);
    m_anim->start();
    
    m_hover = true;
}

void CLink::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    m_anim->stop();
    m_anim->setEndValue(1);
    m_anim->start();
    
    m_hover = false;
}

void CLink::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    emit clicked(this);
}

void CLink::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
//    QPen outline = (isSelected() ? QPen(QColor(255,200,0), 2) : QPen(m_lineColor, 1));
    painter->setPen(QPen(m_lineColor, 1));
    painter->setBrush(QBrush(m_color));
    painter->drawPolygon(m_polygon, Qt::WindingFill);
}

const CConnection *CLink::connection() const
{
    return m_connection;
}

void CLink::setLineColor(const QColor &lineColor)
{
    m_lineColor = lineColor;
    update();
}

void CLink::setConnection(CConnection *connection)
{
    delete m_connection;
    m_connection = connection;
}

void CLink::setColor(const QColor &color)
{
    m_color = color;
    update();
}

void CLink::UpdateShape()
{
    int vert = (m_direction == Up || m_direction == Down) * 10;
    int hor = (!vert) * 10;
    int down = (m_direction == Down) * 10;
    int right = (m_direction == Right) * 10;
    QRectF b = QRectF((right - 10) * m_scale, (down - 10) * m_scale, (20 - hor) * m_scale, (20 - vert) * m_scale);
    
    QPainterPath path;
    switch(m_direction)
    {
    case Up:
        path.moveTo(b.x(), b.y() + b.height());
        path.lineTo(b.x()+ b.width(), b.y() + b.height());
        break;
        
    case Left:
        path.moveTo(b.x() + b.width(), b.y() + b.height());
        path.lineTo(b.x() + b.width(), b.y());
        break;
        
    case Down:
        path.moveTo(b.x() + b.width(), b.y());
        path.lineTo(b.x(), b.y());
        break;
        
    case Right:
        path.moveTo(b.x(), b.y());
        path.lineTo(b.x(), b.y()+ b.height());
        break;
    }
    
    path.arcTo(b, 90 * int(m_direction), 180);
    
    m_polygon = path.toFillPolygon();
    setPolygon(m_polygon);
}
