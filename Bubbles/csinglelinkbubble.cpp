#include "csinglelinkbubble.h"

#include "Connections/cconnection.h"


CSingleLinkBubble::CSingleLinkBubble(QMenu *contextMenu, const QPointF &pos, const Chronicler::CPalette &palette, const QFont &font, QGraphicsItem *parent)
    : CBubble(contextMenu, pos, palette, font, parent), m_link(0)
{}

CSingleLinkBubble::~CSingleLinkBubble()
{
    delete m_link;
}


void CSingleLinkBubble::AddLink(CConnection *link)
{
    if(m_link != link)
        delete m_link;

    m_link = link;
}

void CSingleLinkBubble::RemoveLink(CConnection *link)
{
    if(m_link == link)
        m_link = 0;
}

QList<CConnection *> CSingleLinkBubble::links()
{
    if(m_link)
        return { m_link };
    else
        return {};
}
