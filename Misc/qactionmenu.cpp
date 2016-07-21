#include "qactionmenu.h"

#include <QMouseEvent>

QActionMenu::QActionMenu(QWidget *parent)
    : QMenu(parent)
{}


void QActionMenu::mousePressEvent(QMouseEvent *event)
{
    QAction *a = actionAt(event->pos());
    if(a)
    {
        if(event->button() == Qt::LeftButton)
            emit leftTriggered(a);
        else if(event->button() == Qt::RightButton)
            emit rightTriggered(a);
    }

    QMenu::mousePressEvent(event);
}
