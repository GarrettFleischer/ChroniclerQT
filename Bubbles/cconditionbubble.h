#ifndef CCONDITIONBUBBLE_H
#define CCONDITIONBUBBLE_H

#include "cbubble.h"

class CTextItem;
class CConnection;

class CConditionBubble : public CBubble
{
    Q_OBJECT

public:
    CConditionBubble(const QPointF &pos, CPaletteAction *palette, const QFont &font = QFont(), QGraphicsItem *parent = Q_NULLPTR);
    virtual ~CConditionBubble();

    virtual void setFont(const QFont &font) Q_DECL_OVERRIDE;
    virtual void setPalette(CPaletteAction *palette) Q_DECL_OVERRIDE;

    virtual void AddLink(CConnection *link) Q_DECL_OVERRIDE;
    virtual void RemoveLink(CConnection *link) Q_DECL_OVERRIDE;
    virtual void RemoveLink(Chronicler::Anchor anchor) Q_DECL_OVERRIDE;
    virtual QList<CConnection *> links() Q_DECL_OVERRIDE;

    virtual Anchor OutputAnchorAtPosition(const QPointF &pos) Q_DECL_OVERRIDE;
    virtual Anchor InputAnchorAtPosition(const QPointF &) Q_DECL_OVERRIDE;

    virtual CPaletteAction *getPaletteForAnchor(Chronicler::Anchor anchor) Q_DECL_OVERRIDE;

    CConnection *trueLink();
    CConnection *falseLink();

    void setCondition(QString condition);
    QString getCondition() { return m_conditionText; }

protected:
    virtual void UpdatePolygon() Q_DECL_OVERRIDE;
    void AdjustMinSize();

    virtual QDataStream &Deserialize(QDataStream &ds, const CVersion &version) Q_DECL_OVERRIDE;
    virtual QDataStream &Serialize(QDataStream &ds) const Q_DECL_OVERRIDE;

private:
    CTextItem *m_condition;
    QString m_conditionText;

    CConnection *m_trueLink;
    CConnection *m_falseLink;


    // CBubble interface
public:


    // QGraphicsItem interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
};

#endif // CCONDITIONBUBBLE_H
