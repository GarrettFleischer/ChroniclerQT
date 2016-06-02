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
class QStringListModel;
QT_END_NAMESPACE

class CMainWindow;
class CGraphicsScene;
class CGraphicsView;
class CHomepage;
class CSettingsView;
class CProjectView;
class CDockManager;

namespace Chronicler
{

    typedef quint64 t_uid;

    enum BubbleType { Story, Choice, Action, Condition, Start };

    enum Anchor { Right, Down, Left, Up, None };

    enum Mode { InsertConnection, Cursor, InsertStory, InsertCondition, InsertChoice, InsertAction };

    struct CPalette
    {
        QColor fill = QColor(124, 140, 230);
        QColor line = Qt::black;
        QColor select = QColor(255,200,0);
        QColor font = Qt::white;

        friend QDataStream & operator<<(QDataStream &stream, CPalette p)
        {
            return stream << p.fill << p.line << p.select << p.font;
        }

        friend QDataStream & operator>>(QDataStream &stream, CPalette p)
        {
            return stream >> p.fill >> p.line >> p.select >> p.font;
        }
    };


    // Do NOT instantiate this struct, use shared() singleton access.
    struct SharedInstances
    {
        QString ProgramVersion;

        CMainWindow *mainWindow;

        QTabWidget *sceneTabs;

        CHomepage *homepage;
        CSettingsView *settingsView;
        CProjectView *projectView;

        QDockWidget *dock;
        CDockManager *dockManager;

        QToolBar *pointerToolBar;
        QButtonGroup *pointerTypeGroup;
        QMenu *paletteMenu;

        QStringListModel *actionsModel;

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
        QAction *saveProjectAction;
        QAction *saveAsProjectAction;
        QAction *showHomepageAction;

        Mode cursorMode = Cursor;
    };

    SharedInstances &shared();

}


#endif // CUTILS_H
