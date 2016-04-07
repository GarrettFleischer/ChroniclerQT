#ifndef CCONNECTION_H
#define CCONNECTION_H

#include <QObject>
#include <QGraphicsLineItem>
#include <QColor>
#include <QPen>

class CLink;
class CBubble;

class CConnection : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
    
public:
    CConnection(CLink *from, CBubble *to);
    CConnection(CLink *from);
    
    CLink *from() const;
    void setFrom(CLink *from);
    
    CBubble *to() const;
    void setTo(CBubble *to);
    
    void setFollowMouse(bool followMouse);
    
    QColor color() const;
    void setColor(const QColor &color);
    
public:
    void updatePosition();
    
    
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *evt);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *evt);
    
    
private:
    CLink *m_from;
    CBubble *m_to;
    
    bool m_followMouse;
    QPointF m_mousePos;
    
    QColor m_color;
    
    
signals:
    
public slots:
};

#endif // CCONNECTION_H
