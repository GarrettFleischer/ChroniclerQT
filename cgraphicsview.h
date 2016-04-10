#ifndef CGRAPHICSVIEW_H
#define CGRAPHICSVIEW_H

#include <QGraphicsView>


class CGraphicsView : public QGraphicsView
{
public:
    CGraphicsView(QGraphicsScene *scene, QWidget *parent = 0);

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void wheelEvent(QWheelEvent *);
    virtual void keyPressEvent(QKeyEvent *);
    virtual void keyReleaseEvent(QKeyEvent *);

private:
    void GentleZoom(double factor, const QPointF &mouse_pos);

    double m_scale;
    double m_minScale;
    double m_maxScale;
    Qt::KeyboardModifiers m_modifiers;
    double m_zoom_factor_base;
    QPointF target_scene_pos, target_scene_pos2, target_viewport_pos, target_viewport_pos2;
};

#endif // CGRAPHICSVIEW_H
