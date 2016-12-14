#ifndef CSINGLELINKBUBBLE_H
#define CSINGLELINKBUBBLE_H

#include "cbubble.h"

class CConnection;

class CSingleLinkBubble : public CBubble
{
public:
    CSingleLinkBubble(const QPointF &pos, CPaletteAction *palette, const QFont &font, QGraphicsItem *parent);
    virtual ~CSingleLinkBubble();

    CConnection * link();
    virtual void AddLink(CConnection *link) Q_DECL_OVERRIDE;
    virtual void RemoveLink(CConnection *link) Q_DECL_OVERRIDE;
    virtual void RemoveLink(Chronicler::Anchor anchor) Q_DECL_OVERRIDE;
    virtual QList<CConnection *> links() Q_DECL_OVERRIDE;

protected:
    CConnection *m_link;

    virtual QDataStream &Deserialize(QDataStream &ds, const CVersion &version) Q_DECL_OVERRIDE;
    virtual QDataStream &Serialize(QDataStream &ds) const Q_DECL_OVERRIDE;

};

#endif // CSINGLELINKBUBBLE_H
