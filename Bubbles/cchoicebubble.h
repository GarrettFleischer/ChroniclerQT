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

    CChoiceBubble(const QPointF &pos, const CPalette &palette, const QFont &font = QFont(), QGraphicsItem *parent = 0);

    virtual void setPalette(const Chronicler::CPalette &palette) override;
    virtual void setFont(const QFont &font) override;

    virtual Chronicler::Anchor OutputAnchorAtPosition(const QPointF &) override;
    virtual Chronicler::Anchor InputAnchorAtPosition(const QPointF &pos) override;

    virtual void AddLink(CConnection *);
    virtual void RemoveLink(CConnection *);
    virtual QList<CConnection *> links();

    CChoiceModel *choices();

protected:
    virtual void UpdatePolygon() override;

private:
    virtual void AdjustMinSize();

    CChoiceModel *m_choices;

private slots:
    void ModelUpdated();
};

#endif // CCHOICEBUBBLE_H
