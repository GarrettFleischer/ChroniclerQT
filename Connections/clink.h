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


class CLink : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT
    Q_PROPERTY(qreal scale MEMBER m_scale NOTIFY scaleChanged)
    
public:
    enum Direction { Up = 0, Left, Down, Right };
    
public:
    CLink(Direction direction = Down, QGraphicsItem *parent = 0);
    
    void setColor(const QColor &color);
    void setLineColor(const QColor &lineColor);
    
    CBubble *parent() const { return m_parent; }
    
    
    const CConnection *connection() const;
    void setConnection(CConnection *connection);
    
protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *);
    
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    
private:
    
    
private:
    bool m_hover;
    QPolygonF m_polygon;
    QColor m_color;
    QColor m_lineColor;
    
    Direction m_direction;
    
    QPropertyAnimation *m_anim;
    qreal m_scale;
    
    CBubble *m_parent;
    CConnection *m_connection;
    
signals:
    void scaleChanged();
    void clicked(CLink *);
    
private slots:
    void UpdateShape();
};

#endif // CLINK_H
