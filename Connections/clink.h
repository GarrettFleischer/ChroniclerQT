#ifndef CLINK_H
#define CLINK_H

#include <QObject>
#include <QGraphicsPolygonItem>
#include <QPainterPath>
#include <QRectF>
#include <QPainter>
#include <QPropertyAnimation>

#include "Bubbles/cbubble.h"
#include "Connections/cconnection.h"

#include "Misc/chronicler.h"
using Chronicler::Anchor;
using Chronicler::CPalette;


class CLink : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT
    Q_PROPERTY(qreal scale MEMBER m_scale NOTIFY scaleChanged)
    
public:
    CLink(Anchor anchor = Anchor::DOWN, QGraphicsItem *parent = 0);
    
    void setPalette(const CPalette &palette);
    
    CBubble *parent() const { return m_parent; }
    
    CConnection *connection();
    void setConnection(CConnection *connection);
    
    Anchor anchor() const;
    void setAnchor(Anchor anchor);

protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *);
    
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

private:
    bool m_hover;
    QPolygonF m_polygon;
    QColor m_color;
    QColor m_lineColor;
    
    Anchor m_anchor;
    
    QPropertyAnimation *m_anim;
    qreal m_scale;
    
    CBubble *m_parent;
    CConnection *m_connection;

    CPalette m_palette;
    
signals:
    void scaleChanged();
    void clicked(CLink *);
    void positionChanged();

private slots:
    void UpdateShape();
};

#endif // CLINK_H
