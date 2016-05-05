#ifndef CCONNECTION_H
#define CCONNECTION_H

#include <QObject>
#include <QGraphicsItem>

QT_BEGIN_NAMESPACE
class QColor;
class QPainterPath;
class QGraphicsScene;
QT_END_NAMESPACE

#include "Misc/chronicler.h"
using Chronicler::Anchor;

class CBubble;
class CLine;


class CConnection : public QObject, public QGraphicsItem
{
    Q_OBJECT
    
public:
    CConnection(CBubble *from, CBubble *to, Anchor anc_from, Anchor anc_to, QGraphicsScene *scn);
    virtual ~CConnection();
    
    CBubble *from() const;
    void setFrom(CBubble *from);
    
    CBubble *to() const;
    void setTo(CBubble *to);
    
    QColor color() const;
    void setColor(const QColor &color);

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    
private:
    CBubble *m_from;
    CBubble *m_to;

    CLine *m_line;
    
signals:
    
public slots:
    void UpdatePosition();


};

#endif // CCONNECTION_H
