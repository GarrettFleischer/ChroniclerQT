#include "cline.h"

#include <QPainter>
#include <QPainterPath>
#include <QPointF>
#include <QtMath>
#include <QTransform>

CLine::CLine(const QPointF &start, const QPointF &end, Anchor anc_start, Anchor anc_end, QObject *parent)
    : QObject(parent), QGraphicsItem(), m_start(start), m_end(end),
      m_startAnchor(anc_start), m_endAnchor(anc_end), m_offset(200), m_width(5)
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

void CLine::setStart(const QPointF &start, Anchor anchor)
{
    m_start = start;
    m_startAnchor = anchor;
    UpdateShape();
}

void CLine::setEnd(const QPointF &end, Chronicler::Anchor anchor)
{
    m_end = end;
    m_endAnchor = anchor;
    UpdateShape();
}

void CLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(QPen(QBrush(Qt::black), m_width));
    painter->drawPath(m_path);

    painter->setPen(QPen(QBrush(Qt::black), 1));
    painter->setBrush(QBrush(Qt::black));
    painter->drawPath(m_arrow);
}

void CLine::UpdateShape()
{
    m_path = QPainterPath();

    // TODO: DEAL WITH OFFSETS BASED ON ANCHOR POSITION
    qreal start_angle = (qreal(m_startAnchor) * M_PI * 0.5);
    qreal end_angle = (qreal(m_endAnchor) * M_PI * 0.5);

    QPointF p1(m_start.x(), m_start.y());
    QPointF p2(m_end.x(), m_end.y());
    p1.rx() += (qRound(qCos(start_angle)) * m_offset);
    p1.ry() += (qRound(qSin(start_angle)) * m_offset);
    p2.rx() += (qRound(qCos(end_angle)) * m_offset);
    p2.ry() += (qRound(qSin(end_angle)) * m_offset);

    m_path.moveTo(m_start);
    m_path.cubicTo(p1, p2, m_end);

    // create and rotate arrow
    const int size = 15;
    QRectF box(-size * 0.5, -size * 0.5, size, size);
    m_arrow = QPainterPath();
    QPointF arrow_start(box.left(), box.center().y());
    m_arrow.moveTo(arrow_start);
    m_arrow.lineTo(box.topRight());
    m_arrow.lineTo(box.bottomRight());
    m_arrow.lineTo(arrow_start);
    QTransform t;
    t.rotateRadians(end_angle);
    m_arrow = t.map(m_arrow);
    qreal off_x = qCos(end_angle) * size * 0.5;
    qreal off_y = qSin(end_angle) * size * 0.5;
    m_arrow.translate(m_end.x() + off_x, m_end.y() + off_y);

    m_path.addPath(m_arrow);

    prepareGeometryChange();
    update();
}

Anchor CLine::endAnchor() const
{
    return m_endAnchor;
}

void CLine::setEndAnchor(const Anchor &endAnchor)
{
    m_endAnchor = endAnchor;
    UpdateShape();
}

Anchor CLine::startAnchor() const
{
    return m_startAnchor;
}

void CLine::setStartAnchor(const Anchor &startAnchor)
{
    m_startAnchor = startAnchor;
    UpdateShape();
}
