#include "cactionmenu.h"

#include <QMouseEvent>
#include "Misc/cpaletteaction.h"

CActionMenu::CActionMenu(QWidget *parent)
    : QMenu(parent)
{}


void CActionMenu::mousePressEvent(QMouseEvent *event)
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
