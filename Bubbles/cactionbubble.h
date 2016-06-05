#ifndef CACTIONBUBBLE_H
#define CACTIONBUBBLE_H

#include "csinglelinkbubble.h"

class CTextItem;
class CStringListModel;

class CActionBubble : public CSingleLinkBubble
{
    Q_OBJECT

public:
    CActionBubble(t_uid uid, const QPointF &pos, CPaletteAction *palette, const QFont &font = QFont(), QGraphicsItem *parent = 0);

    virtual void setPalette(CPaletteAction *palette) override;
    virtual void setFont(const QFont &font) override;

    virtual QDataStream &Deserialize(QDataStream &ds, const QString &version) override;
    virtual QDataStream &Serialize(QDataStream &ds) override;

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
