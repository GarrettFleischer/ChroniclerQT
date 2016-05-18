#ifndef CACTIONBUBBLE_H
#define CACTIONBUBBLE_H

#include "csinglelinkbubble.h"

class CTextItem;
class CStringListModel;

class CActionBubble : public CSingleLinkBubble
{
    Q_OBJECT

public:
    CActionBubble(const QPointF &pos, const CPalette &palette, const QFont &font = QFont(), QGraphicsItem *parent = 0);

    virtual void setPalette(const Chronicler::CPalette &palette) override;
    virtual void setFont(const QFont &font) override;

    CStringListModel *actions();

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
