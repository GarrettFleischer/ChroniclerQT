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
    virtual void AddLink(CConnection *link) override;
    virtual void RemoveLink(CConnection *link) override;
    virtual void RemoveLink(Chronicler::Anchor anchor) override;
    virtual QList<CConnection *> links() override;

protected:
    CConnection *m_link;

    virtual QDataStream &Deserialize(QDataStream &ds, const QString &version) override;
    virtual QDataStream &Serialize(QDataStream &ds) override;

};

#endif // CSINGLELINKBUBBLE_H
