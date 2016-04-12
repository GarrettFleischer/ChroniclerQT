#include "cline.h"

#include <QPainter>
#include <QPainterPath>
#include <QPointF>
#include <QtMath>

CLine::CLine(const QPointF &start, const QPointF &end, QObject *parent)
    : QObject(parent), QGraphicsItem(), m_start(start), m_end(end),
      m_startAnchor(DOWN), m_endAnchor(UP), m_offset(200), m_width(5)
{
    UpdateShape();
}

CLine::CLine(const CLine &copy)
    : QObject(copy.parent()), QGraphicsItem(), m_path(copy.m_path), m_offset(copy.m_offset), m_width(copy.m_width)
{
    UpdateShape();
}

CLine &CLine::operator=(const CLine &rhs)
{
    setParent(rhs.parent());

    return *this;
}

QPainterPath CLine::shape() const
{
    return m_path;
}

QRectF CLine::boundingRect() const
{
    QRectF bounds(m_path.boundingRect());

    // add a width/2 buffer to prevent artifacts when moving
    bounds.translate(-m_width / 2, -m_width / 2);
    bounds.setSize(QSizeF(bounds.width() + m_width, bounds.height() + m_width));

    return bounds;
}

void CLine::setStart(const QPointF &start)
{
    m_start = start;
    UpdateShape();
}

void CLine::setEnd(const QPointF &end)
{
    m_end = end;
    UpdateShape();
}

void CLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(QPen(QBrush(Qt::red), m_width));
    painter->drawPath(m_path);
}

void CLine::UpdateShape()
{
    m_path = QPainterPath();

    // TODO: DEAL WITH OFFSETS BASED ON ANCHOR POSITION
    qreal start_angle = (m_startAnchor * M_PI * 0.5);
    qreal end_angle = (m_endAnchor * M_PI * 0.5);

    QPointF p1(m_start.x(), m_start.y());
    QPointF p2(m_end.x(), m_end.y());
    p1.rx() += (qRound(qCos(start_angle)) * m_offset);
    p1.ry() += (qRound(qSin(start_angle)) * m_offset);
    p2.rx() += (qRound(qCos(end_angle)) * m_offset);
    p2.ry() += (qRound(qSin(end_angle)) * m_offset);

    m_path.moveTo(m_start);
    m_path.cubicTo(p1, p2, m_end);

    prepareGeometryChange();
    update();
}
