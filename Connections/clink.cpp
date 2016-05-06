#include "clink.h"

#include <QTransform>
#include <QtMath>

CLink::CLink(Anchor anchor, QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent), m_hover(false), m_scale(1),
      m_parent(dynamic_cast<CBubble *>(parent)), m_connection(0)
{
    setAnchor(anchor);
    setAcceptHoverEvents(true);
    
    setFlag(QGraphicsItem::ItemIsSelectable);
    
    connect(this, SIGNAL(scaleChanged()), this, SLOT(UpdateShape()));
    m_anim = new QPropertyAnimation(this, "scale", this);
    m_anim->setDuration(50);
}

void CLink::setPalette(const CPalette &palette)
{
    m_palette = palette;
    update();
}

void CLink::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    m_anim->stop();
    m_anim->setEndValue(1.2);
    m_anim->start();
}

void CLink::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    m_anim->stop();
    m_anim->setEndValue(1);
    m_anim->start();
}

void CLink::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    emit clicked(this);
}

QVariant CLink::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange)
        emit positionChanged();

    return value;
}

void CLink::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen outline = (isSelected() ? QPen(m_palette.select, 2) : QPen(m_palette.line, 1));
    painter->setPen(outline);
    painter->setBrush(QBrush(m_palette.fill));
    painter->drawPolygon(m_polygon, Qt::WindingFill);
}

Anchor CLink::anchor() const
{
    return m_anchor;
}

void CLink::setAnchor(Chronicler::Anchor anchor)
{
    m_anchor = anchor;
    setPos(m_parent->boundingRect().width() * 0.5 * qCos(90 * m_anchor),
           m_parent->boundingRect().height() * 0.5 * qSin(90 * m_anchor));
    UpdateShape();
}

CConnection *CLink::connection()
{
    return m_connection;
}

void CLink::setConnection(CConnection *connection)
{
    delete m_connection;
    m_connection = connection;
    if(m_connection)
    {
        m_connection->setStartAnchor(m_anchor);
        connect(this, SIGNAL(positionChanged()), m_connection, SLOT(UpdatePosition()));
    }
}

void CLink::UpdateShape()
{
    const int size = 20;
    QRectF b = QRectF(0, -size * m_scale * 0.5, size * m_scale * 0.7, size * m_scale);

    QPainterPath path;
    path.moveTo(b.topLeft());
    path.lineTo(b.bottomLeft());
    path.arcTo(b, 270, 180);

    QTransform t;
    t.rotate(90 * m_anchor);
    path = t.map(path);
    
    m_polygon = path.toFillPolygon();
    setPolygon(m_polygon);
}
