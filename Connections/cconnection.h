#ifndef CCONNECTION_H
#define CCONNECTION_H

#include <QObject>
#include <QGraphicsItem>

QT_BEGIN_NAMESPACE
class QColor;
class QPainterPath;
class QGraphicsScene;
QT_END_NAMESPACE

#include "Misc/cserializable.h"

#include "Misc/chronicler.h"
using Chronicler::Anchor;
using Chronicler::t_uid;

class CBubble;
class CLine;
class CPaletteAction;

class CConnection : public QObject, public QGraphicsItem, public CSerializable
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
    
public:
    CConnection(QGraphicsScene *scn);
    CConnection(CBubble *from, CBubble *to, Anchor anc_from, Anchor anc_to, QGraphicsScene *scn);
    virtual ~CConnection();
    
    CBubble *from() const;
    void setFrom(CBubble *from);
    
    CBubble *to() const;
    void setTo(CBubble *to);
    
    CPaletteAction *getPalette();
    void setPalette(CPaletteAction *palette);

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    Anchor startAnchor() const;
    void setStartAnchor(Anchor anchor);
    Anchor endAnchor() const;
    void setEndAnchor(Anchor anchor);

    void ConnectToUIDs(bool paste = false);

    bool isConnected() const;

    CLine *getLine() const;

protected:
    virtual QDataStream &Deserialize(QDataStream &stream, const CVersion &version) override;
    virtual QDataStream &Serialize(QDataStream &stream) override;

private:
    CBubble *m_from;
    CBubble *m_to;

    CLine *m_line;

    t_uid m_fromUID;
    t_uid m_toUID;

    bool m_connected;
    
signals:
    
public slots:
    void UpdatePosition();

private slots:
    void FromPaletteChanged();


};

#endif // CCONNECTION_H
