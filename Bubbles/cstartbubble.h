#ifndef CSTARTBUBBLE_H
#define CSTARTBUBBLE_H

#include "csinglelinkbubble.h"

class CStartBubble : public CSingleLinkBubble
{
public:
    CStartBubble(const QPointF &pos, const CPalette &palette, const QFont &font, QGraphicsItem *parent = 0);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *) override;
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *) override;

    virtual void UpdatePolygon() override;

};

#endif // CSTARTBUBBLE_H
