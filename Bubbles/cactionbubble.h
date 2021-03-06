#ifndef CACTIONBUBBLE_H
#define CACTIONBUBBLE_H

#include "csinglelinkbubble.h"

class CTextItem;
class CStringListModel;

class CActionBubble : public CSingleLinkBubble
{
    Q_OBJECT

public:
    CActionBubble(const QPointF &pos, CPaletteAction *palette, const QFont &font = QFont(), QGraphicsItem *parent = Q_NULLPTR);

    virtual void setPalette(CPaletteAction *palette) override;
    virtual void setFont(const QFont &font) override;

    virtual QDataStream &Deserialize(QDataStream &ds, const CVersion &version) override;
    virtual QDataStream &Serialize(QDataStream &ds) const override;

    CStringListModel *actions();

    QString actionString();

protected:
    virtual void UpdatePolygon() override;

private:
    void AdjustMinSize();

    CStringListModel *m_actions;
    CTextItem *m_actionsView;

private slots:
    void ModelUpdated();

};

#endif // CACTIONBUBBLE_H
