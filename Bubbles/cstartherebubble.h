#ifndef CSTARTHEREBUBBLE_H
#define CSTARTHEREBUBBLE_H

#include "Bubbles/csinglelinkbubble.h"

class CVariablesView;
class CStartHereModel;

#include "Misc/Variables/cvariable.h"

class CStartHereBubble : public CSingleLinkBubble
{
public:
    CStartHereBubble(const QPointF &pos, CPaletteAction *palette, const QFont &font = QFont(), QGraphicsItem *parent = Q_NULLPTR);

    CStartHereModel *model() const;

protected:
    virtual QDataStream &Deserialize(QDataStream &ds, const Chronicler::CVersion &version) Q_DECL_OVERRIDE;
    virtual QDataStream &Serialize(QDataStream &ds) Q_DECL_OVERRIDE;

private:
    CStartHereModel *m_model;

};

#endif // CSTARTHEREBUBBLE_H
