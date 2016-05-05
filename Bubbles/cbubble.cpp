#include "Bubbles/cbubble.h"

#include <QGraphicsPixmapItem>
#include <QList>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>

#include "Connections/cconnection.h"


CBubble::CBubble(QMenu *contextMenu, const QPointF &pos, const Chronicler::CPalette &palette,const QFont &font, QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent), m_contextMenu(contextMenu),
      m_minSize(QSizeF(150,150)), m_order(0), m_locked(false),
      m_font(font), m_palette(palette)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    
    setCursor(Qt::PointingHandCursor);
    setPos(pos);
}

CBubble::~CBubble()
{
    for(CConnection *connection : m_connections)
        delete connection;
}


void CBubble::mouseMoveEvent(QGraphicsSceneMouseEvent *evt)
{
    QGraphicsPolygonItem::mouseMoveEvent(evt);
    setCursor(Qt::ClosedHandCursor);
}

void CBubble::mouseReleaseEvent(QGraphicsSceneMouseEvent *evt)
{
    QGraphicsPolygonItem::mouseReleaseEvent(evt);
    setCursor(Qt::PointingHandCursor);
}


void CBubble::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if(!isSelected())
    {
        scene()->clearSelection();
        setSelected(true);
    }

    m_contextMenu->exec(event->screenPos());
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
