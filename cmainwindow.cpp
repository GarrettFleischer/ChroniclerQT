#include "cmainwindow.h"

#include <QtWidgets>
#include <QStringListModel>
#include <QtGlobal>
#include <limits>
#include <QTabWidget>
#include <QSettings>
#include <QMessageBox>
#include <QStatusBar>

#include "cgraphicsscene.h"
#include "Bubbles/cstorybubble.h"
#include "cgraphicsview.h"
#include "chomepage.h"
#include "Properties/cdockmanager.h"
#include "csettingsview.h"
#include "Properties/cprojectview.h"

#include "Properties/cpalettecreator.h"
#include "Misc/Palette/cpalettebutton.h"
#include "Misc/Palette/cpaletteaction.h"

#include "Misc/chronicler.h"
using Chronicler::shared;


CMainWindow::CMainWindow(QSettings *settings, const QString &filename)
{
    setWindowTitle(tr("Chronicler ") + shared().ProgramVersion);
    setUnifiedTitleAndToolBarOnMac(false);

    shared().mainWindow = this;

    shared().statusBar = new QStatusBar(this);
    setStatusBar(shared().statusBar);

    // Load the settings...
    shared().settingsView = new CSettingsView(settings);
    connect(shared().settingsView, SIGNAL(SettingsChanged()),
            this, SLOT(SettingsChanged()));
    SettingsChanged();

    CreateActions();
    CreateMenus();

    QStringList lst = QStringList() << "*set" << "*create" << "*temp" << "*if" << "*elseif" << "*else";
    shared().actionsModel = new QStringListModel(lst, this);

    shared().dock = new QDockWidget(tr("Project"), this);
    connect(shared().dock, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),
            this, SLOT(DockAreaChanged(Qt::DockWidgetArea)));

    shared().dockManager = new CDockManager(shared().dock);
    shared().dock->setWidget(shared().dockManager);
    shared().dock->setVisible(false);
    shared().dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    // load the last dock widget area from the settings
    Qt::DockWidgetArea area = static_cast<Qt::DockWidgetArea>(shared().settingsView->settings()->value("MainWindow/DockArea",
                                                                                                       static_cast<int>(Qt::LeftDockWidgetArea)).toInt());
    addDockWidget(area, shared().dock);

    shared().homepage = new CHomepage(this);

    shared().sceneTabs = new QTabWidget(this);
    shared().sceneTabs->setMovable(true);
    shared().sceneTabs->setTabsClosable(true);
    connect(shared().sceneTabs, SIGNAL(tabCloseRequested(int)),
            this, SLOT(TabClosed(int)));

    shared().sceneTabs->addTab(shared().homepage, tr("Homepage"));

    setCentralWidget(shared().sceneTabs);

    CreateToolbars();

    // to allow .chron files to be opened with Chronicler
    if(filename.length())
        shared().projectView->OpenProject(filename);
}


void CMainWindow::DeleteItem()
{
    shared().dockManager->setBubble(0);

    CGraphicsView *view = dynamic_cast<CGraphicsView *>(shared().sceneTabs->currentWidget());

    if(view)
    {
        for(QGraphicsItem *item : view->cScene()->selectedItems())
        {
            CBubble *bubble = dynamic_cast<CBubble *>(item);
            if(bubble && bubble->getType() != Chronicler::Start)
                bubble->deleteLater();
        }
    }
}


void CMainWindow::PointerGroupClicked(int id)
{
    shared().setMode(Chronicler::Mode(id));
}

void CMainWindow::NewProject()
{
    shared().projectView->NewProject();
}

void CMainWindow::OpenProject()
{
    shared().projectView->OpenProject();
}

void CMainWindow::ImportProject()
{
    shared().projectView->ImportChoiceScript();
}

void CMainWindow::SaveProject()
{
    shared().projectView->SaveProject();
}

void CMainWindow::SaveAsProject()
{
    shared().projectView->SaveProjectAs();
}

void CMainWindow::ShowSettings()
{
    if(shared().sceneTabs->indexOf(shared().settingsView) == -1)
        shared().sceneTabs->insertTab(0, shared().settingsView, tr("Settings"));

    shared().sceneTabs->setCurrentWidget(shared().settingsView);
}

void CMainWindow::ShowHomepage()
{
    if(shared().sceneTabs->indexOf(shared().homepage) == -1)
        shared().sceneTabs->insertTab(0, shared().homepage, tr("Homepage"));

    shared().sceneTabs->setCurrentWidget(shared().homepage);
}

void CMainWindow::ShowDock()
{
    shared().dock->setVisible(!shared().dock->isVisible());
}

void CMainWindow::TabClosed(int index)
{
    if(shared().sceneTabs->widget(index) == shared().settingsView)
    {
        if(shared().settingsView->pendingChanges())
        {
            QCheckBox dontShow(tr("Remember my choice and don't show again."));
            dontShow.blockSignals(true); // performance
            QMessageBox msgBox;
            msgBox.setText(tr("Settings have been modified."));
            msgBox.setInformativeText(tr("Do you wish to apply these changes?"));
            msgBox.setStandardButtons(QMessageBox::Apply | QMessageBox::Discard | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Apply);
            msgBox.setCheckBox(&dontShow);
            int ret = msgBox.exec();

            if(ret == QMessageBox::Apply)
                shared().settingsView->ApplyPendingChanges();
            else if(ret == QMessageBox::Cancel)
                return;

            // TODO
            // save dontShow value...
        }
    }

    shared().sceneTabs->removeTab(index);
}

void CMainWindow::DockAreaChanged(Qt::DockWidgetArea area)
{
    // reflect this change in the settings.
    shared().settingsView->settings()->setValue("MainWindow/DockArea", static_cast<int>(area));
}

void CMainWindow::PointerToolBarAreaChanged(bool)
{
    // reflect this change in the settings.
    shared().settingsView->settings()->setValue("MainWindow/ToolBarArea", static_cast<int>(toolBarArea(shared().pointerToolBar)));
}

void CMainWindow::EscapePressed()
{
    qDebug() << "esc";
    shared().setMode(Chronicler::Cursor);
}

void CMainWindow::SettingsChanged()
{
    // Update font and font color.
    setFont(shared().settingsView->font());

    QPalette pal = palette();
    pal.setColor(QPalette::WindowText, shared().settingsView->fontColor());
    pal.setColor(QPalette::Text, shared().settingsView->fontColor());
    setPalette(pal);
}

void CMainWindow::ShowAbout()
{
    QMessageBox::about(this, tr("About Chronicler-Next"), tr("<b>Insert legal stuff here...</b>"));
}


void CMainWindow::CreateActions()
{
    shared().deleteAction = new QAction(QIcon(":/images/icn_trash.png"), tr("&Delete"), this);
    shared().deleteAction->setShortcut(QKeySequence::Delete);
    shared().deleteAction->setToolTip(tr("Delete selected bubble(S)"));
    connect(shared().deleteAction, SIGNAL(triggered()), this, SLOT(DeleteItem()));

    shared().exitAction = new QAction(QIcon(":/images/icn_exit.png"), tr("E&xit"), this);
    shared().exitAction->setShortcuts(QKeySequence::Quit);
    shared().exitAction->setToolTip(tr("Quit program"));
    connect(shared().exitAction, SIGNAL(triggered()), this, SLOT(close()));

    shared().settingsAction = new QAction(QIcon(":/images/icn_settings"), tr("&Settings"), this);
    shared().settingsAction->setShortcut(QKeySequence::Preferences);
    connect(shared().settingsAction, SIGNAL(triggered(bool)), this, SLOT(ShowSettings()));

    shared().aboutAction = new QAction(QIcon(":/images/icn_info"), tr("A&bout"), this);
    connect(shared().aboutAction, SIGNAL(triggered()), this, SLOT(ShowAbout()));

    shared().newProjectAction = new QAction(QIcon(":/images/icn_new"), tr("New Project"), this);
    shared().newProjectAction->setShortcut(QKeySequence::New);
    shared().newProjectAction->setToolTip(tr("Create New Project"));
    connect(shared().newProjectAction, SIGNAL(triggered(bool)), this, SLOT(NewProject()));

    shared().openProjectAction = new QAction(QIcon(":/images/icn_load"), tr("Open Project"), this);
    shared().openProjectAction->setShortcut(QKeySequence::Open);
    shared().openProjectAction->setToolTip(tr("Open Existing Project"));
    connect(shared().openProjectAction, SIGNAL(triggered(bool)), this, SLOT(OpenProject()));

    shared().importProjectAction = new QAction(QIcon(":/images/icn_loadcs"), tr("Import Project"), this);
    shared().importProjectAction->setToolTip(tr("Import ChoiceScript Project"));
    connect(shared().importProjectAction, SIGNAL(triggered(bool)), this, SLOT(ImportProject()));

    shared().saveProjectAction = new QAction(QIcon(":/images/icn_save"), tr("Save"), this);
    shared().saveProjectAction->setShortcut(QKeySequence::Save);
    shared().saveProjectAction->setToolTip(tr("Save Project"));
    connect(shared().saveProjectAction, SIGNAL(triggered(bool)), this, SLOT(SaveProject()));

    shared().saveAsProjectAction = new QAction(QIcon(":/images/icn_savecs"), tr("Save As"), this);
    shared().saveAsProjectAction->setShortcut(QKeySequence::SaveAs);
    shared().saveAsProjectAction->setToolTip(tr("Save Project As"));
    connect(shared().saveAsProjectAction, SIGNAL(triggered(bool)), this, SLOT(SaveAsProject()));

    shared().showHomepageAction = new QAction(QIcon(":/images/icn_home"), tr("Show &homepage"), this);
    connect(shared().showHomepageAction, SIGNAL(triggered(bool)), this, SLOT(ShowHomepage()));

    shared().showDockAction = new QAction(tr("Show &dock"), this);
    connect(shared().showDockAction, SIGNAL(triggered(bool)), this, SLOT(ShowDock()));

    QAction *escape_action = new QAction(Q_NULLPTR);
    escape_action->setShortcut(QKeySequence::Deselect);
    connect(escape_action, SIGNAL(triggered(bool)), this, SLOT(EscapePressed()));

    // Default Palettes
    CPalette dp_story, dp_choice, dp_action, dp_condition, dp_start;
    dp_story.fill = QColor(124, 140, 230);
    dp_choice.fill = QColor(104, 160, 210);
    dp_action.fill = QColor(161,88,136);
    dp_condition.fill = QColor(151,118,166);
    dp_start.fill = Qt::darkGreen;

    shared().defaultStory = new CPaletteAction(this, dp_story, tr("Story"), 1);
    shared().defaultChoice = new CPaletteAction(this, dp_choice, tr("Choice"), 2);
    shared().defaultAction = new CPaletteAction(this, dp_action, tr("Action"), 3);
    shared().defaultCondition = new CPaletteAction(this, dp_condition, tr("Condition"), 4);
    shared().defaultStart = new CPaletteAction(this, dp_start, tr("Start"), 5);
}


void CMainWindow::CreateMenus()
{
    shared().fileMenu = menuBar()->addMenu(tr("&File"));
    shared().fileMenu->addAction(shared().newProjectAction);
    shared().fileMenu->addAction(shared().openProjectAction);
    shared().fileMenu->addAction(shared().importProjectAction);
    shared().fileMenu->addSeparator();
    shared().fileMenu->addAction(shared().saveProjectAction);
    shared().fileMenu->addAction(shared().saveAsProjectAction);
    shared().fileMenu->addSeparator();
    shared().fileMenu->addAction(shared().settingsAction);
    shared().fileMenu->addSeparator();
    shared().fileMenu->addAction(shared().exitAction);

    shared().editMenu = menuBar()->addMenu(tr("&Edit"));
    shared().editMenu->addAction(shared().deleteAction);

    shared().viewMenu = menuBar()->addMenu(tr("&View"));
    shared().viewMenu->addAction(shared().showHomepageAction);
    shared().viewMenu->addAction(shared().showDockAction);

    shared().helpMenu = menuBar()->addMenu(tr("&Help"));
    shared().helpMenu->addAction(shared().aboutAction);
}


void CMainWindow::CreateToolbars()
{
    QToolButton *tb_pointer = new QToolButton();
    tb_pointer->setCheckable(true);
    tb_pointer->setChecked(true);
    tb_pointer->setIcon(QIcon(":/images/icn_pointer.png"));
    tb_pointer->setToolTip(tr("Selection tool"));
    QToolButton *tb_link = new QToolButton();
    tb_link->setCheckable(true);
    tb_link->setIcon(QIcon(":/images/icn_link.png"));
    tb_link->setToolTip(tr("Link tool\nLeft drag: create link\nRight click: remove link"));


    QToolButton *tb_story = new QToolButton();
    tb_story->setCheckable(true);
    tb_story->setIcon(QIcon(":/images/icn_story.png"));
    tb_story->setToolTip(tr("Story bubble"));
    QToolButton *tb_condition = new QToolButton();
    tb_condition->setCheckable(true);
    tb_condition->setIcon(QIcon(":/images/icn_condition.png"));
    tb_condition->setToolTip(tr("Condition bubble"));
    QToolButton *tb_action = new QToolButton();
    tb_action->setCheckable(true);
    tb_action->setIcon(QIcon(":/images/icn_action.png"));
    tb_action->setToolTip(tr("Action bubble"));
    QToolButton *tb_choice = new QToolButton();
    tb_choice->setCheckable(true);
    tb_choice->setIcon(QIcon(":/images/icn_choice2.png"));
    tb_choice->setToolTip(tr("Choice bubble"));

    // Palette creator
    shared().paletteButton = new CPaletteButton();

    shared().pointerTypeGroup = new QButtonGroup(this);
    shared().pointerTypeGroup->addButton(tb_pointer, int(Chronicler::Cursor));
    shared().pointerTypeGroup->addButton(tb_link, int(Chronicler::InsertConnection));
    shared().pointerTypeGroup->addButton(tb_story, int(Chronicler::InsertStory));
    shared().pointerTypeGroup->addButton(tb_condition, int(Chronicler::InsertCondition));
    shared().pointerTypeGroup->addButton(tb_choice, int(Chronicler::InsertChoice));
    shared().pointerTypeGroup->addButton(tb_action, int(Chronicler::InsertAction));
    shared().pointerTypeGroup->addButton(shared().paletteButton, int(Chronicler::Paint));
    connect(shared().pointerTypeGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(PointerGroupClicked(int)));


    Qt::ToolBarArea area = static_cast<Qt::ToolBarArea>(shared().settingsView->settings()->value("MainWindow/ToolBarArea",
                                                                                                 static_cast<int>(Qt::RightToolBarArea)).toInt());
    shared().pointerToolBar = new QToolBar(tr("Pointer type"));
    shared().pointerToolBar->setVisible(false);
    shared().pointerToolBar->addWidget(tb_pointer);
    shared().pointerToolBar->addWidget(tb_link);
    shared().pointerToolBar->addWidget(tb_story);
    shared().pointerToolBar->addWidget(tb_choice);
    shared().pointerToolBar->addWidget(tb_action);
    shared().pointerToolBar->addWidget(tb_condition);
    shared().pointerToolBar->addWidget(shared().paletteButton);
    shared().pointerToolBar->setIconSize(QSize(32,32));
    addToolBar(area, shared().pointerToolBar);
    connect(shared().pointerToolBar, SIGNAL(topLevelChanged(bool)),
            this, SLOT(PointerToolBarAreaChanged(bool)));
}

void CMainWindow::closeEvent(QCloseEvent *event)
{
    shared().settingsView->settings()->setValue("MainWindow/Geometry", geometry());
    event->accept();
}
