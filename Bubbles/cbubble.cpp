#include "Bubbles/cbubble.h"

#include <stdexcept>
using std::logic_error;

#include <QGraphicsPixmapItem>
#include <QList>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QDataStream>
#include <QByteArray>
#include <QFont>
#include <QSizeF>

#include "Connections/cconnection.h"
#include "cgraphicsscene.h"
#include "Misc/Palette/cpaletteaction.h"
#include "Misc/Palette/cpalettebutton.h"

#include "Misc/chronicler.h"
using Chronicler::shared;


QList<t_uid> CBubble::m_UIDs = QList<t_uid>();

CBubble::CBubble(const QPointF &pos, CPaletteAction *palette, const QFont &font, QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent),
      m_minSize(QSizeF(150, 100)), m_order(0), m_locked(false),
      m_font(font), m_palette(palette), m_resize(false)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setAcceptHoverEvents(true);
    
    setCursor(Qt::PointingHandCursor);
    setPos(pos);

    setPalette(palette);
}

CBubble::~CBubble()
{
    CGraphicsScene *scn = dynamic_cast<CGraphicsScene *>(scene());
    if(scn)
        scn->RemoveBubble(this);

    // make a copy to prevent invalid iterator when connections are removed.
    QList<CConnection *> tmp = m_connections;
    for(CConnection *connection : tmp)
    {
        scn->RemoveConnection(connection);
        delete connection;
    }
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
        QRectF oldBounds = m_bounds;
        QPointF delta(evt->scenePos() - m_offset);
        m_bounds = QRectF(m_lastBounds.x(), m_lastBounds.y(),
                          qMax<float>(m_lastBounds.width() + delta.x(), m_minSize.width()),
                          qMax<float>(m_lastBounds.height() + delta.y(), m_minSize.height()));
        setPolygon(m_bounds);
        UpdatePolygon();
        emit ShapeChanged(oldBounds, m_bounds);
        emit PositionOrShapeChanged();
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

QVariant CBubble::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged && value.toBool())
        emit Selected(this);
    else if(change == QGraphicsItem::ItemPositionChange)
        m_oldPos = pos();
    else if (change == QGraphicsItem::ItemPositionHasChanged)
    {
        emit PositionChanged(m_oldPos, pos());
        emit PositionOrShapeChanged();
    }

    return value;
}

void CBubble::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen outline = (isSelected() ? QPen(m_palette->getPalette().select, 2) : QPen(m_palette->getPalette().line, 1.5));
    painter->setPen(outline);
    painter->setBrush(QBrush(m_palette->getPalette().fill));
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

void CBubble::AddUID(t_uid uid)
{
    if(uid == 0 || m_UIDs.contains(uid))
        throw logic_error("Error! UID must be unique.");
    else
        m_UIDs.append(uid);
}

void CBubble::UpdateUID()
{
    m_UID = GenerateUID();
}

t_uid CBubble::GenerateUID()
{
    return reinterpret_cast<t_uid>(this);
}

Anchor CBubble::AnchorAtPosition(const QPointF &pos)
{
    QPointF center = sceneBoundingRect().center();
    qreal diff_x = qAbs(center.x() - pos.x());
    qreal diff_y = qAbs(center.y() - pos.y());

    if(pos.x() < center.x() && diff_x > diff_y)
        return Anchor::WestAnchor;
    if(pos.x() > center.x() && diff_x > diff_y)
        return Anchor::EastAnchor;
    if(pos.y() > center.y())
        return Anchor::SouthAnchor;

    return Anchor::NorthAnchor;
}

void CBubble::UpdatePalette()
{
    this->setPalette(m_palette);
}


void CBubble::setFont(const QFont &font)
{
    if(font != m_font)
    {
        m_font = font;
        update();
    }
}

void CBubble::setPalette(CPaletteAction *palette)
{
    disconnect(m_palette, SIGNAL(changed()), this, SLOT(UpdatePalette()));
    m_palette = palette;
    connect(m_palette, SIGNAL(changed()), this, SLOT(UpdatePalette()));
    update();

    emit PaletteChanged();
}

CPaletteAction * CBubble::getPalette()
{
    return m_palette;
}

void CBubble::setBounds(const QRectF &bounds)
{
    m_bounds = bounds;
    UpdatePolygon();

    emit PositionOrShapeChanged();
}

CBubble *CBubble::container()
{
    return this;
}

void CBubble::AddConnection(CConnection *connection)
{
    if(!m_connections.contains(connection))
    {
        connect(this, SIGNAL(PositionOrShapeChanged()), connection, SLOT(UpdatePosition()));
        m_connections.append(connection);
        emit ConnectionsChanged(m_connections.length());
    }
}

void CBubble::RemoveConnection(CConnection *connection)
{
    disconnect(this, SIGNAL(PositionOrShapeChanged()), connection, SLOT(UpdatePosition()));
    m_connections.removeAll(connection);
    emit ConnectionsChanged(m_connections.length());
}

QList<CConnection *> CBubble::connections()
{
    return m_connections;
}

Anchor CBubble::OutputAnchorAtPosition(const QPointF &pos)
{
    return AnchorAtPosition(pos);
}

Anchor CBubble::InputAnchorAtPosition(const QPointF &pos)
{
    return AnchorAtPosition(pos);
}

t_uid CBubble::getUID()
{
    return m_UID;
}

QDataStream &CBubble::Deserialize(QDataStream &ds, const Chronicler::CVersion &version)
{
    QPointF pos;

    if(version == "0.8.1.0")
    {
        CPalette palette;

        ds >> m_UID
                >> m_label >> m_order >> m_locked
                >> palette
                >> m_bounds >> pos;

        // hopefully fixes corrupted projects...
        if(m_UIDs.contains(m_UID))
            m_UID = GenerateUID();

        AddUID(m_UID);
    }
    else
    {
        t_uid palette_uid;

        ds >> m_UID
                >> m_label >> m_order >> m_locked
                >> palette_uid
                >> m_bounds >> pos;

        m_palette = shared().paletteButton->getPaletteWithUID(palette_uid);
    }

    setLabel(m_label);
    setPos(pos);
    UpdatePolygon();

    return ds;
}

QDataStream & CBubble::Serialize(QDataStream &ds)
{
    ds << static_cast<qint32>(m_type)
       << GenerateUID()
       << m_label << m_order << m_locked
       << m_palette->getUID()
       << m_bounds << scenePos();

    return ds;
}
