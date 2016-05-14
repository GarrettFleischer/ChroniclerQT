#include "Bubbles/cbubble.h"

#include <QGraphicsPixmapItem>
#include <QList>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>

#include "Connections/cconnection.h"


CBubble::CBubble(QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent), m_contextMenu(0),
      m_minSize(QSizeF(150,150)), m_order(0), m_locked(false),
      m_font(QFont()), m_palette(CPalette()), m_resize(false)
{}


CBubble::CBubble(QMenu *contextMenu, const QPointF &pos, const Chronicler::CPalette &palette,const QFont &font, QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent), m_contextMenu(contextMenu),
      m_minSize(QSizeF(150,150)), m_order(0), m_locked(false),
      m_font(font), m_palette(palette), m_resize(false)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setAcceptHoverEvents(true);
    
    setCursor(Qt::PointingHandCursor);
    setPos(pos);
}

CBubble::~CBubble()
{
    for(CConnection *connection : m_connections)
        delete connection;
}

void CBubble::mousePressEvent(QGraphicsSceneMouseEvent *evt)
{
    QRectF b = sceneBoundingRect();
    QRectF resizeRect(QPointF(b.x() + b.width() - 20, b.y() + b.height() - 20), QSizeF(20,20));

    if(resizeRect.contains(evt->scenePos()))
    {
        m_resize = true;
        m_offset = evt->scenePos();
        m_lastBounds = boundingRect();
    }
}

void CBubble::mouseMoveEvent(QGraphicsSceneMouseEvent *evt)
{
    if(m_resize)
    {
        QPointF delta(evt->scenePos() - m_offset);
        m_bounds = QRectF(m_lastBounds.x(), m_lastBounds.y(),
                          qMax<float>(m_lastBounds.width() + delta.x(), m_minSize.width()),
                          qMax<float>(m_lastBounds.height() + delta.y(), m_minSize.height()));
        setPolygon(m_bounds);
        UpdatePolygon();
        emit PositionChanged();
    }
    else
    {
        QGraphicsPolygonItem::mouseMoveEvent(evt);
        setCursor(Qt::ClosedHandCursor);
    }
}

void CBubble::mouseReleaseEvent(QGraphicsSceneMouseEvent *evt)
{
    QGraphicsPolygonItem::mouseReleaseEvent(evt);
    setCursor(Qt::PointingHandCursor);
    m_resize = false;
}

void CBubble::hoverMoveEvent(QGraphicsSceneHoverEvent *evt)
{
    QRectF b = sceneBoundingRect();
    QRectF resizeRect(QPointF(b.x() + b.width() - 20, b.y() + b.height() - 20), QSizeF(20,20));

    if(resizeRect.contains(evt->scenePos()))
        setCursor(Qt::SizeFDiagCursor);
    else
        setCursor(Qt::PointingHandCursor);
}


void CBubble::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if(m_contextMenu)
    {
        if(!isSelected())
        {
            scene()->clearSelection();
            setSelected(true);
        }

        m_contextMenu->exec(event->screenPos());
    }
}


QVariant CBubble::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged && value.toBool())
        emit Selected(this);
    else if (change == QGraphicsItem::ItemPositionChange)
        emit PositionChanged();

    return value;
}

void CBubble::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen outline = (isSelected() ? QPen(m_palette.select, 2) : QPen(m_palette.line, 1.5));
    painter->setPen(outline);
    painter->setBrush(QBrush(m_palette.fill));
    painter->drawPolygon(polygon(), Qt::WindingFill);
}

void CBubble::UpdatePolygon()
{
    QPointF padding(10, 10);

    QRectF b(m_bounds.topLeft(), m_bounds.bottomRight() + padding);
    b.setWidth(qMax(b.width(), m_minSize.width()));
    b.setHeight(qMax(b.height(), m_minSize.height()));

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(b, 10, 10);
    path.addRect(QRectF(b.center(), b.bottomRight())); // Bottom right corner not rounded

    setPolygon(path.simplified().toFillPolygon());
}

Anchor CBubble::AnchorAtPosition(const QPointF &pos)
{
    QPointF center = sceneBoundingRect().center();
    qreal diff_x = qAbs(center.x() - pos.x());
    qreal diff_y = qAbs(center.y() - pos.y());

    if(pos.x() < center.x() && diff_x > diff_y)
        return Anchor::Left;
    if(pos.x() > center.x() && diff_x > diff_y)
        return Anchor::Right;
    if(pos.y() > center.y())
        return Anchor::Down;

    return Anchor::Up;
}


void CBubble::setFont(const QFont &font)
{
    if(font != m_font)
    {
        m_font = font;
        update();
    }
}

void CBubble::setPalette(const Chronicler::CPalette &palette)
{
    m_palette = palette;
    update();
}

void CBubble::AddConnection(CConnection *connection)
{
    if(!m_connections.contains(connection))
    {
        connect(this, SIGNAL(PositionChanged()), connection, SLOT(UpdatePosition()));
        m_connections.append(connection);
        emit ConnectionsChanged(m_connections.length());
    }
}

void CBubble::RemoveConnection(CConnection *connection)
{
    m_connections.removeAll(connection);

    emit ConnectionsChanged(m_connections.length());
}

Anchor CBubble::OutputAnchorAtPosition(const QPointF &pos)
{
    return AnchorAtPosition(pos);
}

Anchor CBubble::InputAnchorAtPosition(const QPointF &pos)
{
    return AnchorAtPosition(pos);
}

void CBubble::setAllowedAnchors(int allowed)
{
    m_allowedAnchors = allowed;
}
