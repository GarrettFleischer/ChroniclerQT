#ifndef CCHOICEBUBBLE_H
#define CCHOICEBUBBLE_H

#include "cbubble.h"

class CStringListModel;
class CTextItem;


class CChoiceBubble : public CBubble
{
    Q_OBJECT

public:
    CChoiceBubble(QMenu *contextMenu, const QPointF &pos, const CPalette &palette, const QFont &font = QFont(), QGraphicsItem *parent = 0);

    virtual void setPalette(const Chronicler::CPalette &palette) override;
    virtual void setFont(const QFont &font) override;

    virtual void AddLink(CConnection *link);
    virtual void RemoveLink(CConnection *link);
    virtual QList<CConnection *> links();

    CStringListModel *choices();

protected:
    virtual void UpdatePolygon() override;

private:
    virtual void AdjustMinSize();

    CStringListModel *m_choices;
    CTextItem *m_modelView;

private slots:
    void ModelUpdated();

};

#endif // CCHOICEBUBBLE_H
