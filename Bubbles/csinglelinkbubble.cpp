#include "csinglelinkbubble.h"

#include "Connections/cconnection.h"
#include "cgraphicsscene.h"
#include "cgraphicsscene.h"


CSingleLinkBubble::CSingleLinkBubble(const QPointF &pos, const Chronicler::CPalette &palette, const QFont &font, QGraphicsItem *parent)
    : CBubble(pos, palette, font, parent), m_link(0)
{}

CSingleLinkBubble::~CSingleLinkBubble()
{
    dynamic_cast<CGraphicsScene *>(scene())->RemoveConnection(m_link);
    delete m_link;
}

CConnection *CSingleLinkBubble::link()
{
    return m_link;
}


void CSingleLinkBubble::AddLink(CConnection *link)
{
    if(m_link != link)
    {
        dynamic_cast<CGraphicsScene *>(scene())->RemoveConnection(m_link);
        delete m_link;
    }

    m_link = link;
}

void CSingleLinkBubble::RemoveLink(CConnection *link)
{
    if(m_link == link)
        m_link = 0;
}

void CSingleLinkBubble::RemoveLink(Chronicler::Anchor anchor)
{
    if(m_link && m_link->startAnchor() == anchor)
    {
        dynamic_cast<CGraphicsScene *>(scene())->RemoveConnection(m_link);
        delete m_link;
        m_link = 0;
    }
}

QList<CConnection *> CSingleLinkBubble::links()
{
    if(m_link)
        return { m_link };
    else
        return {};
}


QDataStream &CSingleLinkBubble::Read(QDataStream &ds)
{
    CBubble::Read(ds);

    bool linked;
    ds >> linked;

    if(linked)
    {
        m_link = dynamic_cast<CGraphicsScene *>(scene())->AddConnection();
        m_link->Read(ds);
    }

    return ds;
}

QDataStream & CSingleLinkBubble::Write(QDataStream &ds)
{
    CBubble::Write(ds);

    ds << bool(m_link);
    if(m_link)
        m_link->Write(ds);

    return ds;
}
