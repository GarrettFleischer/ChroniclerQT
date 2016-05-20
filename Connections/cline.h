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
using Chronicler::CPalette;


class CLine : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit CLine(const QPointF &start, const QPointF &end, Anchor anc_start = Anchor::Down, Anchor anc_end = Anchor::Up, QObject *parent = 0);
    explicit CLine(const CLine & copy);

    CLine & operator=(const CLine & rhs);

    virtual QPainterPath shape() const;
    virtual QRectF boundingRect() const;

    CPalette palette() const;
    void setPalette(const CPalette &palette);

    const QPointF & start() const { return m_start; }
    void setStart(const QPointF &start);

    const QPointF & end() const { return m_end; }
    void setEnd(const QPointF &end);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    Anchor startAnchor() const;
    void setStartAnchor(const Anchor &startAnchor);

    Anchor endAnchor() const;
    void setEndAnchor(const Anchor &endAnchor);

signals:

public slots:


private:
    void UpdateShape();

    QPainterPath    m_path;
    QPainterPath    m_arrow;
    QPointF         m_start;
    QPointF         m_end;
    Anchor          m_startAnchor;
    Anchor          m_endAnchor;
    CPalette        m_palette;
    float           m_width;

};

#endif // CLINE_H
