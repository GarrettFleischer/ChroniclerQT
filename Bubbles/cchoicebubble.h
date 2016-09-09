#ifndef CCHOICEBUBBLE_H
#define CCHOICEBUBBLE_H

#include "cbubble.h"

class CChoiceModel;
class CChoice;

class CChoiceBubble : public CBubble
{
    Q_OBJECT

public:
    typedef QList<CChoice *> choiceList;

    CChoiceBubble(const QPointF &pos, CPaletteAction *palette, const QFont &font = QFont(), QGraphicsItem *parent = Q_NULLPTR);

    virtual void setPalette(CPaletteAction *palette) override;
    virtual void setFont(const QFont &font) override;

    virtual Chronicler::Anchor OutputAnchorAtPosition(const QPointF &) override;
    virtual Chronicler::Anchor InputAnchorAtPosition(const QPointF &pos) override;

    virtual void AddLink(CConnection *) override;
    virtual void RemoveLink(CConnection *) override;
    virtual void RemoveLink(Chronicler::Anchor) override;
    virtual QList<CConnection *> links() override;

    CChoiceModel *choices();
    choiceList choiceBubbles();

protected:
    virtual void UpdatePolygon() override;

private:
    virtual void AdjustMinSize();

    virtual QDataStream &Deserialize(QDataStream &ds, const CVersion &version) override;
    virtual QDataStream &Serialize(QDataStream &ds) override;

    CChoiceModel *m_choices;

private slots:
    void ModelUpdated();


    // CBubble interface
public:
    virtual void UpdateUID() Q_DECL_OVERRIDE;
};

#endif // CCHOICEBUBBLE_H
