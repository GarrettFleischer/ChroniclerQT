#include "cconnection.h"
#include "clink.h"
#include "Bubbles/cbubble.h"

CConnection::CConnection(CLink *from, CBubble *to)
    : m_from(from), m_to(to), m_followMouse(false)
{
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    m_color = Qt::black;
    setPen(QPen(m_color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    
    setAcceptHoverEvents(true);
}

CConnection::CConnection(CLink *from)
    : m_from(from), m_to(0), m_followMouse(true)
{
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    m_color = Qt::black;
    setPen(QPen(m_color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    
    setAcceptHoverEvents(true);
}

CLink *CConnection::from() const
{
    return m_from;
}

void CConnection::setFrom(CLink *from)
{
    m_from = from;
}

CBubble *CConnection::to() const
{
    return m_to;
}

void CConnection::setTo(CBubble *to)
{
    m_to = to;
}

void CConnection::setFollowMouse(bool followMouse)
{
    m_followMouse = followMouse;
}

QColor CConnection::color() const
{
    return m_color;
}

void CConnection::setColor(const QColor &color)
{
    m_color = color;
}



void CConnection::updatePosition()
{
    QPointF point = (m_followMouse ? m_mousePos : m_to->scenePos());
    setLine(QLineF(m_from->scenePos(), point));
}

void CConnection::mousePressEvent(QGraphicsSceneMouseEvent *evt)
{
    QGraphicsLineItem::mousePressEvent(evt);
}

void CConnection::mouseMoveEvent(QGraphicsSceneMouseEvent *evt)
{
    QGraphicsLineItem::mouseMoveEvent(evt);
    if(m_followMouse)
    {
        m_mousePos = evt->scenePos();
        updatePosition();
    }
}
