#ifndef CUTILS_H
#define CUTILS_H

#include <QColor>

QT_BEGIN_NAMESPACE
class QTabWidget;
class QAction;
class QMenu;
class QToolBar;
class QButtonGroup;
class QDockWidget;
QT_END_NAMESPACE

class CGraphicsScene;
class CGraphicsView;
class CHomepage;
class CSettingsView;
class CDockManager;


namespace Chronicler
{

    enum BubbleType { Story, Choice, Action, Condition  };

    enum Anchor { Right, Down, Left, Up, None };
    enum AllowedAnchors { NoAnchor = 0, RightAnchor = 1, DownAnchor = 2, LeftAnchor = 4, UpAnchor = 8 };

    struct CPalette
    {
        QColor fill = QColor(124, 140, 230);
        QColor line = Qt::black;
        QColor select = QColor(255,200,0);
        QColor font = Qt::white;
    };


    // Do NOT instantiate this struct, use shared() singleton access.
    struct SharedWidgets
    {
        QTabWidget *sceneTabs;

        CHomepage *homepage;
        CSettingsView *settingsView;

        QDockWidget *dock;
        CDockManager *dockManager;

        QToolBar *pointerToolBar;
        QButtonGroup *pointerTypeGroup;

        QMenu *fileMenu;
        QMenu *editMenu;
        QMenu *viewMenu;
        QMenu *helpMenu;

        QAction *exitAction;
        QAction *deleteAction;
        QAction *aboutAction;
        QAction *settingsAction;
        QAction *newProjectAction;
        QAction *openProjectAction;
        QAction *importProjectAction;
        QAction *showHomepageAction;
    };

    SharedWidgets &shared();

}


#endif // CUTILS_H
