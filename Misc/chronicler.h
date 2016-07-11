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
class QStatusBar;
QT_END_NAMESPACE

class CMainWindow;
class CGraphicsScene;
class CGraphicsView;
class CHomepage;
class CSettingsView;
class CProjectView;
class CVariablesView;
class CDockManager;
class CPaletteButton;
class CPaletteAction;

namespace Chronicler
{

    typedef quint64 t_uid;

    enum BubbleType { Story, Choice, Action, Condition, Start, ChoiceItem };

    enum Anchor { Right, Down, Left, Up, None };

    enum Mode { InsertConnection, Cursor, InsertStory, InsertCondition, InsertChoice, InsertAction, Paint };

    struct CPalette
    {
        QColor fill = QColor(124, 140, 230);
        QColor line = Qt::black;
        QColor select = QColor(255,200,0);
        QColor font = Qt::white;

        friend QDataStream & operator<<(QDataStream &stream, CPalette &p)
        {
            return stream << p.fill << p.line << p.select << p.font;
        }

        friend QDataStream & operator>>(QDataStream &stream, CPalette &p)
        {
            return stream >> p.fill >> p.line >> p.select >> p.font;
        }
    };

    struct CSIndent
    {
        char type = ' ';
        quint8 count = 4;
    };

    struct CVariable
    {
        CGraphicsScene *scene = Q_NULLPTR;
        QString name;
        QString data;

        CVariable()
            : scene(Q_NULLPTR) {}

        CVariable(CGraphicsScene *_scene, QString _name, QString _data)
            : scene(_scene), name(_name), data(_data) {}

        bool operator ==(const CVariable &rhs)
        {
            return (rhs.scene == scene && rhs.name == name && rhs.data == data);
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
        CVariablesView *variablesView;

        QDockWidget *dock;
        CDockManager *dockManager;

        QStatusBar *statusBar;

        QToolBar *pointerToolBar;
        QButtonGroup *pointerTypeGroup;
        CPaletteButton *paletteButton;

        QStringListModel *actionsModel;

        QMenu *fileMenu;
        QMenu *editMenu;
        QMenu *viewMenu;
        QMenu *helpMenu;

        QAction *exitAction;
        QAction *deleteAction;
        QAction *copyAction;
        QAction *aboutAction;
        QAction *settingsAction;
        QAction *newProjectAction;
        QAction *openProjectAction;
        QAction *importProjectAction;
        QAction *saveProjectAction;
        QAction *saveAsProjectAction;
        QAction *showHomepageAction;

        CPaletteAction *defaultStory;
        CPaletteAction *defaultChoice;
        CPaletteAction *defaultAction;
        CPaletteAction *defaultCondition;
        CPaletteAction *defaultStart;

        Mode cursorMode = Cursor;


        void setMode(Mode mode);
    };

    SharedInstances &shared();

}


#endif // CUTILS_H
