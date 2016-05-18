#ifndef CSINGLELINKBUBBLE_H
#define CSINGLELINKBUBBLE_H

#include "cbubble.h"

class CConnection;

class CSingleLinkBubble : public CBubble
{
public:
    CSingleLinkBubble(const QPointF &pos, const CPalette &palette, const QFont &font, QGraphicsItem *parent);
    ~CSingleLinkBubble();

    virtual void AddLink(CConnection *link);
    virtual void RemoveLink(CConnection *link);
    virtual QList<CConnection *> links();

protected:
    CConnection *m_link;
};

#endif // CSINGLELINKBUBBLE_H
