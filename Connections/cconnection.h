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
using Chronicler::t_uid;

class CBubble;
class CLine;


class CConnection : public QObject, public QGraphicsItem
{
    Q_OBJECT
    
public:
    CConnection(QGraphicsScene *scn);
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

    Anchor startAnchor() const;
    void setStartAnchor(Anchor anchor);
    Anchor endAnchor() const;
    void setEndAnchor(Anchor anchor);

    void ConnectToUIDs();

    QDataStream &Read(QDataStream &ds, const QString &);
    QDataStream &Write(QDataStream &ds);

private:
    CBubble *m_from;
    CBubble *m_to;

    CLine *m_line;

    t_uid m_fromUID;
    t_uid m_toUID;
    
signals:
    
public slots:
    void UpdatePosition();


};

#endif // CCONNECTION_H
