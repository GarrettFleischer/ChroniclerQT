#ifndef CCONDITIONBUBBLE_H
#define CCONDITIONBUBBLE_H

#include "cbubble.h"

class CTextItem;
class CConnection;

class CConditionBubble : public CBubble
{
    Q_OBJECT

public:
    CConditionBubble(QMenu *contextMenu, const QPointF &pos, const Chronicler::CPalette &palette, const QFont &font = QFont(), QGraphicsItem *parent = 0);
    virtual ~CConditionBubble();

    virtual void setFont(const QFont &font);
    virtual void setPalette(const Chronicler::CPalette &palette);

    void setCondition(QString condition);
    QString getCondition() { return m_conditionText; }

    virtual void RemoveLink(CConnection *link);
    virtual void AddLink(CConnection *link);
    virtual QList<CConnection *> links();

    virtual Anchor OutputAnchorAtPosition(const QPointF &pos) override;
    virtual Anchor InputAnchorAtPosition(const QPointF &) override;

protected:
    virtual void UpdatePolygon() override;
    void AdjustMinSize();

private:
    CTextItem *m_condition;
    QString m_conditionText;

    CConnection *m_trueLink;
    CConnection *m_falseLink;

};

#endif // CCONDITIONBUBBLE_H
