#include "cactionedit.h"

#include "Properties/cvariablesview.h"
#include "Misc/Variables/cvariablesmodel.h"
#include "Misc/Variables/cvariable.h"

#include "Properties/cprojectview.h"
#include "Misc/cscenemodel.h"
#include "cgraphicsview.h"
#include "cgraphicsscene.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

CActionEdit::CActionEdit(QWidget *parent)
    : CLineEdit(parent)
{
    setAlwaysEnabled(true);

    m_actions.append({"*bug", "*comment", "*ending", "*finish", "*gosub", "*gosub_scene", "*goto", "*goto_scene", "*hide_reuse", "*image", "*input_number", "*input_text", "*label", "*line_break", "*page_break", "*rand", "*return", "*set"});

    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(UpdateCompletionModel()));
}

void CActionEdit::UpdateVariables()
{
    m_variables.clear();

    for(const CVariable &v : shared().variablesView->model()->variables())
        m_variables.append(v.name());
}

void CActionEdit::UpdateLabels(const QString &scene)
{
    if(scene.isEmpty())
    {

    }
}

void CActionEdit::UpdateScenes()
{

}

void CActionEdit::UpdateCompletionModel()
{
    QTextCursor cursor = textCursor();
    QString text = toPlainText();
    QStringList words = text.split(" ");
    const int cursorIndex = text.mid(0, cursor.position()).count(" ");

    m_completionModel->setStringList({});

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

        }

        // fourth word
        else if(cursorIndex == 3)
        {

        }
    }

}
