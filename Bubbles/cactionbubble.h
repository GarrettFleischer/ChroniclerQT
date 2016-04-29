#ifndef CACTIONBUBBLE_H
#define CACTIONBUBBLE_H

#include "cbubble.h"

QT_BEGIN_NAMESPACE
class QStringList;
QT_END_NAMESPACE

class CActionBubble : public CBubble
{
public:
    CActionBubble(QMenu *contextMenu, const QPointF &pos, const CPalette &palette, const QFont &font = QFont(), QGraphicsItem *parent = 0);

//    void AddAction(QString action);
//    void RemoveAction(QString action);

    QStringList * actions() { return &m_actions; }

protected:
    virtual void UpdatePolygon();

private:
    void AdjustMinSize();

    QStringList m_actions;

    // CBubble interface
public:
    virtual void AddLink(CConnection *link, int position);
    virtual void RemoveLink(CConnection *link);

    // CBubble interface
public:
    virtual QList<CConnection *> links();
};

#endif // CACTIONBUBBLE_H
