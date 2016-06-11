#ifndef CGRAPHICSVIEW_H
#define CGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QObject>

class CGraphicsScene;


class CGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    CGraphicsView(CGraphicsScene *scene, QWidget *parent = 0);

    CGraphicsScene *cScene();

protected:
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void wheelEvent(QWheelEvent *);

private:
    void GentleZoom(double factor, const QPointF &mouse_pos);

    CGraphicsScene *m_scene;

    double m_scale;
    double m_minScale;
    double m_maxScale;
    Qt::KeyboardModifiers m_modifiers;
    double m_zoom_factor_base;
    QPointF target_scene_pos, target_scene_pos2, target_viewport_pos, target_viewport_pos2;

public slots:
    void Recenter();
};

#endif // CGRAPHICSVIEW_H
