#include "cmainwindow.h"

#include <QtWidgets>
#include <QStringListModel>
#include <QtGlobal>
#include <limits>
#include <QTabWidget>
#include <QFileInfo>
#include <QSettings>
#include <QMessageBox>

#include "cgraphicsscene.h"
#include "Bubbles/cstorybubble.h"
#include "cgraphicsview.h"
#include "chomepage.h"
#include "Properties/cdockmanager.h"
#include "csettingsview.h"

#include "Misc/chronicler.h"
using Chronicler::shared;


const int InsertTextButton = 10;


CMainWindow::CMainWindow(QSettings *settings)
    : m_ShiftHeld(false)
{
    setWindowTitle(tr("Chronicler"));
    setUnifiedTitleAndToolBarOnMac(true);

    // Load the settings...
    shared().settingsView = new CSettingsView(settings);
    connect(shared().settingsView, SIGNAL(SettingsChanged()),
            this, SLOT(SettingsChanged()));
    SettingsChanged();

    CreateActions();
    CreateMenus();

    m_scene = new CGraphicsScene(this);
    m_scene->setFont(shared().settingsView->font());

    m_view = new CGraphicsView(m_scene);

    QStringList lst = QStringList() << "*set" << "*action" << "*create" << "*if" << "*elseif" << "${name}" << "${title}" << "${strength}";
    shared().actionsModel = new QStringListModel(lst, this);

    shared().dock = new QDockWidget("Project", this);
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

    shared().sceneTabs->addTab(shared().homepage, "Homepage");
    //shared().sceneTabs->addTab(m_view, "startup.scn");

    setCentralWidget(shared().sceneTabs);

    CreateToolbars();


}

void CMainWindow::LoadProject(const QString &filepath)
{
    setWindowTitle("Chronicler - " + QFileInfo(filepath).baseName());

    shared().dock->setVisible(true);
    shared().dock->setWindowTitle(QFileInfo(filepath).fileName());
    shared().sceneTabs->addTab(m_view, "startup.scn");
    shared().sceneTabs->setCurrentWidget(m_view);

    shared().sceneTabs->removeTab(shared().sceneTabs->indexOf(shared().homepage));
}


void CMainWindow::DeleteItem()
{
    shared().dockManager->setBubble(0);

    foreach (QGraphicsItem *item, m_scene->selectedItems())
        delete item;
}


void CMainWindow::PointerGroupClicked(int id)
{
    m_scene->setMode(CGraphicsScene::Mode(id));
}

void CMainWindow::ShowSettings()
{
    if(shared().sceneTabs->indexOf(shared().settingsView) == -1)
        shared().sceneTabs->insertTab(0, shared().settingsView, "Settings");

    shared().sceneTabs->setCurrentWidget(shared().settingsView);
}

void CMainWindow::NewProject()
{
    // TODO popup new project dialog
}

void CMainWindow::OpenProject()
{
    // TODO popup ".chron" file picker
}

void CMainWindow::ImportProject()
{
    // TODO popup "startup.txt" file picker
}

void CMainWindow::ShowHomepage()
{
    if(shared().sceneTabs->indexOf(shared().homepage) == -1)
        shared().sceneTabs->insertTab(0, shared().homepage, "Homepage");

    shared().sceneTabs->setCurrentWidget(shared().homepage);
}

void CMainWindow::TabClosed(int index)
{
    if(shared().sceneTabs->widget(index) == shared().settingsView)
    {
        if(shared().settingsView->pendingChanges())
        {
            QCheckBox dontShow("Remember my choice and don't show again.");
            dontShow.blockSignals(true); // performance
            QMessageBox msgBox;
            msgBox.setText("Settings have been modified.");
            msgBox.setInformativeText("Do you wish to apply these changes?");
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
    shared().deleteAction->setShortcut(tr("Delete"));
    shared().deleteAction->setToolTip(tr("Delete selected bubble(S)"));
    connect(shared().deleteAction, SIGNAL(triggered()), this, SLOT(DeleteItem()));

    shared().exitAction = new QAction(QIcon(":/images/icn_exit.png"), tr("E&xit"), this);
    shared().exitAction->setShortcuts(QKeySequence::Quit);
    shared().exitAction->setToolTip(tr("Quit program"));
    connect(shared().exitAction, SIGNAL(triggered()), this, SLOT(close()));

    shared().settingsAction = new QAction(QIcon(":/images/icn_settings"), tr("&Settings"), this);
    shared().settingsAction->setShortcut(tr("Ctrl+P"));
    connect(shared().settingsAction, SIGNAL(triggered(bool)), this, SLOT(ShowSettings()));

    shared().aboutAction = new QAction(QIcon(":/images/icn_info"), tr("A&bout"), this);
    connect(shared().aboutAction, SIGNAL(triggered()), this, SLOT(ShowAbout()));

    shared().newProjectAction = new QAction(QIcon(":/images/icn_new"), tr("New Project"), this);
    shared().newProjectAction->setShortcut(tr("Ctrl+N"));
    shared().newProjectAction->setToolTip("Create New Project");
    connect(shared().newProjectAction, SIGNAL(triggered(bool)), this, SLOT(NewProject()));

    shared().openProjectAction = new QAction(QIcon(":/images/icn_load"), tr("Open Project"), this);
    shared().openProjectAction->setShortcut(tr("Ctrl+O"));
    shared().openProjectAction->setToolTip("Open Existing Project");
    connect(shared().openProjectAction, SIGNAL(triggered(bool)), this, SLOT(OpenProject()));

    shared().importProjectAction = new QAction(QIcon(":/images/icn_loadcs"), tr("Import Project"), this);
    shared().importProjectAction->setToolTip("Import ChoiceScript Project");
    connect(shared().importProjectAction, SIGNAL(triggered(bool)), this, SLOT(ImportProject()));

    shared().showHomepageAction = new QAction(QIcon(":/images/icn_home"), tr("Show &homepage"), this);
    connect(shared().showHomepageAction, SIGNAL(triggered(bool)), this, SLOT(ShowHomepage()));
}


void CMainWindow::CreateMenus()
{
    shared().fileMenu = menuBar()->addMenu(tr("&File"));
    shared().fileMenu->addAction(shared().newProjectAction);
    shared().fileMenu->addAction(shared().openProjectAction);
    shared().fileMenu->addAction(shared().importProjectAction);
    shared().fileMenu->addSeparator();
    shared().fileMenu->addAction(shared().settingsAction);
    shared().fileMenu->addSeparator();
    shared().fileMenu->addAction(shared().exitAction);

    shared().editMenu = menuBar()->addMenu(tr("&Edit"));
    shared().editMenu->addAction(shared().deleteAction);

    shared().viewMenu = menuBar()->addMenu(tr("&View"));
    shared().viewMenu->addAction(shared().showHomepageAction);

    shared().helpMenu = menuBar()->addMenu(tr("&Help"));
    shared().helpMenu->addAction(shared().aboutAction);
}


void CMainWindow::CreateToolbars()
{
    QToolButton *tb_pointer = new QToolButton();
    tb_pointer->setCheckable(true);
    tb_pointer->setChecked(true);
    tb_pointer->setIcon(QIcon(":/images/icn_pointer.png"));
    tb_pointer->setToolTip("Selection tool");
    QToolButton *tb_link = new QToolButton();
    tb_link->setCheckable(true);
    tb_link->setIcon(QIcon(":/images/icn_link.png"));
    tb_link->setToolTip("Link tool");


    QToolButton *tb_story = new QToolButton();
    tb_story->setCheckable(true);
    tb_story->setIcon(QIcon(":/images/icn_story.png"));
    tb_story->setToolTip("Story bubble");
    QToolButton *tb_condition = new QToolButton();
    tb_condition->setCheckable(true);
    tb_condition->setIcon(QIcon(":/images/icn_condition.png"));
    tb_condition->setToolTip("Condition bubble");
    QToolButton *tb_action = new QToolButton();
    tb_action->setCheckable(true);
    tb_action->setIcon(QIcon(":/images/icn_action.png"));
    tb_action->setToolTip("Action bubble");
    QToolButton *tb_choice = new QToolButton();
    tb_choice->setCheckable(true);
    tb_choice->setIcon(QIcon(":/images/icn_choice2.png"));
    tb_choice->setToolTip("Choice bubble");

    shared().pointerTypeGroup = new QButtonGroup(this);
    shared().pointerTypeGroup->addButton(tb_pointer, int(CGraphicsScene::Cursor));
    shared().pointerTypeGroup->addButton(tb_link, int(CGraphicsScene::InsertConnection));
    shared().pointerTypeGroup->addButton(tb_story, int(CGraphicsScene::InsertStory));
    shared().pointerTypeGroup->addButton(tb_condition, int(CGraphicsScene::InsertCondition));
    shared().pointerTypeGroup->addButton(tb_choice, int(CGraphicsScene::InsertChoice));
    shared().pointerTypeGroup->addButton(tb_action, int(CGraphicsScene::InsertAction));
    connect(shared().pointerTypeGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(PointerGroupClicked(int)));


    Qt::ToolBarArea area = static_cast<Qt::ToolBarArea>(shared().settingsView->settings()->value("MainWindow/ToolBarArea",
                                                                                          static_cast<int>(Qt::RightToolBarArea)).toInt());
    shared().pointerToolBar = new QToolBar("Pointer type");
    shared().pointerToolBar->addWidget(tb_pointer);
    shared().pointerToolBar->addWidget(tb_link);
    shared().pointerToolBar->addWidget(tb_story);
    shared().pointerToolBar->addWidget(tb_choice);
    shared().pointerToolBar->addWidget(tb_action);
    shared().pointerToolBar->addWidget(tb_condition);
    shared().pointerToolBar->setIconSize(QSize(32,32));
    addToolBar(area, shared().pointerToolBar);
    connect(shared().pointerToolBar, SIGNAL(topLevelChanged(bool)),
            this, SLOT(PointerToolBarAreaChanged(bool)));
}
