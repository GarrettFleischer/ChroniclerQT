#include "cconnection.h"

#include <QGraphicsScene>

#include "cline.h"
#include "Bubbles/cbubble.h"

CConnection::CConnection(CBubble *from, CBubble *to, QGraphicsScene *scn)
    : m_from(0), m_to(0)
{
    m_line = new CLine(QPointF(), QPointF());
    m_line->setZValue(-999999);
    scn->addItem(m_line);

    setFrom(from);
    setTo(to);

    UpdatePosition();
}

CConnection::~CConnection()
{
    setFrom(0);
    setTo(0);

    delete m_line;
}

CBubble *CConnection::from() const
{
    return m_from;
}

void CConnection::setFrom(CBubble *from)
{
    if(m_from)
    {
        disconnect(m_from, SIGNAL(PositionChanged()), this, SLOT(UpdatePosition()));
        m_from->RemoveLink(this);
    }

    m_from = from;
    if(m_from)
    {
        m_from->AddLink(this);
        connect(m_from, SIGNAL(PositionChanged()), this, SLOT(UpdatePosition()));
    }

    UpdatePosition();
}

CBubble *CConnection::to() const
{
    return m_to;
}

void CConnection::setTo(CBubble *to)
{
    if(m_to)
    {
        disconnect(m_to, SIGNAL(PositionChanged()), this, SLOT(UpdatePosition()));
        m_to->RemoveConnection(this);
    }

    m_to = to;
    if(m_to)
    {
        m_to->AddConnection(this);
        connect(m_to, SIGNAL(PositionChanged()), this, SLOT(UpdatePosition()));
    }

    UpdatePosition();
}

QColor CConnection::color() const
{
    return m_color;
}

void CConnection::setColor(const QColor &color)
{
    m_color = color;
}

void CConnection::UpdatePosition()
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

void CConnection::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{}
