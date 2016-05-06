#ifndef CACTIONBUBBLE_H
#define CACTIONBUBBLE_H

#include "cbubble.h"

QT_BEGIN_NAMESPACE
class QStringList;
QT_END_NAMESPACE

class CConnection;

class CActionBubble : public CBubble
{
public:
    CActionBubble(QMenu *contextMenu, const QPointF &pos, const CPalette &palette, const QFont &font = QFont(), QGraphicsItem *parent = 0);
    virtual ~CActionBubble();

//    void AddAction(QString action);
//    void RemoveAction(QString action);

    QStringList * actions() { return &m_actions; }

    virtual void AddLink(CConnection *link);
    virtual void RemoveLink(CConnection *link);
    virtual QList<CConnection *> links();

    virtual Anchor AnchorAtPosition(const QPointF &pos);

protected:
    virtual void UpdatePolygon();

private:
    void AdjustMinSize();

    QStringList m_actions;

    CConnection *m_link;

};

#endif // CACTIONBUBBLE_H
