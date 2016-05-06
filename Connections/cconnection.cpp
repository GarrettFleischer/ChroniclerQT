#include "cconnection.h"

#include <QPointF>
#include <QGraphicsScene>
#include <QtMath>

#include "cline.h"
#include "Bubbles/cbubble.h"

#include <QDebug>

CConnection::CConnection(CBubble *from, CBubble *to, Anchor anc_from, Anchor anc_to, QGraphicsScene *scn)
    : m_from(0), m_to(0)
{
    m_line = new CLine(QPointF(), QPointF(), anc_from, anc_to);
    m_line->setZValue(-999999);
    scn->addItem(m_line);

    setFrom(from);
    setTo(to);
}

CConnection::~CConnection()
{
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
                                       size.height() * 0.5 * qSin(angle)),
                         startAnchor());
    }
    if(m_to)
    {
        qreal angle = (endAnchor() * M_PI * 0.5);
        QPointF pos = m_to->sceneBoundingRect().center();
        QSizeF size = m_to->sceneBoundingRect().size();
        m_line->setEnd(pos + QPointF(size.width() * 0.5 * qCos(angle),
                                     size.height() * 0.5 * qSin(angle)),
                       endAnchor());
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

