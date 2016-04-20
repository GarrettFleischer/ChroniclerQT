#ifndef CACTIONBUBBLE_H
#define CACTIONBUBBLE_H

#include "cbubble.h"

class CActionBubble : public CBubble
{
public:
    CActionBubble(QMenu *contextMenu, const QPointF &pos, const CPalette &palette, const QFont &font = QFont(), QGraphicsItem *parent = 0);


protected:
    virtual void UpdateShape();

private:
    void AdjustMinSize();
};

#endif // CACTIONBUBBLE_H
