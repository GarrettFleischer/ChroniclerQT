#ifndef CCHOICEBUBBLE_H
#define CCHOICEBUBBLE_H

#include "clistbubble.h"

class CTextItem;


class CChoiceBubble : public CListBubble
{
    Q_OBJECT

public:
    CChoiceBubble(QMenu *contextMenu, const QPointF &pos, const CPalette &palette, const QFont &font = QFont(), QGraphicsItem *parent = 0);

    virtual void setPalette(const Chronicler::CPalette &palette) override;

    virtual void AddLink(CConnection *link);
    virtual void RemoveLink(CConnection *link);
    virtual QList<CConnection *> links();

protected:
    virtual void UpdatePolygon() override;

private:
    CTextItem *m_modelView;

protected slots:
    virtual void ModelUpdated() override;

};

#endif // CCHOICEBUBBLE_H
