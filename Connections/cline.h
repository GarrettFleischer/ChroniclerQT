#ifndef CLINE_H
#define CLINE_H

#include <QObject>
#include<QGraphicsItem>

QT_BEGIN_NAMESPACE
class QPainter;
class QPainterPath;
class QPointF;
QT_END_NAMESPACE


class CLine : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit CLine(const QPointF &start, const QPointF &end, QObject *parent = 0);
    explicit CLine(const CLine & copy);

    CLine & operator=(const CLine & rhs);

    virtual QPainterPath shape() const;
    virtual QRectF boundingRect() const;

    const QPointF & setStart() const;
    void setStart(const QPointF &start);

    const QPointF & setEnd() const;
    void setEnd(const QPointF &end);

signals:

public slots:


private:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    void UpdateShape();

    QPainterPath m_path;
    QPointF m_start;
    QPointF m_end;
    uint m_offset;

};

#endif // CLINE_H
