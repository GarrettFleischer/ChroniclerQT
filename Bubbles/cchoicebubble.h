#ifndef CCHOICEBUBBLE_H
#define CCHOICEBUBBLE_H

#include "cbubble.h"

class CStringListModel;
class CChoice;

class CChoiceBubble : public CBubble
{
    Q_OBJECT

public:
    CChoiceBubble(QMenu *contextMenu, const QPointF &pos, const CPalette &palette, const QFont &font = QFont(), QGraphicsItem *parent = 0);

    virtual void setPalette(const Chronicler::CPalette &palette) override;
    virtual void setFont(const QFont &font) override;

    virtual Chronicler::Anchor OutputAnchorAtPosition(const QPointF &) override;
    virtual Chronicler::Anchor InputAnchorAtPosition(const QPointF &pos) override;

    virtual void AddLink(CConnection *);
    virtual void RemoveLink(CConnection *);
    virtual QList<CConnection *> links();

    CStringListModel *choices();

protected:
    virtual void UpdatePolygon() override;

private:
    virtual void AdjustMinSize();

    CStringListModel *m_choices;
    QList<CChoice *> m_choiceBubbles;

private slots:
    void ModelUpdated();
};

#endif // CCHOICEBUBBLE_H
