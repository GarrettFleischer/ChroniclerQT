#ifndef CSTORYBUBBLE_H
#define CSTORYBUBBLE_H


#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>

#include "Bubbles/cbubble.h"
#include "Misc/ctextitem.h"
#include "Connections/clink.h"


class CStoryBubble : public CBubble
{
    Q_OBJECT

public:
    CStoryBubble(QMenu *contextMenu, const QPointF &pos, const Chronicler::CPalette &palette, const QFont &font = QFont(), QGraphicsItem *parent = 0);

    virtual void setFont(const QFont &font);
    virtual void setPalette(const Chronicler::CPalette &palette);

    virtual void setLabel(QString label);

    void setStory(QString story) { m_story->setText(story); }
    QString getStory() { return m_story->Text(); }

protected:
    virtual void UpdatePolygon();

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *evt);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *evt);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *evt);

    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *evt);

private:
    CTextItem * m_title;
    CTextItem * m_story;
    
    bool m_resize;
    QPointF m_offset;
    QRectF m_lastBounds;

};

#endif // CSTORYBUBBLE_H
