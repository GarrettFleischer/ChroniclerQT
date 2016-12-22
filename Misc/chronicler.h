#ifndef CUTILS_H
#define CUTILS_H

#include <QColor>

QT_BEGIN_NAMESPACE
class QTabWidget;
class QAction;
class QMenu;
class QToolBar;
class QToolButton;
class QButtonGroup;
class QDockWidget;
class QStringListModel;
class QStatusBar;
class QUndoStack;
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

    enum BubbleType { StoryBubble, ChoiceBubble, ActionBubble, ConditionBubble, StartBubble, ChoiceItemBubble, CodeBubble, StartHereBubble };

    enum Anchor { EastAnchor, SouthAnchor, WestAnchor, NorthAnchor, NoAnchor };

    enum Mode { InsertConnection, Cursor, InsertStory, InsertCondition, InsertChoice, InsertAction, InsertCode, InsertStartHere, Paint };

    enum Command { AddBubblesCommand, RemoveBubblesCommand, MoveBubblesCommand, ResizeBubbleCommand, RemoveSceneCommand };

    enum Stat { TextStat, PercentStat, OpposedPairStat, CustomStat };

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

    struct CVersion
    {
    public:
        QString string;


        CVersion(QString _version) : string(_version) {}

        bool operator ==(const CVersion &rhs) const
        {
            return rhs.string == string;
        }

        bool operator ==(const QString &rhs) const
        {
            return rhs == string;
        }

        bool operator <(QString _version) const
        {
            return versionToInt(string) < versionToInt(_version);
        }

        bool operator >(QString _version) const
        {
            return versionToInt(string) > versionToInt(_version);
        }

        bool operator <=(QString _version) const
        {
            return versionToInt(string) <= versionToInt(_version);
        }

        bool operator >=(QString _version) const
        {
            return versionToInt(string) >= versionToInt(_version);
        }

        friend QDataStream & operator<<(QDataStream &stream, const CVersion &rhs)
        {
            return stream << rhs.string;
        }

        friend QDataStream & operator>>(QDataStream &stream, CVersion &rhs)
        {
            return stream >> rhs.string;
        }

    private:
        int versionToInt(const QString &versionToInt) const;
    };


    /// @brief Do NOT instantiate this struct, use shared() singleton access.
    struct SharedInstances
    {
        const CVersion ProgramVersion = CVersion("0.9.17.1");

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

        QToolButton *debugButton;

        QMenu *fileMenu;
        QMenu *editMenu;
        QMenu *viewMenu;
        QMenu *toolsMenu;
        QMenu *helpMenu;

        QAction *exitAction;
        QAction *deleteAction;
        QAction *copyAction;
        QAction *pasteAction;
        QAction *aboutAction;
        QAction *forumAction;
        QAction *settingsAction;
        QAction *newProjectAction;
        QAction *openProjectAction;
        QAction *importProjectAction;
        QAction *importSceneAction;
        QAction *saveProjectAction;
        QAction *saveAsProjectAction;
        QAction *showHomepageAction;
        QAction *showDockAction;
        QAction *undoAction;
        QAction *redoAction;
        QAction *quickTestAction;
        QAction *randomTestAction;
        QAction *compileAction;
        QAction *playAction;
        QAction *debugAction;

        QUndoStack *history;

        CPaletteAction *defaultStory;
        CPaletteAction *defaultChoice;
        CPaletteAction *defaultAction;
        CPaletteAction *defaultCondition;
        CPaletteAction *defaultCode;
        CPaletteAction *defaultStart;
        CPaletteAction *defaultTrue;
        CPaletteAction *defaultFalse;

        QString globalVariableTitle = "Global";

        Mode cursorMode = Cursor;

        void setMode(Mode mode);

    };

    SharedInstances &shared();

}


#endif // CUTILS_H
