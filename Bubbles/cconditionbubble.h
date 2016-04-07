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
    CConditionBubble(QMenu *contextMenu, QGraphicsItem *parent = 0);

public:
    virtual void SetFont(const QFont &font);
    virtual void SetFontColor(const QColor &color);

    void SetCondition(const QString &condition);

protected:
    virtual void UpdateShape();
    void AdjustMinSize();

private:
    QString m_label;
    CTextItem * m_condition;
    QString m_conditionText;

};

#endif // CACTIONBUBBLE_H
