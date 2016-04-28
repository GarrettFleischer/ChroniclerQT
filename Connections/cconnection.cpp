#include "cconnection.h"
#include "cline.h"
#include "Bubbles/cbubble.h"

CConnection::CConnection(CBubble *from, CBubble *to)
    : m_from(from), m_to(to), m_line(0)
{}

CConnection::~CConnection()
{
    m_from->RemoveLink(this);
    m_to->RemoveConnection(this);
}

CBubble *CConnection::from() const
{
    return m_from;
}

void CConnection::setFrom(CBubble *from)
{
    m_from = from;
    updatePosition();
}

CBubble *CConnection::to() const
{
    return m_to;
}

void CConnection::setTo(CBubble *to)
{
    m_to = to;
    updatePosition();
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
    if(!m_line)
        m_line = new CLine(QPointF(), QPointF(), this);

    if(m_from)
        m_line->setStart(m_from->scenePos());
    if(m_to)
        m_line->setStart(m_to->scenePos());
}

QRectF CConnection::boundingRect() const
{
    if(m_line)
        return m_line->boundingRect();

    return QRectF();
}

void CConnection::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{}
