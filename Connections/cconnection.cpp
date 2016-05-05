#include "cconnection.h"

#include <QPointF>

#include "cline.h"
#include "Bubbles/cbubble.h"

CConnection::CConnection(CBubble *from, CBubble *to)
    : m_from(0), m_to(0)
{
    setZValue(-999999);

    setFrom(from);
    setTo(to);
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


void CConnection::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(QPen(QBrush(Qt::black), m_width));
    painter->drawPath(m_path);
}

void CConnection::UpdateShape()
{
    m_path = QPainterPath();

    QPointF start;
    QPointF end;

    // TODO: DEAL WITH OFFSETS BASED ON ANCHOR POSITION
    qreal start_angle = (startAnchor * M_PI * 0.5);
    qreal end_angle = (endAnchor * M_PI * 0.5);

    QPointF p1(start.x(), start.y());
    QPointF p2(end.x(), end.y());
    p1.rx() += (qRound(qCos(start_angle)) * m_offset);
    p1.ry() += (qRound(qSin(start_angle)) * m_offset);
    p2.rx() += (qRound(qCos(end_angle)) * m_offset);
    p2.ry() += (qRound(qSin(end_angle)) * m_offset);

    m_path.moveTo(start);
    m_path.cubicTo(p1, p2, end);

    prepareGeometryChange();
    update();
}
