#ifndef CSTARTHEREBUBBLE_H
#define CSTARTHEREBUBBLE_H

#include "Bubbles/csinglelinkbubble.h"

class CVariablesView;
class CStartHereModel;
class CTextItem;

#include "Misc/Variables/cvariable.h"

class CStartHereBubble : public CSingleLinkBubble
{
public:
    CStartHereBubble(const QPointF &pos, CPaletteAction *palette, const QFont &font = QFont(), QGraphicsItem *parent = Q_NULLPTR);

    virtual void setLabel(QString label) Q_DECL_OVERRIDE;
    virtual void setFont(const QFont &font) Q_DECL_OVERRIDE;
    virtual void setPalette(CPaletteAction *palette) Q_DECL_OVERRIDE;

    QString customCode() const;
    void setCustomCode(const QString &customCode);

    CStartHereModel *model() const;

protected:
    virtual QDataStream &Deserialize(QDataStream &ds, const Chronicler::CVersion &version) Q_DECL_OVERRIDE;
    virtual QDataStream &Serialize(QDataStream &ds) const Q_DECL_OVERRIDE;

    virtual void UpdatePolygon() Q_DECL_OVERRIDE;

private:
    CStartHereModel *m_model;
    CTextItem *m_title;
    QString m_customCode;
};

#endif // CSTARTHEREBUBBLE_H
