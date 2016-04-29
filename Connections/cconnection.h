#ifndef CCONNECTION_H
#define CCONNECTION_H

#include <QObject>
#include <QGraphicsItem>

QT_BEGIN_NAMESPACE
class QColor;
class QPen;
QT_END_NAMESPACE

class CBubble;
class CLine;


class CConnection : public QObject, public QGraphicsItem
{
    Q_OBJECT
    
public:
    CConnection(CBubble *from, CBubble *to, QGraphicsScene *scn);
    ~CConnection();
    
    CBubble *from() const;
    void setFrom(CBubble *from);
    
    CBubble *to() const;
    void setTo(CBubble *to);
    
    QColor color() const;
    void setColor(const QColor &color);

    void updatePosition();

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget);
    
private:
    CBubble *m_from;
    CBubble *m_to;
    
    CLine *m_line;

    QColor m_color;
    
signals:
    
public slots:
};

#endif // CCONNECTION_H
