#ifndef CBUBBLE_H
#define CBUBBLE_H


#include <QObject>
#include <QGraphicsPolygonItem>

#include <QFont>

QT_BEGIN_NAMESPACE
class QGraphicsItem;
class QGraphicsSceneMouseEvent;
class QStyleOptionGraphicsItem;
class QPolygonF;
QT_END_NAMESPACE

#include "Misc/cserializable.h"

#include "Misc/chronicler.h"
using Chronicler::CPalette;
using Chronicler::Anchor;
using Chronicler::BubbleType;
using Chronicler::t_uid;

class CPaletteAction;
class CConnection;
class CLink;


class CBubble : public QObject, public QGraphicsPolygonItem, public CSerializable
{
    Q_OBJECT

public:
    CBubble(t_uid uid, const QPointF &pos, CPaletteAction *palette, const QFont &font = QFont(), QGraphicsItem *parent = 0);

    virtual ~CBubble();

    virtual void setLabel(QString label) { m_label = label; }
    QString getLabel() const { return m_label; }

    void setOrder(qint64 order) { m_order = order; }
    qint64 getOrder() const { return m_order; }

    void setLocked(bool locked) { m_locked = locked; }
    bool getLocked() const { return m_locked; }

    virtual void setFont(const QFont &font);
    QFont getFont() const { return m_font; }

    virtual void setPalette(CPaletteAction *palette);
    CPaletteAction *getPalette();

    BubbleType getType() const { return m_type; }

    static t_uid GenerateUID();

    void setBounds(const QRectF &bounds);

    // if this bubble is contained inside another bubble, return that
    // else return itself
    virtual CBubble * container();

    void AddConnection(CConnection *connection);
    void RemoveConnection(CConnection *connection);
    QList<CConnection *> connections();

    virtual void AddLink(CConnection *link) = 0;
    virtual void RemoveLink(CConnection *link) = 0;
    virtual void RemoveLink(Anchor anchor) = 0;
    virtual QList<CConnection *> links() = 0;

    virtual Anchor OutputAnchorAtPosition(const QPointF &pos);
    virtual Anchor InputAnchorAtPosition(const QPointF &pos);

    t_uid UID();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *evt) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *evt) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *evt) override;

    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *evt) override;

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    virtual void UpdatePolygon();

    virtual QDataStream &Read(QDataStream &ds, const QString &version) override;
    virtual QDataStream &Write(QDataStream &ds) override;

    static void AddUID(Chronicler::t_uid uid);

    t_uid m_UID;

    BubbleType m_type;

    QRectF m_bounds;
    QSizeF m_minSize;

    QList<CConnection *> m_connections;

    QString m_label;
    qint64 m_order;
    bool m_locked;

    QFont m_font;
    CPaletteAction *m_palette;

    bool m_resize;
    QPointF m_offset;
    QRectF m_lastBounds;

private:
    Anchor AnchorAtPosition(const QPointF &pos);
    static QList<t_uid> m_UIDs;
    
signals:
    void Selected(QGraphicsItem *item);
    void ConnectionsChanged(int);
    void PositionOrShapeChanged();

    void PaletteChanged();

private slots:
    void UpdatePalette();

};

#endif // CBUBBLE_H
