#include "cline.h"

#include <QPainter>
#include <QPainterPath>
#include <QPointF>

CLine::CLine(QObject *parent)
    : QObject(parent), QGraphicsItem(), m_path(new QPainterPath()), m_offset(200)
{}

CLine::CLine(const CLine &copy)
    : QObject(copy.parent()), QGraphicsItem(), m_path(new QPainterPath(*copy.m_path)), m_offset(copy.m_offset)
{}

CLine::~CLine()
{
    delete m_path;
}

CLine &CLine::operator=(const CLine &rhs)
{
    setParent(rhs.parent());
    m_path = new QPainterPath(*rhs.m_path);

    return *this;
}

QPainterPath CLine::shape() const
{
    return *m_path;
}

QRectF CLine::boundingRect() const
{
    QRectF bounds(m_path->boundingRect());
    bounds.translate(-2, -2);
    bounds.setSize(QSizeF(bounds.width() + 4, bounds.height() + 4));

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
    painter->setPen(QPen(QBrush(Qt::red), 5));
    painter->drawPath(*m_path);
}

void CLine::UpdateShape()
{
    delete m_path;
    m_path = new QPainterPath();

    QPointF p1(m_start.x(), m_start.y() + m_offset);
    QPointF p2(m_end.x(), m_end.y() - m_offset);

    m_path->moveTo(m_start);
    m_path->cubicTo(p1, p2, m_end);

    prepareGeometryChange();
    update(boundingRect());
}
