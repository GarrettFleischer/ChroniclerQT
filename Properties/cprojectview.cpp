#include "cprojectview.h"

#include <QFileInfo>

#include <QListView>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QFile>
#include <QSaveFile>
#include <QFileDialog>
#include <QAction>

#include <QMessageBox>

#include <QDockWidget>
#include <QTabWidget>

#include <QSettings>


#include "Misc/cscenemodel.h"
#include "cgraphicsscene.h"
#include "cgraphicsview.h"
#include "chomepage.h"
#include "cmainwindow.h"

#include "Bubbles/cbubble.h"
#include "Bubbles/cstartbubble.h"
#include "Bubbles/cstorybubble.h"
#include "Bubbles/cchoicebubble.h"
#include "Bubbles/cactionbubble.h"
#include "Bubbles/cconditionbubble.h"
#include "Connections/cconnection.h"

#include "csettingsview.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

Q_DECLARE_METATYPE(QStringList)


const QString CProjectView::currentVersion = "0.7.0.0";


CProjectView::CProjectView(QWidget *parent)
    : QWidget(parent), m_path(""), m_version(currentVersion)
{
    m_upButton = new QPushButton(QIcon(":/images/icn_up"), "");
    m_upButton->setEnabled(false);
    connect(m_upButton, SIGNAL(clicked(bool)), this, SLOT(MoveUp()));

    m_downButton = new QPushButton(QIcon(":/images/icn_down"), "");
    m_downButton->setEnabled(false);
    connect(m_downButton, SIGNAL(clicked(bool)), this, SLOT(MoveDown()));

    m_addButton = new QPushButton(QIcon(":/images/icn_add"), "");
    connect(m_addButton, SIGNAL(clicked(bool)), this, SLOT(AddItem()));

    m_removeButton = new QPushButton(QIcon(":/images/icn_trash"), "");
    m_removeButton->setEnabled(false);
    connect(m_removeButton, SIGNAL(clicked(bool)), this, SLOT(RemoveItem()));

    QVBoxLayout *vl_buttons = new QVBoxLayout();
    vl_buttons->addWidget(m_upButton);
    vl_buttons->addWidget(m_downButton);
    vl_buttons->addWidget(m_addButton);
    vl_buttons->addWidget(m_removeButton);
    vl_buttons->addStretch(1);

    m_sceneModel = new CSceneModel(this);

    m_modelView = new QListView();
    m_modelView->setModel(m_sceneModel);
    m_modelView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    connect(m_modelView, SIGNAL(clicked(QModelIndex)), this, SLOT(SelectedChanged(QModelIndex)));
    connect(m_sceneModel, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(DataChanged(QModelIndex,QModelIndex)));

    QHBoxLayout *hl_viewButtons = new QHBoxLayout();
    hl_viewButtons->addWidget(m_modelView);
    hl_viewButtons->addLayout(vl_buttons);

    m_name = new QLineEdit();
    connect(m_name, SIGNAL(textChanged(QString)), this, SLOT(ProjectNameChanged()));

    QVBoxLayout *l_main = new QVBoxLayout(this);
    l_main->addWidget(new QLabel("Title"));
    l_main->addWidget(m_name);
    l_main->addWidget(new QLabel("Scenes"));
    l_main->addLayout(hl_viewButtons);
}

void CProjectView::SaveProject()
{
    if(!m_path.length())
        SaveProjectAs();
    else
    {
        QByteArray ba;
        QDataStream ds(&ba, QIODevice::WriteOnly);

        ds << currentVersion << m_name->text() << m_sceneModel->rowCount();
        for(CGraphicsView *view : m_sceneModel->views())
            ds << *(view->cScene());


        ExportChoiceScript();

        QSaveFile file(m_path);
        file.open(QIODevice::WriteOnly);
        file.write(ba);

        while(!file.commit())
        {
            QMessageBox msgBox;
            msgBox.setText("Error writing to file!");
            msgBox.setInformativeText("Ensure that the file exists and that you have enough disk space.");
            msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Retry | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Retry);
            int ret = msgBox.exec();

            if(ret == QMessageBox::Save)
            {
                SaveProjectAs();
                break;
            }
            else if(ret == QMessageBox::Cancel)
                break;
        }

        QStringList recent_files = shared().settingsView->settings()->value("Homepage/RecentFiles").value<QStringList>();
        if(!recent_files.contains(m_path))
        {
            recent_files.insert(0, m_path);
            if(recent_files.length() > shared().settingsView->maxRecentFiles())
                recent_files.removeLast();
        }

        shared().settingsView->settings()->setValue("Homepage/RecentFiles", QVariant::fromValue(recent_files));
    }
}

void CProjectView::SaveProjectAs()
{
    QString dir = QFileInfo(m_path).absolutePath();
    QFileDialog dialog(this, "Save As", dir, "chronx (*.chronx)");
    dialog.setDefaultSuffix("chronx");
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);

    if(dialog.exec())
    {
        m_path = dialog.selectedFiles().first();
        SaveProject();
    }
}

void CProjectView::OpenProject(const QString &filepath)
{
    CloseProject();

    QFile file(filepath);
    while(!file.open(QIODevice::ReadOnly))
    {
        QString dir = QFileInfo("C:/Development/QT/Chronicler Saves/Test.chronx").absolutePath();
        file.setFileName(QFileDialog::getOpenFileName(this, "Open", dir, "chronx (*.chronx)"));
        if(!file.fileName().length())
            return;
    }

    m_path = file.fileName();


    // TODO
    // Load data from file and instantiate all scenes & bubbles...

    QByteArray ba(file.readAll());
    QDataStream ds(&ba, QIODevice::ReadOnly);

    int num_scenes;
    QString project_name;
    ds >> m_version >> project_name >> num_scenes;

    m_name->setText(project_name);

    // load other scenes
    for(int i = 0; i < num_scenes; ++i)
    {
        QString name;
        ds >> name;
        m_sceneModel->AddItem(new CGraphicsView(new CGraphicsScene(name), this));
        ds >> *(m_sceneModel->views().last()->cScene());
    }

    shared().dock->setVisible(true);
    shared().dock->setWindowTitle(m_path);//(QFileInfo(m_path).fileName());
    shared().sceneTabs->removeTab(shared().sceneTabs->indexOf(shared().homepage));

    // grab the startup view
    CGraphicsView *view = m_sceneModel->views().first();
    shared().sceneTabs->addTab(view, view->cScene()->name());
    shared().sceneTabs->setCurrentWidget(view);
    view->centerOn(view->scene()->sceneRect().center());
}

void CProjectView::ImportProject()
{

}

void CProjectView::NewProject()
{
    CloseProject();

    m_name->setText("New Project");

    shared().dock->setVisible(true);
    shared().dock->setWindowTitle(m_path);
    shared().sceneTabs->removeTab(shared().sceneTabs->indexOf(shared().homepage));

    CGraphicsView *view = new CGraphicsView(new CGraphicsScene("startup"), this);
    m_sceneModel->AddItem(view);

    shared().sceneTabs->addTab(view, view->cScene()->name());
    shared().sceneTabs->setCurrentWidget(view);
    view->centerOn(view->scene()->sceneRect().center());
}

void CProjectView::CloseProject()
{
    m_name->setText("");
    m_path = "";

    for(CGraphicsView *view : m_sceneModel->views())
    {
        QList<CBubble *> bubbles = view->cScene()->bubbles();

        for(CBubble *bbl : bubbles)
            delete bbl;
    }

    while(m_sceneModel->rowCount())
        m_sceneModel->RemoveItem(0);

    shared().dock->hide();
    shared().showHomepageAction->trigger();
}

void CProjectView::ExportChoiceScript()
{
    QFile file("C:/Development/QT/Chronicler Saves/startup.txt");
    QList<CBubble *> processed;

    CStartBubble *start_bubble = m_sceneModel->views().first()->cScene()->startBubble();

    QString cs = BubbleToChoiceScript(processed, 0, start_bubble);

    QList<CBubble *> bubbles = m_sceneModel->views().first()->cScene()->bubbles();
    for(CBubble *bbl : bubbles)
    {
        if(!processed.contains(bbl) && bbl->connections().length() > 0)
            cs += BubbleToChoiceScript(processed, 0, bbl);
    }


    file.open(QIODevice::WriteOnly);
    file.write(cs.toStdString().c_str());
    file.close();
}

QList<CGraphicsView *> CProjectView::views()
{
    return m_sceneModel->views();
}

CBubble *CProjectView::BubbleWithUID(uint uid)
{
    CBubble *bubble = 0;

    for(CGraphicsView *view : m_sceneModel->views())
    {
        for(QGraphicsItem *item : view->items())
        {
            if((bubble = dynamic_cast<CBubble *>(item)) && (bubble->UID() == uid))
                return bubble;
        }
    }

    return 0;
}

QString CProjectView::BubbleToChoiceScript(QList<CBubble *> &processed, int indent_level, CBubble *bubble)
{
    processed.prepend(bubble);

    QString cs;
    QString indent;

    QList<CConnection *> connections = bubble->connections();

    // TODO add ignored list of bubbles with no connections...

    // figure out if we need a label or not
    bool make_label = connections.length() > 1;
    if(!make_label && connections.length() > 0)
    {
        // if the only connecting bubble hasn't been processed yet...
        CBubble *from = connections.first()->from()->container();
        if(!processed.contains(from))//from->getOrder() > bubble->getOrder())
            make_label = true;
    }

    if(make_label)
        cs +=  "\n*label bubble_" + QString::number(bubble->UID());
    else
        indent = QString("    ").repeated(indent_level);

    if(bubble->getType() == Chronicler::Start)
    {
        CStartBubble *start = dynamic_cast<CStartBubble *>(bubble);
        if(start->link())
            cs += BubbleToChoiceScript(processed, indent_level, start->link()->to());
    }
    else if(bubble->getType() == Chronicler::Story)
    {
        CStoryBubble *story = dynamic_cast<CStoryBubble *>(bubble);

        cs += "\n" + indent + story->getStory().replace("\n", "\n" + indent) + "\n";

        if(story->link() && processed.contains(story->link()->to()))
            cs += indent + "*goto " + "bubble_" + QString::number(story->link()->to()->UID());
        else if(story->link())
            cs += BubbleToChoiceScript(processed, indent_level, story->link()->to());
    }
    else if(bubble->getType() == Chronicler::Choice)
    {

    }
    else if(bubble->getType() == Chronicler::Action)
    {

    }
    else if(bubble->getType() == Chronicler::Condition)
    {

    }

    return cs;
}



void CProjectView::SelectedChanged(QModelIndex current)
{
    if(current.row() >= 0 && current.row() < m_modelView->model()->rowCount())
    {
        m_upButton->setEnabled(true);
        m_downButton->setEnabled(true);
        m_removeButton->setEnabled(true);

        CGraphicsView *view = m_sceneModel->views()[current.row()];

        if(shared().sceneTabs->indexOf(view) == -1)
            shared().sceneTabs->addTab(view, view->cScene()->name());
        shared().sceneTabs->setCurrentWidget(view);
    }
    else
    {
        m_upButton->setEnabled(false);
        m_downButton->setEnabled(false);
        m_removeButton->setEnabled(false);
    }
}

void CProjectView::DataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    for(int i = topLeft.row(); i <= bottomRight.row(); ++i)
    {
        CGraphicsView *view = m_sceneModel->views()[i];
        int tab = shared().sceneTabs->indexOf(view);
        if(tab != -1)
            shared().sceneTabs->setTabText(tab, view->cScene()->name());
    }
}

void CProjectView::ProjectNameChanged()
{
    shared().mainWindow->setWindowTitle("Chronicler - " + m_name->text());
}

void CProjectView::MoveUp()
{
    m_sceneModel->MoveUp(m_modelView->currentIndex().row());
}

void CProjectView::MoveDown()
{
    m_sceneModel->MoveDown(m_modelView->currentIndex().row());
}

void CProjectView::AddItem()
{
    m_sceneModel->AddItem(new CGraphicsView(new CGraphicsScene("Scene " + QString().setNum(m_sceneModel->rowCount())), this));
    m_modelView->edit(QModelIndex(m_sceneModel->index(m_sceneModel->rowCount() - 1, 0)));
}

void CProjectView::RemoveItem()
{
    if(m_modelView->currentIndex().row() > 0)
        m_sceneModel->RemoveItem(m_modelView->currentIndex().row());
}
