#ifndef CLINE_H
#define CLINE_H

#include <QObject>
#include<QGraphicsItem>

QT_BEGIN_NAMESPACE
class QPainter;
class QPainterPath;
class QPointF;
QT_END_NAMESPACE

#include "Misc/chronicler.h"
using Chronicler::Anchor;


class CLine : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit CLine(const QPointF &start, const QPointF &end, Anchor anc_start = Anchor::DOWN, Anchor anc_end = Anchor::UP, QObject *parent = 0);
    explicit CLine(const CLine & copy);

    CLine & operator=(const CLine & rhs);

    virtual QPainterPath shape() const;
    virtual QRectF boundingRect() const;

    const QPointF & start() const { return m_start; }
    void setStart(const QPointF &start, Anchor anchor = Anchor::DOWN);

    const QPointF & end() const { return m_end; }
    void setEnd(const QPointF &end, Anchor anchor = Anchor::UP);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    Anchor startAnchor() const;
    void setStartAnchor(const Anchor &startAnchor);

    Anchor endAnchor() const;
    void setEndAnchor(const Anchor &endAnchor);

signals:

public slots:


private:
    void UpdateShape();

    QPainterPath m_path;
    QPainterPath m_arrow;
    QPointF m_start;
    QPointF m_end;
    Anchor m_startAnchor;
    Anchor m_endAnchor;
    int m_offset;
    float m_width;

};

#endif // CLINE_H
