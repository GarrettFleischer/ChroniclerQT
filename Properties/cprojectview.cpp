#include "cprojectview.h"

#include <QFileInfo>

#include <QListView>
#include <QPushButton>
#include <QLabel>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QFile>
#include <QSaveFile>
#include <QFileDialog>
#include <QAction>

#include <QMessageBox>

#include <QDockWidget>
#include <QTabWidget>

#include "Misc/cscenemodel.h"
#include "cgraphicsscene.h"
#include "cgraphicsview.h"
#include "chomepage.h"
#include "cmainwindow.h"

#include "Bubbles/cbubble.h"


#include "Misc/chronicler.h"
using Chronicler::shared;


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

    CGraphicsView *startup = new CGraphicsView(new CGraphicsScene("startup"), this);
    m_sceneModel = new CSceneModel(startup, this);

    m_modelView = new QListView();
    m_modelView->setModel(m_sceneModel);
    m_modelView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    connect(m_modelView, SIGNAL(clicked(QModelIndex)), this, SLOT(SelectedChanged(QModelIndex)));
    connect(m_sceneModel, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(DataChanged(QModelIndex,QModelIndex)));

    QHBoxLayout *hl_viewButtons = new QHBoxLayout();
    hl_viewButtons->addWidget(m_modelView);
    hl_viewButtons->addLayout(vl_buttons);

    QVBoxLayout *l_main = new QVBoxLayout(this);
    l_main->addWidget(new QLabel("Scenes"));
    l_main->addLayout(hl_viewButtons);
}

void CProjectView::Save()
{
    if(!m_path.length())
        SaveAs();
    else
    {
        QByteArray ba;
        QDataStream ds(&ba, QIODevice::WriteOnly);

        ds << currentVersion
           << m_name
           << m_sceneModel->rowCount();
        for(CGraphicsView *view : m_sceneModel->views())
            ds << *(view->cScene());


        QSaveFile file(m_path);
        file.open(QIODevice::WriteOnly);
        file.write(ba);

        while(!file.commit())
        {
            QMessageBox msgBox;
            msgBox.setText("Error writing to file!");
            msgBox.setInformativeText("Ensure that the path is valid and that you have enough disk space.");
            msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Retry | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Retry);
            int ret = msgBox.exec();

            if(ret == QMessageBox::Save)
                SaveAs();
            else if(ret == QMessageBox::Cancel)
                break;
        }
    }
}

void CProjectView::SaveAs()
{
    QString dir = QFileInfo(m_path).absolutePath();
    QFileDialog dialog(this, "Save As", dir, "chronx (*.chronx)");
    dialog.setDefaultSuffix("chronx");
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);

    if(dialog.exec())
    {
        m_path = dialog.selectedFiles().first();
        Save();
    }
}

void CProjectView::Load(QString filepath)
{
    if(!filepath.length())
    {
        QString dir = QFileInfo("C:/Development/QT/Chronicler Saves").absolutePath();
        filepath = QFileDialog::getOpenFileName(this, "Open", dir, "chronx (*.chronx)");
        if(!filepath.length())
            return;
    }

    m_path = filepath;
    shared().mainWindow->setWindowTitle("Chronicler - " + QFileInfo(filepath).baseName());
    shared().dock->setVisible(true);
    shared().dock->setWindowTitle(QFileInfo(filepath).fileName());
    shared().sceneTabs->removeTab(shared().sceneTabs->indexOf(shared().homepage));

    // TODO
    // Load data from file and instantiate all scenes & bubbles...
    QFile file(filepath);
    file.open(QIODevice::ReadOnly);
    QByteArray ba(file.readAll());
    QDataStream ds(&ba, QIODevice::ReadOnly);

    int num_scenes;
    ds >> m_version
       >> m_name
       >> num_scenes;

    // grab the startup view
    CGraphicsView *view = m_sceneModel->views().first();
    ds >> *(view->cScene());

    // load other scenes
    for(int i = 1; i < num_scenes; ++i)
    {

//        m_sceneModel->AddItem(new CGraphicsView(new CGraphicsScene("Scene " + QString().setNum(m_sceneModel->rowCount())), this));
    }



    shared().sceneTabs->addTab(view, view->cScene()->name());
    shared().sceneTabs->setCurrentWidget(view);
    view->centerOn(view->scene()->sceneRect().center());
}

void CProjectView::Import()
{

}

void CProjectView::NewProject()
{

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
    m_sceneModel->RemoveItem(m_modelView->currentIndex().row());
}
