#include "csinglelinkbubble.h"

#include "Connections/cconnection.h"
#include "cgraphicsscene.h"
#include "cgraphicsscene.h"


CSingleLinkBubble::CSingleLinkBubble(const QPointF &pos, CPaletteAction *palette, const QFont &font, QGraphicsItem *parent)
    : CBubble(pos, palette, font, parent), m_link(0)
{}

CSingleLinkBubble::~CSingleLinkBubble()
{
    CGraphicsScene *scn = dynamic_cast<CGraphicsScene *>(scene());
    if(scn)
        scn->RemoveConnection(m_link);
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
        m_link = Q_NULLPTR;
}

void CSingleLinkBubble::RemoveLink(Chronicler::Anchor anchor)
{
    if(m_link && m_link->startAnchor() == anchor)
    {
        dynamic_cast<CGraphicsScene *>(scene())->RemoveConnection(m_link);
        delete m_link;
        m_link = Q_NULLPTR;
    }
}

QList<CConnection *> CSingleLinkBubble::links()
{
    if(m_link)
        return { m_link };
    else
        return {};
}


QDataStream &CSingleLinkBubble::Deserialize(QDataStream &ds, const Chronicler::CVersion &version)
{
    CBubble::Deserialize(ds, version);

    bool linked;
    ds >> linked;

    if(linked)
    {
        m_link = dynamic_cast<CGraphicsScene *>(scene())->AddConnection();
        ds >> *m_link;
    }

    return ds;
}

QDataStream & CSingleLinkBubble::Serialize(QDataStream &ds) const
{
    CBubble::Serialize(ds);

    ds << bool(m_link);
    if(m_link)
        ds << *m_link;

    return ds;
}
