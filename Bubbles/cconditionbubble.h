#ifndef CACTIONBUBBLE_H
#define CACTIONBUBBLE_H

#include "cbubble.h"
#include "Misc/ctextitem.h"

class CConditionBubble : public CBubble
{
    Q_OBJECT

public:
    friend class CConditionProperties;

public:
    CConditionBubble(QMenu *contextMenu, const QPointF &pos, const Chronicler::CPalette &palette, const QFont &font = QFont(), QGraphicsItem *parent = 0);

public:
    virtual void setFont(const QFont &font);

    virtual void setPalette(const Chronicler::CPalette &palette);
    //virtual void setFontColor(const QColor &color);

    void setCondition(const QString &condition);

protected:
    virtual void UpdateShape();
    void AdjustMinSize();

private:
    CTextItem * m_condition;
    QString m_conditionText;


};

#endif // CACTIONBUBBLE_H
