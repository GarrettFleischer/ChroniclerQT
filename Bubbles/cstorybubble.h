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
    friend class CStoryProperties;

public:
    CStoryBubble(QMenu *contextMenu, const QPointF &pos, const Chronicler::CPalette &palette, const QFont &font = QFont(), QGraphicsItem *parent = 0);

public:
    virtual void setFont(const QFont &font);
    virtual void setPalette(const Chronicler::CPalette &palette);

    virtual void setLabel(QString label);

//    virtual void setFontColor(const QColor &color);
//    virtual void setColor(const QColor &color);
//    virtual void setLineColor(const QColor &color);


protected:
    virtual void UpdateShape();


    virtual void mousePressEvent(QGraphicsSceneMouseEvent *evt);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *evt);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *evt);

    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *evt);
//    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *evt);

private:
    CTextItem * m_title;
    CTextItem * m_story;
    
//    CLink *m_linkLeft;
//    CLink *m_linkDown;
//    CLink *m_linkRight;
    
    bool m_resize;
    QPointF m_offset;
    QRectF m_lastBounds;
    
public slots:
//    void LinkClicked(CLink *link);

};

#endif // CSTORYBUBBLE_H
