#ifndef CCHOICEBUBBLE_H
#define CCHOICEBUBBLE_H


#include "cbubble.h"
#include "Misc/ctextitem.h"

class CChoiceBubble : public CBubble
{
    Q_OBJECT

public:
    friend class CConditionProperties;

public:
    CChoiceBubble(QMenu *contextMenu, const QPointF &pos, const CPalette &palette, const QFont &font = QFont(), QGraphicsItem *parent = 0);

public:
    virtual void setFont(const QFont &font);
    virtual void setPalette(const Chronicler::CPalette &palette);

    void AddChoice(const QString &choice);
    void MoveChoice(int old_index, int new_index);

protected:
    virtual void UpdateShape();
    void AdjustMinSize();

private:
    QList<CTextItem *> m_choices;

};

#endif // CCHOICEBUBBLE_H
