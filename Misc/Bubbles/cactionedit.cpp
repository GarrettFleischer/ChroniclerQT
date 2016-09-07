#include "cactionedit.h"

#include "Properties/cvariablesview.h"
#include "Misc/Variables/cvariablesmodel.h"
#include "Misc/Variables/cvariable.h"

#include "Properties/cprojectview.h"
#include "Misc/cscenemodel.h"
#include "cgraphicsview.h"
#include "cgraphicsscene.h"

#include "Bubbles/cbubble.h"
#include "Bubbles/cactionbubble.h"

#include "Misc/cstringlistmodel.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

CActionEdit::CActionEdit(QWidget *parent)
    : CLineEdit(parent)
{
    setAlwaysEnabled(true);

    // insert actions alphabetically
    m_actions.append({"*bug", "*comment", "*ending", "*finish", "*gosub", "*gosub_scene",
                      "*goto", "*goto_scene", "*hide_reuse", "*image", "*input_number", "*input_text",
                      "*label", "*line_break", "*page_break", "*rand", "*return", "*set"});

    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(UpdateCompletionModel()));
}

void CActionEdit::UpdateVariables()
{
    m_variables.clear();

    m_variables.append({"choice_purchase_supported", "choice_purchased_adfree", "choice_is_web"});

    for(const CVariable &v : shared().variablesView->model()->variables())
        m_variables.append(v.name());
}

void CActionEdit::UpdateLabels(const QString &scene)
{
    m_labels.clear();

    if(scene.isEmpty())
    {
        CGraphicsView *currentView = dynamic_cast<CGraphicsView *>(shared().sceneTabs->currentWidget());
        if(currentView)
        {
            for(CBubble *b : currentView->cScene()->bubbles())
            {
                if(b->getLabel().length())
                    m_labels.append(b->getLabel());

                if(b->getType() == Chronicler::Action)
                {
                    CActionBubble *ab = static_cast<CActionBubble *>(b);
                    for(QString action : ab->actions()->stringList())
                    {
                        QStringList words = action.split(" ", QString::SkipEmptyParts);
                        if(words.length() > 1 && words[0] == "*label")
                            m_labels.append(words[1]);
                    }
                }
            }
        }
    }
}

void CActionEdit::UpdateScenes()
{
    m_scenes.clear();

    for(CGraphicsView *v : shared().projectView->getViews())
        m_scenes.append(v->cScene()->name());
}

void CActionEdit::UpdateCompletionModel()
{
    QTextCursor cursor = textCursor();
    QString text = toPlainText();
    QStringList words = text.split(" ");
    const int cursorIndex = text.mid(0, cursor.position()).count(" ");

    m_completionModel->setStringList({});

    // first word
    if(cursorIndex == 0)
    {
        m_completionModel->setStringList(m_actions);
    }
    else
    {
        QString action = words[0];

        // second word
        if(cursorIndex == 1)
        {
            // labels
            if(action == "*gosub" || action == "*goto")
            {
                UpdateLabels();
                m_completionModel->setStringList(m_labels);
            }
            // scenes
            else if(action == "*gosub_scene" || action == "*goto_scene")
            {
                UpdateScenes();
                m_completionModel->setStringList(m_scenes);
            }
            // variables
            else if(action == "*input_number" || action == "*input_text" || action == "*rand" || action == "*set")
            {
                UpdateVariables();
                m_completionModel->setStringList(m_variables);
            }
        }

        // third word
        else if(cursorIndex == 2)
        {
            // labels
            if(action == "*gosub_scene")
            {
                UpdateLabels(words[1]);
                m_completionModel->setStringList(m_labels);
            }

            // misc
            else if(action == "*set")
            {
                UpdateVariables();
                m_variables.prepend("%-");
                m_variables.prepend("%+");
                m_variables.prepend("-");
                m_variables.prepend("+");

                m_completionModel->setStringList(m_variables);
            }
        }

        // fourth word
        else if(cursorIndex == 3)
        {
            if(action == "*set")
            {
                UpdateVariables();
                m_completionModel->setStringList(m_variables);
            }
        }
    }

}
