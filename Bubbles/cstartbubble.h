#ifndef CSTARTBUBBLE_H
#define CSTARTBUBBLE_H

#include "csinglelinkbubble.h"

class CStartBubble : public CSingleLinkBubble
{
public:
    CStartBubble(const QPointF &pos, CPaletteAction *palette, const QFont &font, QGraphicsItem *parent = Q_NULLPTR);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *) override;
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *) override;

    virtual Chronicler::Anchor InputAnchorAtPosition(const QPointF &) override;

    virtual void UpdatePolygon() override;
};

#endif // CSTARTBUBBLE_H
