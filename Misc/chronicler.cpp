#include "chronicler.h"

#include <QGuiApplication>
#include <QButtonGroup>
#include <QAbstractButton>

#include "cgraphicsview.h"
#include "cgraphicsscene.h"
#include "Connections/cline.h"
#include "Properties/cprojectview.h"
#include "Misc/cpalettebutton.h"
#include "Misc/cpaletteaction.h"

static Chronicler::SharedInstances shared_singleton = Chronicler::SharedInstances();



namespace Chronicler
{
    SharedInstances &shared()
    {
        return shared_singleton;
    }

    void SharedInstances::setMode(Mode mode)
    {
        cursorMode = mode;

        pointerTypeGroup->button(int(cursorMode))->setChecked(true);

        QGuiApplication::restoreOverrideCursor();
        if(cursorMode == Chronicler::Paint)
            QGuiApplication::setOverrideCursor(QCursor(paletteButton->getCurrent()->icon().pixmap(32, 32)));

        for(CGraphicsView *view : projectView->views())
        {
            view->setDragMode(QGraphicsView::ScrollHandDrag);

            if(cursorMode == Chronicler::Cursor)
                view->cScene()->getLine()->hide();
            else if(cursorMode == Chronicler::InsertConnection)
                view->setDragMode(QGraphicsView::NoDrag);
        }
    }
}
