#ifndef CCONNECTION_H
#define CCONNECTION_H

#include <QObject>
#include <QGraphicsItem>

QT_BEGIN_NAMESPACE
class QColor;
class QPainterPath;
QT_END_NAMESPACE

class CBubble;
class CLine;


class CConnection : public QObject, public QGraphicsItem
{
    Q_OBJECT
    
public:
    enum Anchor { RIGHT, DOWN, LEFT, UP };

    CConnection(CBubble *from, CBubble *to);
    virtual ~CConnection();
    
    CBubble *from() const;
    void setFrom(CBubble *from);
    
    CBubble *to() const;
    void setTo(CBubble *to);
    
    QColor color() const;
    void setColor(const QColor &color);

    virtual QPainterPath shape() const;
    virtual QRectF boundingRect() const;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    
private:
    void UpdateShape();

    CBubble *m_from;
    CBubble *m_to;
    QPainterPath m_path;

    Anchor m_startAnchor;
    Anchor m_endAnchor;

    QColor m_color;
    float m_width;
    int m_offset;
    
signals:
    
public slots:
    void UpdatePosition();


};

#endif // CCONNECTION_H
