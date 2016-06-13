#include "cprojectview.h"

#include <QFileInfo>

#include <QListView>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

#include <QMessageBox>

#include <QDockWidget>
#include <QTabWidget>

#include <QStatusBar>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QFile>
#include <QSaveFile>
#include <QFileDialog>
#include <QTextStream>
#include <QAction>

#include <QStack>

#include <QSettings>

#include <QtAlgorithms>
#include <QtMath>

#include <QTimer>

#include "Bubbles/cchoice.h"
#include "Misc/cscenemodel.h"
#include "cgraphicsscene.h"
#include "cgraphicsview.h"
#include "chomepage.h"
#include "cmainwindow.h"

#include "Bubbles/cbubble.h"
#include "Bubbles/cstartbubble.h"
#include "Bubbles/cstorybubble.h"
#include "Bubbles/cchoicebubble.h"
#include "Misc/cchoicemodel.h"
#include "Bubbles/cactionbubble.h"
#include "Misc/cstringlistmodel.h"
#include "Bubbles/cconditionbubble.h"
#include "Connections/cconnection.h"

#include "Misc/cpalettebutton.h"

#include "csettingsview.h"

#include "Misc/chronicler.h"
using Chronicler::shared;


#include <QDebug>


Q_DECLARE_METATYPE(QStringList)


CProjectView::CProjectView(QWidget *parent)
    : QWidget(parent), m_version(shared().ProgramVersion), m_path("")
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

    m_autosave_num = 0;

    QTimer::singleShot(shared().settingsView->autosaveInterval(), this, SLOT(Autosave()));
}

void CProjectView::SaveProject()
{
    if(m_sceneModel->rowCount() > 0)
    {
        if(!m_path.length())
            SaveProjectAs();
        else
        {
            // Create necessary directories
            QString dir = QFileInfo(m_path).absolutePath();

            if(!QDir(dir + "/backups").exists())
                QDir().mkdir(dir + "/backups");

            if(!QDir(dir + "/scenes").exists())
                QDir().mkdir(dir + "/scenes");

            // Update order before saving...
            ExportChoiceScript();

            QSaveFile file(m_path);
            SaveToFile(file);

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
                    return;
                }
                else if(ret == QMessageBox::Cancel)
                    return;
            }

            QStringList recent_files = shared().settingsView->settings()->value("Homepage/RecentFiles").value<QStringList>();
            if(!recent_files.contains(m_path))
            {
                recent_files.insert(0, m_path);
                if(recent_files.length() > shared().settingsView->maxRecentFiles())
                    recent_files.removeLast();
            }

            shared().settingsView->settings()->setValue("Homepage/RecentFiles", QVariant::fromValue(recent_files));

            // show status message for 10 seconds
            shared().statusBar->showMessage("Successfully saved to " + m_path + "...", 30000);
        }
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
        shared().dock->setWindowTitle(m_path);
        SaveProject();
    }
}

void CProjectView::Autosave()
{
    if(shared().settingsView->maxAutosaves() > 0 && m_sceneModel->rowCount() > 0 && m_path.length())
    {
        QString filename = QFileInfo(m_path).absolutePath() + "/backups/" + QFileInfo(m_path).completeBaseName() +
                ".backup" + QString::number((m_autosave_num = (m_autosave_num % shared().settingsView->maxAutosaves()) + 1)) + ".chronx";
        QSaveFile file(filename);
        SaveToFile(file);
        file.commit();
        // show status message for 30 seconds
        shared().statusBar->showMessage("Autosave complete...", 30000);
    }

    QTimer::singleShot(shared().settingsView->autosaveInterval(), this, SLOT(Autosave()));
}

void CProjectView::SaveToFile(QSaveFile &file)
{
    QByteArray ba;
    QDataStream ds(&ba, QIODevice::WriteOnly);

    ds << shared().ProgramVersion << m_name->text() << *(shared().paletteButton) << m_sceneModel->rowCount();
    for(CGraphicsView *view : m_sceneModel->views())
        ds << *(view->cScene());

    file.open(QIODevice::WriteOnly);
    file.write(ba);
}

void CProjectView::OpenProject(const QString &filepath)
{
    CloseProject();

    QFile file(filepath);
    while(!file.open(QIODevice::ReadOnly))
    {
        QString dir = QFileInfo(filepath).absolutePath();
        file.setFileName(QFileDialog::getOpenFileName(this, "Open", dir, "chronx (*.chronx)"));
        if(!file.fileName().length())
            return;
    }

    m_path = file.fileName();

    // Load data from the file
    QDataStream ds(file.readAll());

    int num_scenes;
    QString project_name;
    ds >> m_version;
    if(m_version == "0.8.1.0")
        ds >> project_name >> num_scenes;
    else
        ds >> project_name >> *(shared().paletteButton) >> num_scenes;

    m_name->setText(project_name);

    // load other scenes
    for(int i = 0; i < num_scenes; ++i)
    {
        QString name;
        ds >> name;
        CGraphicsView *view = new CGraphicsView(new CGraphicsScene(false, name), this);
        view->hide();
        m_sceneModel->AddItem(view);
        ds >> *(m_sceneModel->views().last()->cScene());
    }

    shared().dock->setVisible(true);
    shared().dock->setWindowTitle(m_path);//(QFileInfo(m_path).fileName());
    shared().sceneTabs->removeTab(shared().sceneTabs->indexOf(shared().homepage));

    // grab the startup view
    CGraphicsView *view = m_sceneModel->views().first();
    shared().sceneTabs->addTab(view, view->cScene()->name());
    shared().sceneTabs->setCurrentWidget(view);
    view->centerOn(view->cScene()->startBubble()->scenePos());

    // show status message for 30 seconds
    shared().statusBar->showMessage("Successfully opened " + m_path, 30000);
}

QString CProjectView::CSStripIndent(const QString &line, const CSIndent &csindent)
{
    if(line.length())
    {
        int i = 0;
        while (line[i] == csindent.type)
            ++i;

        return line.mid(i);
    }
    else
        return "";
}

quint8 CProjectView::CSIndentLevel(const QString &line, const CSIndent &csindent)
{
    quint8 level = 0;

    if(line.length())
    {
        quint8 i = 0;
        while (line[i] == csindent.type)
            ++i;

        level = i / csindent.count;
    }
    return level;
}

QList<CProjectView::CSLine> CProjectView::CSProcLines(QTextStream &stream, const CSIndent &csindent)
{
    QList<CSLine> lines;
    QStack<CSLine *> choices;

    while(!stream.atEnd())
    {
        lines.append(CSLine(stream.readLine()));
        CSLine &csline = lines.last();

        csline.indent = CSIndentLevel(csline.line, csindent);
        csline.line = CSStripIndent(csline.line, csindent);

        if (csline.line.startsWith("*choice", Qt::CaseInsensitive))
        {
            csline.type = ChoiceAction;
            choices.push(&csline);
        }

        else if (csline.line.startsWith("*fake_choice", Qt::CaseInsensitive))
        {
            csline.type = FakeChoice;
            choices.push(&csline);
        }

        else if (csline.line.startsWith("#"))
        {
            csline.type = Choice;
            csline.line.remove('#');

            if(csline.indent <= choices.top()->indent)
                choices.pop();

            choices.top()->children.append(&csline);
        }

        else if (csline.line.startsWith("*title", Qt::CaseInsensitive))
        {
            csline.type = Title;
            csline.line.remove("*title ", Qt::CaseInsensitive);
        }

        else if (csline.line.startsWith("*author", Qt::CaseInsensitive))
        {
            csline.type = Author;
            csline.line.remove("*author ", Qt::CaseInsensitive);
        }

        else if (csline.line.startsWith("*create", Qt::CaseInsensitive))
        {
            csline.type = Create;
            csline.line.remove("*create ", Qt::CaseInsensitive);
        }

        else if (csline.line.startsWith("*temp", Qt::CaseInsensitive))
        {
            csline.type = Temp;
            csline.line.remove("*temp ", Qt::CaseInsensitive);
        }

        else if (csline.line.startsWith("*scene_list", Qt::CaseInsensitive))
            csline.type = SceneList;

        else if (csline.line.startsWith("*stat_chart", Qt::CaseInsensitive))
            csline.type = StatChart;

        else if (csline.line.startsWith("*if", Qt::CaseInsensitive))
        {
            csline.type = If;
            csline.line.remove("*if", Qt::CaseInsensitive);
        }

        else if (csline.line.startsWith("*elseif", Qt::CaseInsensitive))
        {
            csline.type = ElseIf;
            csline.line.remove("*elseif", Qt::CaseInsensitive);
        }

        else if (csline.line.startsWith("*else", Qt::CaseInsensitive))
            csline.type = Else;

        else if (csline.line.startsWith("*label", Qt::CaseInsensitive))
        {
            csline.type = Label;
            csline.line.remove("*label", Qt::CaseInsensitive).remove(' ');
        }

        else if (csline.line.startsWith("*finish", Qt::CaseInsensitive))
            csline.type = Finish;

        else if (csline.line.toLower().startsWith("*goto"))
        {
            csline.type = GoTo;
            csline.line.remove("*goto", Qt::CaseInsensitive).remove(' ');
        }

        else if (csline.line.startsWith("*"))
            csline.type = Action;

        else if (csline.line.length())
            csline.type = Text;
    }

    return lines;
}

QList<CProjectView::CSBlock> CProjectView::CSProcBlocks(const QList<CProjectView::CSLine> &lines)
{
    QList<CSBlock> blocks;

    CSBlock block;
    for (int i = 0; i < lines.length();)
    {
        block = CSProcBlock(lines, i);
        if(block.type != Empty)
            blocks.append(block);
    }

    return blocks;
}

CProjectView::CSBlock CProjectView::CSProcBlock(const QList<CProjectView::CSLine> &lines, int &index)
{
    CSBlock csblock;
    const CSLine &csline = lines[index];

    if(csline.type == Empty)
    {
        csblock.width = 0;
        csblock.height = 0;
        ++index;
    }
    else
    {
        csblock.type = csline.type;

        // Label
        if(csline.type == Label)
        {
            csblock = CSProcBlock(lines, ++index);
            csblock.label = csline.line;
        }

        // STORY
        else if(csline.type == Text || csline.type == StatChart)
        {
            csblock.text = csline.line;
            while (++index < lines.length() && (lines[index].type == Text || lines[index].type == Empty || lines[index].type == StatChart))
                csblock.text += "\n" + lines[index].line;
        }

        // ACTION
        else if(csline.type == Action)
        {
            csblock.text = csline.line;
            while (++index < lines.length() && lines[index].type == Action)
                csblock.text += "\n" + lines[index].line;
        }

        // SCENE_LIST
        else if(csline.type == SceneList)
        {
            while (++index < lines.length() && (lines[index].indent == csline.indent + 1))
                csblock.text += (csblock.text.length() ? "\n" : "") + lines[index].line;
            csblock.height = 0;
        }

        // CONDITION
        else if(csline.type == If || csline.type == ElseIf || csline.type == Else)
        {
            ++index;

            while (lines[index].indent == csline.indent + 1 || lines[index].type == Empty)
            {
                CSBlock child = CSProcBlock(lines, index);
                csblock.width = (child.width > csblock.width) ? child.width : csblock.width;
                csblock.height += child.height;
                csblock.AddChild(child);
            }

            //            for (const CSLine *condition : csline.children)
            //            {
            //                int choice_index = lines.indexOf(*condition, index);
            //                CSBlock child = CSProcBlock(lines, choice_index);
            //                csblock.width += child.width;
            //                csblock.height = (child.height > csblock.height) ? child.height : csblock.height;
            //                csblock.AddChild(child);
            //                index = choice_index;
            //            }
        }

        // CHOICE_ACTION
        else if(csline.type == ChoiceAction)
        {
            ++index;
            for (const CSLine *choice : csline.children)
            {
                int choice_index = lines.indexOf(*choice, index);
                CSBlock child = CSProcBlock(lines, choice_index);
                csblock.width += child.width;
                csblock.height = (child.height > csblock.height) ? child.height : csblock.height;
                csblock.AddChild(child);
                index = choice_index;
            }
            ++csblock.height;
        }

        // FAKE_CHOICE
        else if(csline.type == FakeChoice)
        {
            ++index;
            if(csline.children.length())
            {
                for (const CSLine *choice : csline.children)
                    csblock.text += (csblock.text.length() ? "\n" : "") + choice->line;

                index = lines.indexOf(*csline.children.last(), index) + 1;
            }
        }

        // CHOICE
        else if(csline.type == Choice)
        {
            csblock.text = csline.line;
            ++index;
            while (lines[index].indent == csline.indent + 1 || lines[index].type == Empty)
            {
                CSBlock child = CSProcBlock(lines, index);
                csblock.width = (child.width > csblock.width) ? child.width : csblock.width;
                csblock.height += child.height;
                csblock.AddChild(child);
            }
        }

        // ANYTHING ELSE
        else
        {
            csblock.text = csline.line;
            csblock.height = 0;
            ++index;
        }
    }

    return csblock;
}

QList<CProjectView::CSBubble> CProjectView::CSProcBubbles(const QList<CProjectView::CSBlock> &blocks, CGraphicsScene *scene)
{
    QList<CSBubble> deferred;
    CBubble *prev = scene->startBubble();
    int row = 1;
    for(int i = 0; i < blocks.length(); ++i)
        prev = CSProcBubble(blocks[i], deferred, scene, row += blocks[qMax(i - 1, 0)].height, 0, prev);

    return deferred;
}

CBubble * CProjectView::CSProcBubble(const CSBlock &csblock, QList<CSBubble> &deferredLinks, CGraphicsScene *scene, int row, int column, CBubble *prev)
{
    const static int cw = 225;
    const static int rh = 225;

    CBubble *bubble = Q_NULLPTR;
    QPointF pos(column * cw, row * rh);

    if(csblock.type == Text)
    {
        CStoryBubble *bbl = dynamic_cast<CStoryBubble *>(scene->AddBubble(Chronicler::Story, pos, false));
        bbl->setStory(csblock.text);

        bubble = bbl;
    }

    else if(csblock.type == Action)
    {
        CActionBubble *bbl = dynamic_cast<CActionBubble *>(scene->AddBubble(Chronicler::Action, pos, false));

        QStringList actions = csblock.text.split('\n');
        for(QString action : actions)
            bbl->actions()->AddItem(action);

        bubble = bbl;
    }

    else if(csblock.type == ChoiceAction)
    {
        CChoiceBubble *bbl = dynamic_cast<CChoiceBubble *>(scene->AddBubble(Chronicler::Choice, pos, false));

        const qreal half = qreal(csblock.width) / 2.0;
        const int start_col = column - half + qCeil(half - int(half));// - csblock.children.first().width;
        int child_x_offset = 0;
        bool first_right = true;
        for(int i = 0; i < csblock.children.length(); ++i)
        {
            const CSBlock &choice = csblock.children[i];
            CChoice *cschoice = new CChoice(bbl->getPalette(), bbl->getFont(), bbl, choice.text);
            bbl->choices()->AddItem(cschoice);

            bool left = (i < csblock.children.length() / 2);
            if(left && i > 0)
                child_x_offset += csblock.children[i - 1].width;
            else if (i > 0)
            {
                if(first_right)
                {
                    child_x_offset += csblock.children[i].width;
                    first_right = false;
                }
                else
                    child_x_offset += csblock.children[i - 1].width + csblock.children[i].width;
            }


            int child_y_offset = 1;

            CBubble *prev_child = Q_NULLPTR;
            for(int j = 0; j < csblock.children[i].children.length(); ++j)
            {
                const CSBlock &child = csblock.children[i].children[j];

                // TODO use left_col / right_col
                int ncol = start_col + child_x_offset;
                int nrow = row + child_y_offset;
                child_y_offset += child.height;
                if(ncol == column)
                    ncol += left ? -child.width : child.width;

                CBubble *next_child = CSProcBubble(child, deferredLinks, scene, nrow, ncol, prev_child);

                if(!prev_child && next_child)
                    scene->AddConnection(cschoice, next_child, left ? Chronicler::Left : Chronicler::Right, Chronicler::Up);

                prev_child = next_child;
            }
        }

        bubble = bbl;
    }

    else if(csblock.type == If || csblock.type == ElseIf)
    {
        CConditionBubble *bbl = dynamic_cast<CConditionBubble *>(scene->AddBubble(Chronicler::Condition, pos, false));
        bbl->setCondition(csblock.text);

        int ncol = column - csblock.width / 2;
        int nrow = row + 1;

        CBubble *prev_child = Q_NULLPTR;
        for(int i = 0; i < csblock.children.length(); ++i)
        {
            const CSBlock &child = csblock.children[i];

            CBubble *next_child = CSProcBubble(child, deferredLinks, scene, nrow, ncol, prev_child);

            // link true output
            if(!prev_child && next_child)
                scene->AddConnection(bbl, next_child, Chronicler::Left, Chronicler::Up);

            nrow += child.height;
            prev_child = next_child;
        }

        bubble = bbl;
    }

    else if(csblock.type == GoTo)
    {
        CSBubble csbubble;
        csbubble.bubble = prev;
        csbubble.link = csblock.text;
        deferredLinks.append(csbubble);
    }

    if(bubble)
    {
        if(csblock.label.length())
            bubble->setLabel(csblock.label);

        if(prev != bubble && prev)
        {
            if(prev->getType() != Chronicler::Choice && prev->getType() != Chronicler::Condition)
                scene->AddConnection(prev, bubble, Chronicler::Down, Chronicler::Up);
            else if (prev->getType() == Chronicler::Condition)
                scene->AddConnection(prev, bubble, Chronicler::Right, Chronicler::Up);
        }

    }

    return (bubble ? bubble : prev);
}

void CProjectView::CSLinkBubbles(QList<CProjectView::CSBubble> &csbubbles, CGraphicsScene *scene)
{
    for(CSBubble csbubble : csbubbles)
    {
        if(csbubble.bubble)
        {
            CBubble *to = CSBubbleWithLabel(scene, csbubble.link);
            if(to)
                scene->AddConnection(csbubble.bubble, to, Chronicler::Down, Chronicler::Up);
        }
    }
}

CBubble *CProjectView::CSBubbleWithLabel(CGraphicsScene *scene, const QString &label)
{
    for(CBubble *bubble : scene->bubbles())
    {
        if(bubble->getLabel() == label)
            return bubble;
    }

    return Q_NULLPTR;
}

void CProjectView::ImportChoiceScript(const QString &filepath)
{
    QString dir = QFileInfo(filepath).absolutePath();
    QFile file(filepath);
    while(!file.open(QIODevice::ReadOnly))
    {
        file.setFileName(QFileDialog::getOpenFileName(this, "Open", dir, "startup (startup.txt)"));
        if(!file.fileName().length())
            return; // quit if user hits cancel
    }

    // TODO: ask for indent type
    CSIndent csindent;
    QTextStream stream(&file);

    QList<CSLine> lines = CSProcLines(stream, csindent);
    QList<CSBlock> blocks = CSProcBlocks(lines);

    CGraphicsView *view = new CGraphicsView(new CGraphicsScene(true, "test"));
    shared().sceneTabs->addTab(view, view->cScene()->name());

    QList<CSBubble> deferred = CSProcBubbles(blocks, view->cScene());
    CSLinkBubbles(deferred, view->cScene());

}

void CProjectView::NewProject()
{
    CloseProject();

    m_name->setText("New Project");

    shared().dock->setVisible(true);
    shared().dock->setWindowTitle(m_path);
    shared().sceneTabs->removeTab(shared().sceneTabs->indexOf(shared().homepage));

    CGraphicsView *view = new CGraphicsView(new CGraphicsScene(true, "startup"), this);
    m_sceneModel->AddItem(view);

    shared().sceneTabs->addTab(view, view->cScene()->name());
    shared().sceneTabs->setCurrentWidget(view);
    view->centerOn(view->cScene()->startBubble()->scenePos());
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


bool SortByOrderAscending(CBubble *first, CBubble *second)
{
    return first->getOrder() < second->getOrder();
}

void CProjectView::ExportChoiceScript()
{
    QString project_folder = QFileInfo(m_path).absolutePath();
    for(CGraphicsView *view : m_sceneModel->views())
    {
        QFile file(project_folder + "/scenes/" + view->cScene()->name() + ".txt");

        QString cs;
        if(view->cScene()->name() == "startup")
        {
            cs += "*title " + m_name->text() + "\n\n";

            cs += "*scene_list\n";
            for(CGraphicsView *v : m_sceneModel->views())
                cs += "    " + v->cScene()->name() + "\n";

            cs += "\n";
        }

        QList<CConnection *> processed_links;
        CalculateOrder(view->cScene()->startBubble()->link(), processed_links, 0);

        QList<CBubble *> bubbles = view->cScene()->bubbles();
        qSort(bubbles.begin(), bubbles.end(), SortByOrderAscending);

        QList<CBubble *> processed;


        for(CBubble *bbl : bubbles)
            cs += BubbleToChoiceScript(bubbles, processed, 0, bbl);

        file.open(QIODevice::WriteOnly);
        file.write(cs.toStdString().c_str());
        file.close();
    }
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
            if((bubble = dynamic_cast<CBubble *>(item)) && (bubble->getUID() == uid))
                return bubble;
        }
    }

    return 0;
}

const QString CProjectView::version() const
{
    return m_version;
}

QString CProjectView::BubbleToChoiceScript(const QList<CBubble *> &bubbles, QList<CBubble *> &processed, int indent_level, CBubble *bubble)
{
    QString cs;

    if(!processed.contains(bubble))
    {
        processed.prepend(bubble);

        QString indent_str = "    ";
        QString indent;

        // generate label...
        if(LabelNeeded(bubble, bubbles))
            cs +=  "\n\n*label " + MakeLabel(bubble, bubbles) + "\n";
        else
            indent = QString(indent_str).repeated(indent_level);

        // ------------ Start bubble ------------
        if(bubble->getType() == Chronicler::Start)
        {
            CStartBubble *start = dynamic_cast<CStartBubble *>(bubble);
            if(start->link())
                cs += BubbleToChoiceScript(bubbles, processed, indent_level, start->link()->to());
            else
                cs += indent + "*finish\n";
        }

        // ------------ Story bubble ------------
        if(bubble->getType() == Chronicler::Story)
        {
            CStoryBubble *story = dynamic_cast<CStoryBubble *>(bubble);

            cs += indent + story->getStory().replace("\n", "\n" + indent) + "\n";

            if(story->link())
            {
                if(LabelNeeded(story->link()->to(), bubbles))
                    cs += indent + "*goto " + MakeLabel(story->link()->to(), bubbles);
                //                if(story->link()->to()->getType() == Chronicler::Story)
                //                    cs += indent + "*page_break";
                else
                    cs += "\n" + BubbleToChoiceScript(bubbles, processed, indent_level, story->link()->to());
            }
            else
                cs += indent + "*finish";
        }

        // ------------ Choice bubble ------------
        else if(bubble->getType() == Chronicler::Choice)
        {
            CChoiceBubble *choice_bubble = dynamic_cast<CChoiceBubble *>(bubble);

            cs += indent + "*choice";

            // increase indent level
            indent = QString(indent_str).repeated(++indent_level);

            for(CChoice *choice : choice_bubble->choiceBubbles())
            {
                // TODO add section for conditional choices
                cs += "\n" + indent + "#" + choice->choice() + "\n";

                if(choice->link() && LabelNeeded(choice->link()->to(), bubbles))
                    cs += indent + indent_str + "*goto " + MakeLabel(choice->link()->to(), bubbles);
                else if(choice->link())
                    cs += "\n" + BubbleToChoiceScript(bubbles, processed, indent_level + 1, choice->link()->to());
                else
                    cs += indent + indent_str + "*finish";

                cs += "\n";
            }
        }

        // ------------ Action bubble ------------
        else if(bubble->getType() == Chronicler::Action)
        {
            CActionBubble *action = dynamic_cast<CActionBubble *>(bubble);

            cs += indent + action->actionString().replace("\n", "\n" + indent) + "\n";

            if(action->link() && LabelNeeded(action->link()->to(), bubbles))
                cs += indent + "*goto " + MakeLabel(action->link()->to(), bubbles);
            else if(action->link())
                cs += "\n" + BubbleToChoiceScript(bubbles, processed, indent_level, action->link()->to());
            else
                cs += indent + "*finish";
        }

        // ------------ Condition bubble ------------
        else if(bubble->getType() == Chronicler::Condition)
        {
            CConditionBubble *cb = dynamic_cast<CConditionBubble *>(bubble);

            cs += indent + "*if(" + cb->getCondition() + ")\n";

            // true
            if(cb->trueLink())
            {
                if(LabelNeeded(cb->trueLink()->to(), bubbles))
                    cs += indent + indent_str + "*goto " + MakeLabel(cb->trueLink()->to(), bubbles);
                else
                    cs += BubbleToChoiceScript(bubbles, processed, indent_level + 1, cb->trueLink()->to());
            }
            else
                cs += indent + indent_str + "*finish";

            cs += "\n" + indent + "*else\n";

            // false
            if(cb->falseLink())
            {
                if(LabelNeeded(cb->falseLink()->to(), bubbles))
                    cs += indent + indent_str + "*goto " + MakeLabel(cb->falseLink()->to(), bubbles);
                else
                    cs += BubbleToChoiceScript(bubbles, processed, indent_level + 1, cb->falseLink()->to());
            }
            else
                cs += indent + indent_str + "*finish";
        }
    }

    return cs;
}

bool CProjectView::LabelNeeded(CBubble *bubble, const QList<CBubble *> &bubbles)
{
    QList<CConnection *> connections = bubble->connections();

    // figure out if we need a label or not
    bool make_label = connections.length() > 1 || bubble->getLocked();
    if(!make_label && connections.length() > 0)
    {
        // if the only connecting bubble hasn't been processed yet...
        CBubble *from = connections.first()->from()->container();
        int diff = (bubbles.indexOf(from) - bubbles.indexOf(bubble));
        if((diff > 1) && !dynamic_cast<CChoiceBubble *>(from))
            make_label = true;
    }

    return make_label;
}

QString CProjectView::MakeLabel(CBubble *bubble, const QList<CBubble *> &bubbles)
{
    QString label = bubble->getLabel().replace(" ", "_");
    if(!label.length())
        label = "bubble_" + QString::number(bubble->GenerateUID());
    else
    {
        for(CBubble *b : bubbles)
        {
            if(b != bubble && LabelNeeded(b, bubbles) && b->getLabel() == bubble->getLabel())
            {
                label += "_" + QString::number(bubble->GenerateUID());
                break;
            }
        }
    }

    return label;
}

void CProjectView::CalculateOrder(CConnection *connection, QList<CConnection *> &processed, qint64 order)
{
    if(connection && !processed.contains(connection))
    {
        // update processed
        processed.prepend(connection);

        // calculate new order for current bubble
        qint64 new_order = order;
        if(connection->to()->getLocked())
            new_order = connection->to()->getOrder();
        else
            connection->to()->setOrder(new_order);

        // recurse
        for(CConnection *c : connection->to()->links())
            CalculateOrder(c, processed, new_order + 1);
    }
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
    // TODO ensure that each scene has a unique name
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
    shared().mainWindow->setWindowTitle("Chronicler " + shared().ProgramVersion + " - " + m_name->text());
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
    m_sceneModel->AddItem(new CGraphicsView(new CGraphicsScene(true, "scene " + QString().setNum(m_sceneModel->rowCount())), this));
    m_modelView->edit(QModelIndex(m_sceneModel->index(m_sceneModel->rowCount() - 1, 0)));
}

void CProjectView::RemoveItem()
{
    if(m_modelView->currentIndex().row() > 0)
        m_sceneModel->RemoveItem(m_modelView->currentIndex().row());
}
