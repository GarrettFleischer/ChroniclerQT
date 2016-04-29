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
    enum Anchor { RIGHT, DOWN, LEFT, UP };

    explicit CLine(const QPointF &start, const QPointF &end, QObject *parent = 0);
    explicit CLine(const CLine & copy);

    CLine & operator=(const CLine & rhs);

    virtual QPainterPath shape() const;
    virtual QRectF boundingRect() const;

    const QPointF & start() const { return m_start; }
    void setStart(const QPointF &start);

    const QPointF & end() const { return m_end; }
    void setEnd(const QPointF &end);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

signals:

public slots:


private:
    void UpdateShape();

    QPainterPath m_path;
    QPointF m_start;
    QPointF m_end;
    Anchor m_startAnchor;
    Anchor m_endAnchor;
    int m_offset;
    float m_width;

};

#endif // CLINE_H
