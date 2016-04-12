#ifndef CBUBBLE_H
#define CBUBBLE_H


#include <QObject>
#include <QGraphicsPixmapItem>
#include <QList>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>

QT_BEGIN_NAMESPACE
class QGraphicsItem;
class QGraphicsSceneMouseEvent;
class QStyleOptionGraphicsItem;
class QPolygonF;
QT_END_NAMESPACE

class Arrow;
class CLink;

class CBubble : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT

public:
    enum BType { Story, Condition, Choice};

    CBubble(QGraphicsItem *parent);
    CBubble(QMenu *contextMenu, const QPointF &pos, const QFont &font = QFont(), const QColor &fontColor = Qt::black,
            const QColor &lineColor = Qt::black,
            QGraphicsItem *parent = 0);

public:
    void addArrow(Arrow *arrow);
    void removeArrow(Arrow *arrow);
    void removeArrows();

    QPolygonF polygon() const { return m_polygon; }

    virtual void SetFont(const QFont &font);
    QFont GetFont() { return m_font; }

    virtual void SetFontColor(const QColor &color);
    virtual void SetColor(const QColor &color);
    virtual void SetLineColor(const QColor &color);


    BType GetType() const { return m_type; }

protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *evt);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *evt);
    
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    virtual void UpdateShape() = 0;

protected:
    QMenu *m_contextMenu;
    QRectF m_bounds;
    QSizeF m_minSize;
    QPolygonF m_polygon;
    QList<Arrow *> m_links;
    QList<Arrow *> m_connections;
    int m_order;
    bool m_locked;
    QFont m_font;
    QColor m_fontColor;
    QColor m_color;
    QColor m_lineColor;
    BType m_type;

    
signals:
    void Selected(QGraphicsItem *item);
    void linkClicked(CLink *);
};

#endif // CBUBBLE_H
