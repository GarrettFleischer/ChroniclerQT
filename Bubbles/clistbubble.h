#ifndef CLISTBUBBLE_H
#define CLISTBUBBLE_H

#include "cbubble.h"

class CTextItem;
class CStringListModel;


class CListBubble : public CBubble
{
    Q_OBJECT

public:
    CListBubble(QMenu *contextMenu, const QPointF &pos, const CPalette &palette, const QFont &font = QFont(), QGraphicsItem *parent = 0);

    virtual void setFont(const QFont &font) override;

    CStringListModel *model();

protected:
    virtual void AdjustMinSize();

    CStringListModel *m_model;

protected slots:
    virtual void ModelUpdated() = 0;
};

#endif // CLISTBUBBLE_H
