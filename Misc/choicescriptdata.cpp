#include "choicescriptdata.h"

#include <QFile>
#include <QTextStream>
#include <QStack>

#include <QFile>
#include <QFileInfo>
#include <QDir>

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
#include "Misc/Bubbles/cchoicemodel.h"
#include "Bubbles/cactionbubble.h"
#include "Misc/cstringlistmodel.h"
#include "Bubbles/cconditionbubble.h"
#include "Connections/cconnection.h"

#include "Properties/cprojectview.h"


#include "Misc/chronicler.h"
using Chronicler::shared;


ChoiceScriptData::ChoiceScriptData(QFile &startup, const CSIndent &csindent)
{
    m_currentScene = "startup";
    QList<CSBlock> blocks = ProcessFile(startup, csindent);

    QString dir = QFileInfo(startup).absoluteDir().absolutePath();
    if(!dir.endsWith('/'))
        dir += "/";

    for(const CSBlock &block : blocks)
    {
        if(block.type == SceneList)
        {
            QStringList scenes = block.text.split('\n', QString::SkipEmptyParts);
            scenes.removeOne("startup");

            for(const QString &scene : scenes)
            {
                QFile file(dir + scene + ".txt");
                file.open(QIODevice::ReadOnly);
                if(file.isOpen())
                {
                    m_currentScene = scene;
                    ProcessFile(file, csindent);
                    file.close();
                }
            }

            break;
        }
    }
}

ChoiceScriptData::ChoiceScriptData(const QString &fileName, const Chronicler::CSIndent &csindent)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    if(file.isOpen())
    {
        m_currentScene = shared().projectView->model()->uniqueName(QFileInfo(file).completeBaseName());
        ProcessFile(file, csindent);
        file.close();
    }
}

QList<CGraphicsView *> ChoiceScriptData::getViews()
{
    return m_views;
}

QList<CVariable> ChoiceScriptData::getVariables()
{
    QList<CVariable> variables;

    for(const CSVariable &v : m_variables)
        variables.append(CVariable(v.name, v.data, shared().projectView->model()->sceneWithName(v.scene)));

    return variables;
}

const QString ChoiceScriptData::getTitle() const
{
    return m_title;
}

const QString ChoiceScriptData::getAuthor() const
{
    return m_author;
}

QList<ChoiceScriptData::CSBlock> ChoiceScriptData::ProcessFile(QFile &file, const CSIndent &csindent)
{
    QString name = shared().projectView->model()->uniqueName(QFileInfo(file.fileName()).completeBaseName());
    CGraphicsView *view = new CGraphicsView(new CGraphicsScene(true, name));

    QTextStream stream(&file);

    QList<CSLine> lines = CSProcLines(stream, csindent);
    QList<CSBlock> blocks = CSProcBlocks(lines);

    QList<CSBubble> deferred = CSProcBubbles(blocks, view->cScene());
    CSLinkBubbles(deferred, view->cScene());

    m_views.append(view);

    return blocks;
}


QString ChoiceScriptData::CSStripIndent(const QString &line, const CSIndent &csindent)
{
    if(line.length() && line.simplified().remove(" ").length())
    {
        int i = 0;
        while (i < line.length() && line[i] == csindent.type)
            ++i;

        return line.mid(i);
    }
    else
        return "";
}

quint8 ChoiceScriptData::CSIndentLevel(const QString &line, const CSIndent &csindent)
{
    quint8 level = 0;

    quint8 i = 0;
    while (i < line.length() && line[i] == csindent.type)
        ++i;

    level = i / csindent.count;

    return level;
}

QList<ChoiceScriptData::CSLine> ChoiceScriptData::CSProcLines(QTextStream &stream, const CSIndent &csindent)
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

QList<ChoiceScriptData::CSBlock> ChoiceScriptData::CSProcBlocks(const QList<ChoiceScriptData::CSLine> &lines)
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

ChoiceScriptData::CSBlock ChoiceScriptData::CSProcBlock(const QList<ChoiceScriptData::CSLine> &lines, int &index)
{
    CSBlock csblock;
    const CSLine &csline = lines[index];

    // Empty
    if(csline.type == Empty || csline.type == Create || csline.type == Temp)
    {
        csblock.width = 0;
        csblock.height = 0;
        ++index;

        if(csline.type == Create || csline.type == Temp)
        {
            QStringList data = csline.line.split(" ");
            m_variables.append(CSVariable(data.first(), (data.length() > 1 ? data[1] : ""), (csline.type == Temp ? m_currentScene : "")));
        }
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
            while (++index < lines.length() && (lines[index].type == Action || lines[index].type == Empty))
                csblock.text += ((lines[index].type == Action) ? "\n" + lines[index].line : "");
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

            csblock.text = csline.line;

            while (index < lines.length() && (lines[index].indent == csline.indent + 1 || lines[index].type == Empty))
            {
                CSBlock child = CSProcBlock(lines, index);
                csblock.width += child.width;
                csblock.AddChild(child);
            }
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

QList<ChoiceScriptData::CSBubble> ChoiceScriptData::CSProcBubbles(const QList<ChoiceScriptData::CSBlock> &blocks, CGraphicsScene *scene)
{
    QList<CSBubble> deferred;
    CBubble *prev = scene->startBubble();
    int row = 1;
    int column = 0;
    for(int i = 0; i < blocks.length(); ++i)
        prev = CSProcBubble(blocks[i], deferred, scene, row += blocks[qMax(i - 1, 0)].height, column, prev);

    return deferred;
}

CBubble * ChoiceScriptData::CSProcBubble(const CSBlock &csblock, QList<CSBubble> &deferredLinks, CGraphicsScene *scene, int row, int column, CBubble *prev)
{
    const static int column_width = 225;
    const static int row_height = 225;

    CBubble *bubble = Q_NULLPTR;
    QPointF pos(column * column_width, row * row_height);

    // Story
    if(csblock.type == Text)
    {
        CStoryBubble *bbl = dynamic_cast<CStoryBubble *>(scene->AddBubble(Chronicler::Story, pos, false));
        bbl->setStory(csblock.text);

        bubble = bbl;
    }

    // Action
    else if(csblock.type == Action)
    {
        CActionBubble *bbl = dynamic_cast<CActionBubble *>(scene->AddBubble(Chronicler::Action, pos, false));

        QStringList actions = csblock.text.split('\n');
        bbl->actions()->setStringList(actions);

        bubble = bbl;
    }

    // Choice
    else if(csblock.type == ChoiceAction)
    {
        CChoiceBubble *bbl = dynamic_cast<CChoiceBubble *>(scene->AddBubble(Chronicler::Choice, pos, false));

        int left_width = 0;
        for(int i = 0; i < csblock.children.length() / 2; ++i)
            left_width += csblock.children[i].width;

        const int left_start = column - left_width;
        const int right_start = column + csblock.children[csblock.children.length() / 2].width;
        int child_left_offset = 0;
        int child_right_offset = 0;
        for(int i = 0; i < csblock.children.length(); ++i)
        {
            const CSBlock &choice = csblock.children[i];
            CChoice *cschoice = new CChoice(bbl->getPalette(), bbl->getFont(), bbl, choice.text);
            bbl->choices()->AddItem(cschoice);

            bool left = (i < csblock.children.length() / 2);
            if(left && i > 0)
                child_left_offset += qMax(1, csblock.children[i - 1].width / 2 + csblock.children[i].width / 2);
            else if (i > csblock.children.length() / 2)
                child_right_offset += qMax(1, csblock.children[i - 1].width / 2 + csblock.children[i].width / 2);


            int child_y_offset = 1;

            CBubble *prev_child = Q_NULLPTR;
            for(int j = 0; j < csblock.children[i].children.length(); ++j)
            {
                const CSBlock &child = csblock.children[i].children[j];

                int ncol = (left ? (left_start + child_left_offset) : (right_start + child_right_offset));
                int nrow = row + child_y_offset;
                child_y_offset += child.height;

                CBubble *next_child = Q_NULLPTR;

                if(!prev_child && child.type == GoTo)
                {
                    //                    next_child = scene->AddBubble(Chronicler::Action, pos, false);
                    CSBubble csbubble;
                    csbubble.bubble = cschoice;
                    csbubble.link = child.text;
                    csbubble.anchor = left ? Chronicler::Left : Chronicler::Right;
                    deferredLinks.append(csbubble);
                }
                else
                    next_child = CSProcBubble(child, deferredLinks, scene, nrow, ncol, prev_child);

                if(!prev_child && next_child)
                    scene->AddConnection(cschoice, next_child, left ? Chronicler::Left : Chronicler::Right, Chronicler::Up);

                prev_child = next_child;
            }
        }

        bubble = bbl;
    }

    // if / elseif
    else if(csblock.type == If || csblock.type == ElseIf)
    {
        if(csblock.type == ElseIf)
            pos.rx() += (column += qMax(1, csblock.width)) * column_width;

        CConditionBubble *bbl = dynamic_cast<CConditionBubble *>(scene->AddBubble(Chronicler::Condition, pos, false));
        bbl->setCondition(csblock.text);

        int ncol = column - qMax(1, csblock.width / 2);
        int nrow = row + 1;

        CBubble *prev_child = Q_NULLPTR;
        for(int i = 0; i < csblock.children.length(); ++i)
        {
            const CSBlock &child = csblock.children[i];

            CBubble *next_child = Q_NULLPTR;

            if(!prev_child && child.type == GoTo)
            {
                CSBubble csbubble;
                csbubble.bubble = bbl;
                csbubble.link = child.text;
                csbubble.anchor = Chronicler::Left;
                deferredLinks.append(csbubble);
            }
            else
                next_child = CSProcBubble(child, deferredLinks, scene, nrow, ncol, prev_child);

            // link true output
            if(!prev_child && next_child)
                scene->AddConnection(bbl, next_child, Chronicler::Left, Chronicler::Up);

            nrow += child.height;
            prev_child = next_child;
        }

        bubble = bbl;
    }

    // Else
    else if(csblock.type == Else)
    {
        column = prev->scenePos().x() / column_width + qMax(1, csblock.width / 2);

        CBubble *prev_child = Q_NULLPTR;
        for(int i = 0; i < csblock.children.length(); ++i)
        {
            const CSBlock &child = csblock.children[i];

            CBubble *next_child = Q_NULLPTR;

            if(!prev_child && child.type == GoTo)
            {
                CSBubble csbubble;
                csbubble.bubble = prev;
                csbubble.link = child.text;
                csbubble.anchor = Chronicler::Right;
                deferredLinks.append(csbubble);
            }
            else
                next_child = CSProcBubble(child, deferredLinks, scene, row, column, prev_child);

            row += child.height;
            prev_child = next_child;
        }

        bubble = prev_child;
    }

    // Goto
    else if(csblock.type == GoTo)
    {
        CSBubble csbubble;
        csbubble.bubble = prev;
        csbubble.link = csblock.text;
        csbubble.anchor = Chronicler::Down;
        deferredLinks.append(csbubble);
    }

    // Title
    else if(csblock.type == Title)
        m_title = csblock.text;

    // Author
    else if(csblock.type == Author)
        m_author = csblock.text;

    if(bubble)
    {
        if(csblock.label.length())
            bubble->setLabel(csblock.label);

        // TODO ensure that the last Action bubble doesn't contain a *goto
        if(prev != bubble && prev)
        {
            if(prev->getType() != Chronicler::Choice && prev->getType() != Chronicler::Condition && prev->getType() != Chronicler::Action)
                scene->AddConnection(prev, bubble, Chronicler::Down, Chronicler::Up);
            else if (prev->getType() == Chronicler::Condition)
                scene->AddConnection(prev, bubble, Chronicler::Right, Chronicler::Up);
            else if(prev->getType() == Chronicler::Action)
            {
                bool has_goto = false;
                for(const QString &str : dynamic_cast<CActionBubble *>(prev)->actions()->stringList())
                {
                    if(str.contains("*goto "))
                    {
                        has_goto = true;
                        break;
                    }
                }

                if(!has_goto)
                    scene->AddConnection(prev, bubble, Chronicler::Down, Chronicler::Up);
            }
        }
    }

    return (bubble ? bubble : prev);
}

void ChoiceScriptData::CSLinkBubbles(QList<ChoiceScriptData::CSBubble> &csbubbles, CGraphicsScene *scene)
{
    for(CSBubble csbubble : csbubbles)
    {
        if(csbubble.bubble)
        {
            CBubble *to = CSBubbleWithLabel(scene, csbubble.link);
            if(to)
                scene->AddConnection(csbubble.bubble, to, csbubble.anchor, Chronicler::Up);
        }
    }
}

CBubble *ChoiceScriptData::CSBubbleWithLabel(CGraphicsScene *scene, const QString &label)
{
    for(CBubble *bubble : scene->bubbles())
    {
        if(bubble->getLabel() == label)
            return bubble;
    }

    return Q_NULLPTR;
}
