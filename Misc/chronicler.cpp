#include "chronicler.h"

#include <QGuiApplication>
#include <QButtonGroup>
#include <QAbstractButton>

#include <QtMath>

#include "cgraphicsview.h"
#include "cgraphicsscene.h"
#include "Connections/cline.h"
#include "Properties/cprojectview.h"
#include "Misc/Palette/cpalettebutton.h"
#include "Misc/Palette/cpaletteaction.h"

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

        for(CGraphicsView *view : projectView->getViews())
        {
            view->setDragMode(QGraphicsView::ScrollHandDrag);

            if(cursorMode == Chronicler::Cursor)
                view->cScene()->getLine()->hide();
            else if(cursorMode == Chronicler::InsertConnection)
                view->setDragMode(QGraphicsView::NoDrag);
        }
    }

    /**
     * @brief CVersion::versionDiff
     * @param v1
     * @param v2
     * @return 0 if v1 = v2, 1 if v1 > v2, -1 if v1 < v2
     */
    int CVersion::versionDiff(const QString &v1, const QString &v2) const
    {
        QStringList sv1 = v1.split(".", QString::SkipEmptyParts);
        QStringList sv2 = v2.split(".", QString::SkipEmptyParts);

        for(int i = 0; i < sv1.length(); ++i)
        {
            if(sv1[i].toInt() > sv2[i].toInt())
                return 1;
            else if(sv1[i].toInt() < sv2[i].toInt())
                return -1;
        }

        return 0;
    }
}
