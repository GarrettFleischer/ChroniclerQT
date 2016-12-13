#include "cstartherebubble.h"

#include "Misc/Bubbles/cstartheremodel.h"

CStartHereBubble::CStartHereBubble(const QPointF &pos, CPaletteAction *palette, const QFont &font, QGraphicsItem *parent)
    : CSingleLinkBubble(pos, palette, font, parent)
{
    m_model = new CStartHereModel(this);
}


QDataStream &CStartHereBubble::Deserialize(QDataStream &ds, const Chronicler::CVersion &version)
{
    CSingleLinkBubble::Deserialize(ds, version);

    QList<CVariable> modelData;
    ds >> modelData;

    m_model->setVariables(modelData);

    return ds;
}

QDataStream &CStartHereBubble::Serialize(QDataStream &ds)
{
    CSingleLinkBubble::Serialize(ds);

    ds << m_model->variables();

    return ds;
}

CStartHereModel *CStartHereBubble::model() const
{
    return m_model;
}
