#include "cgraphicsview.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QApplication>
#include <QScrollBar>
#include <QtMath>
#include <QtGlobal>


CGraphicsView::CGraphicsView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent), m_scale(1.0), m_minScale(0.05), m_maxScale(5.0),
      m_modifiers(Qt::NoModifier), m_zoom_factor_base(1.0015)
{
    setMouseTracking(true);
    setDragMode(ScrollHandDrag);
    setRenderHint(QPainter::Antialiasing, true);
    //setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void CGraphicsView::mouseMoveEvent(QMouseEvent *evt)
{
    QPointF delta = target_viewport_pos - evt->pos();
    if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5)
    {
        target_viewport_pos = evt->pos();
        target_viewport_pos2 = QPoint(viewport()->width(), viewport()->height()) - evt->pos();
        target_scene_pos = mapToScene(target_viewport_pos.toPoint());
        target_scene_pos2 = mapToScene(target_viewport_pos2.toPoint());
    }

    QGraphicsView::mouseMoveEvent(evt);
}

void CGraphicsView::wheelEvent(QWheelEvent *evt)
{
    if (QApplication::keyboardModifiers() == m_modifiers)
    {
        if (evt->orientation() == Qt::Vertical)
        {
            double angle = evt->angleDelta().y();
            double factor = qPow(m_zoom_factor_base, angle);
            GentleZoom(factor, evt->posF());
        }
    }
    else
        QGraphicsView::wheelEvent(evt);
}


void CGraphicsView::GentleZoom(double factor, const QPointF & mouse_pos)
{
    double newscale = m_scale * factor;

    if(newscale > m_minScale && newscale < m_maxScale)
    {
        m_scale = newscale;
        scale(factor, factor);

        // zoom in
        if(factor > 1)
        {
            centerOn(target_scene_pos);
            QPointF delta_viewport_pos = target_viewport_pos - QPointF(viewport()->width() / 2.0,
                                                                       viewport()->height() / 2.0);
            QPointF viewport_center = mapFromScene(target_scene_pos) - delta_viewport_pos;
            centerOn(mapToScene(viewport_center.toPoint()));

            // update opposite positions
            target_viewport_pos2 = QPoint(viewport()->width(), viewport()->height()) - mouse_pos;
            target_scene_pos2 = mapToScene(target_viewport_pos2.toPoint());
        }
        // zoom out
        else
        {
            centerOn(target_scene_pos2);
            QPointF delta_viewport_pos = target_viewport_pos2 - QPointF(viewport()->width() / 2.0,
                                                                       viewport()->height() / 2.0);
            QPointF viewport_center = mapFromScene(target_scene_pos2) - delta_viewport_pos;
            centerOn(mapToScene(viewport_center.toPoint()));

            // update opposite positions
            target_viewport_pos = mouse_pos;
            target_scene_pos = mapToScene(target_viewport_pos.toPoint());
        }
    }
}

