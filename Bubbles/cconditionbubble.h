#ifndef CCONDITIONBUBBLE_H
#define CCONDITIONBUBBLE_H

#include "cbubble.h"

class CTextItem;
class CConnection;

class CConditionBubble : public CBubble
{
    Q_OBJECT

public:
    CConditionBubble(const QPointF &pos, CPaletteAction *palette, const QFont &font = QFont(), QGraphicsItem *parent = 0);
    virtual ~CConditionBubble();

    virtual void setFont(const QFont &font) override;
    virtual void setPalette(CPaletteAction *palette) override;

    virtual void AddLink(CConnection *link) override;
    virtual void RemoveLink(CConnection *link) override;
    virtual void RemoveLink(Chronicler::Anchor anchor) override;
    virtual QList<CConnection *> links() override;

    virtual Anchor OutputAnchorAtPosition(const QPointF &pos) override;
    virtual Anchor InputAnchorAtPosition(const QPointF &) override;

    CConnection *trueLink();
    CConnection *falseLink();

    void setCondition(QString condition);
    QString getCondition() { return m_conditionText; }

protected:
    virtual void UpdatePolygon() override;
    void AdjustMinSize();

    virtual QDataStream &Deserialize(QDataStream &ds, const CVersion &version) override;
    virtual QDataStream &Serialize(QDataStream &ds) override;

private:
    CTextItem *m_condition;
    QString m_conditionText;

    CConnection *m_trueLink;
    CConnection *m_falseLink;

};

#endif // CCONDITIONBUBBLE_H
