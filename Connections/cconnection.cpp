#include "cconnection.h"

#include <QGraphicsScene>

#include "cline.h"
#include "Bubbles/cbubble.h"

CConnection::CConnection(CBubble *from, CBubble *to, QGraphicsScene *scn)
    : m_from(from), m_to(to)
{
    m_line = new CLine(QPointF(), QPointF());
    scn->addItem(m_line);

    m_from->AddLink(this, 0);
    m_to->AddConnection(this);
    updatePosition();
}

CConnection::~CConnection()
{
    if(m_from)
        m_from->RemoveLink(this);
    if(m_to)
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
    if(m_from)
        m_line->setStart(m_from->scenePos());
    if(m_to)
        m_line->setEnd(m_to->scenePos());
}

QRectF CConnection::boundingRect() const
{
    if(m_line)
        return m_line->boundingRect();

    return QRectF();
}

void CConnection::paint(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget)
{
    //m_line->paint(painter, style, widget);
}
