#include "cconnection.h"

#include <QPointF>
#include <QGraphicsScene>
#include <QtMath>

#include "cline.h"
#include "Bubbles/cbubble.h"

#include "Properties/cprojectview.h"
//#include "cgraphicsscene.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

CConnection::CConnection(QGraphicsScene *scn)
    : CConnection(0, 0, Anchor::Down, Anchor::Up, scn)
{}

CConnection::CConnection(CBubble *from, CBubble *to, Anchor anc_from, Anchor anc_to, QGraphicsScene *scn)
    : m_from(0), m_to(0), m_fromUID(-1), m_toUID(-1)
{
    m_line = new CLine(QPointF(), QPointF(), anc_from, anc_to);
    m_line->setZValue(-999999);
    scn->addItem(m_line);

    setFrom(from);
    setTo(to);
}

CConnection::~CConnection()
{
//    dynamic_cast<CGraphicsScene *>(scene())->RemoveConnection(this);

    setFrom(0);
    setTo(0);

    delete m_line;
}

CBubble *CConnection::from() const
{
    return m_from;
}

void CConnection::setFrom(CBubble *from)
{
    if(m_from)
    {
        disconnect(m_from, SIGNAL(PositionChanged()), this, SLOT(UpdatePosition()));
        m_from->RemoveLink(this);
    }

    m_from = from;
    if(m_from)
    {
        m_from->AddLink(this);
        connect(m_from, SIGNAL(PositionChanged()), this, SLOT(UpdatePosition()));

        m_line->setPalette(m_from->getPalette());
    }

    UpdatePosition();
}

CBubble *CConnection::to() const
{
    return m_to;
}

void CConnection::setTo(CBubble *to)
{
    if(m_to)
    {
        disconnect(m_to, SIGNAL(PositionChanged()), this, SLOT(UpdatePosition()));
        m_to->RemoveConnection(this);
    }

    m_to = to;
    if(m_to)
    {
        m_to->AddConnection(this);
        connect(m_to, SIGNAL(PositionChanged()), this, SLOT(UpdatePosition()));
    }

    UpdatePosition();
}

void CConnection::UpdatePosition()
{
    if(m_from)
    {
        qreal angle = (startAnchor() * M_PI * 0.5);
        QPointF pos = m_from->sceneBoundingRect().center();
        QSizeF size = m_from->sceneBoundingRect().size();
        m_line->setStart(pos + QPointF(size.width() * 0.5 * qCos(angle),
                                       size.height() * 0.5 * qSin(angle)));
    }
    if(m_to)
    {
        qreal angle = (endAnchor() * M_PI * 0.5);
        QPointF pos = m_to->sceneBoundingRect().center();
        QSizeF size = m_to->sceneBoundingRect().size();
        m_line->setEnd(pos + QPointF(size.width() * 0.5 * qCos(angle),
                                     size.height() * 0.5 * qSin(angle)));
    }
}

QRectF CConnection::boundingRect() const
{
    return m_line->boundingRect();
}

void CConnection::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{}

Anchor CConnection::startAnchor() const
{
    return m_line->startAnchor();
}

void CConnection::setStartAnchor(Chronicler::Anchor anchor)
{
    m_line->setStartAnchor(anchor);
}

Anchor CConnection::endAnchor() const
{
    return m_line->endAnchor();
}

void CConnection::setEndAnchor(Chronicler::Anchor anchor)
{
    m_line->setEndAnchor(anchor);
}

void CConnection::ConnectToUIDs()
{
    CBubble *from = shared().projectView->BubbleWithUID(m_fromUID);
    CBubble *to = shared().projectView->BubbleWithUID(m_toUID);
    if(m_fromUID != -1)
        setFrom(from);
    if(m_toUID != -1)
        setTo(to);
}

QDataStream & CConnection::Read(QDataStream &ds)
{
    int start, end;

    ds >> start
       >> end
       >> m_fromUID
       >> m_toUID;

    m_line->setStartAnchor(Anchor(start));
    m_line->setEndAnchor(Anchor(end));

    return ds;
}

QDataStream & CConnection::Write(QDataStream &ds)
{
    ds << static_cast<qint32>(m_line->startAnchor())
       << static_cast<qint32>(m_line->endAnchor())
       << m_from->UID()
       << m_to->UID();

    return ds;
}

