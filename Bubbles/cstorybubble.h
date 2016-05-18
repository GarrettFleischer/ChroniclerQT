#ifndef CSTORYBUBBLE_H
#define CSTORYBUBBLE_H


#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>

#include "Bubbles/csinglelinkbubble.h"
#include "Misc/ctextitem.h"

class CConnection;


class CStoryBubble : public CSingleLinkBubble
{
    Q_OBJECT

public:
    CStoryBubble(const QPointF &pos, const Chronicler::CPalette &palette, const QFont &font = QFont(), QGraphicsItem *parent = 0);

    virtual void setFont(const QFont &font);
    virtual void setPalette(const Chronicler::CPalette &palette);

    virtual void setLabel(QString label);

    void setStory(QString story) { m_story->setText(story); }
    QString getStory() { return m_story->Text(); }

protected:
    virtual void UpdatePolygon();

private:
    CTextItem * m_title;
    CTextItem * m_story;

};

#endif // CSTORYBUBBLE_H
