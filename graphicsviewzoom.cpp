#include "graphicsviewzoom.h"
#include <QMouseEvent>
#include <QApplication>
#include <QScrollBar>
#include <qmath.h>
#include <QtGlobal>

GraphicsViewZoom::GraphicsViewZoom(QGraphicsView* view)
    : QObject(view), m_view(view), m_scale(1.0), m_minScale(0.05), m_maxScale(5.0)
{
    m_view->viewport()->installEventFilter(this);
    m_view->setMouseTracking(true);
    m_modifiers = Qt::ControlModifier;
    m_zoom_factor_base = 1.0015;
}

void GraphicsViewZoom::GentleZoom(double factor)
{
    double newscale = m_scale * factor;

    if(newscale > m_minScale && newscale < m_maxScale)
    {
        m_scale = newscale;
        m_view->scale(factor, factor);
        m_view->centerOn(target_scene_pos);
        QPointF delta_viewport_pos = target_viewport_pos - QPointF(m_view->viewport()->width() / 2.0,
                                                                   m_view->viewport()->height() / 2.0);
        QPointF viewport_center = m_view->mapFromScene(target_scene_pos) - delta_viewport_pos;
        m_view->centerOn(m_view->mapToScene(viewport_center.toPoint()));
        emit zoomed();
    }
}

void GraphicsViewZoom::SetModifiers(Qt::KeyboardModifiers modifiers)
{
    m_modifiers = modifiers;
}

void GraphicsViewZoom::SetZoomFactor(double value)
{
    m_zoom_factor_base = value;
}

bool GraphicsViewZoom::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent* mouse_event = dynamic_cast<QMouseEvent*>(event);
        QPointF delta = target_viewport_pos - mouse_event->pos();
        if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5)
        {
            target_viewport_pos = mouse_event->pos();
            target_scene_pos = m_view->mapToScene(mouse_event->pos());
        }
    }
    else if (event->type() == QEvent::Wheel)
    {
        QWheelEvent* wheel_event = dynamic_cast<QWheelEvent*>(event);
        if (QApplication::keyboardModifiers() == m_modifiers)
        {
            if (wheel_event->orientation() == Qt::Vertical)
            {
                double angle = wheel_event->angleDelta().y();
                double factor = qPow(m_zoom_factor_base, angle);
                GentleZoom(factor);
                return true;
            }
        }
    }

    return false;
}
