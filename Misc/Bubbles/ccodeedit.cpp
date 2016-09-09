#include "ccodeedit.h"

#include "Misc/cstringlistmodel.h"

#include "Properties/cprojectview.h"
#include "cgraphicsview.h"
#include "cgraphicsscene.h"

#include "Bubbles/cbubble.h"
#include "Bubbles/cactionbubble.h"

#include "Properties/cvariablesview.h"
#include "Misc/Variables/cvariablesmodel.h"
#include "Misc/Variables/cvariable.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

CCodeEdit::CCodeEdit(QWidget *parent)
    : CTextEdit(parent)
{
    setAcceptsTab(true);

    // insert actions alphabetically
    m_actions.append({"*bug", "*choice", "*comment", "*elseif", "*ending", "*fake_choice", "*finish", "*gosub", "*gosub_scene",
                      "*goto", "*goto_scene", "*hide_reuse", "*if", "*image", "*input_number", "*input_text",
                      "*label", "*line_break", "*page_break", "*rand", "*return", "*set"});

    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(UpdateCompletionModel()));
}

void CCodeEdit::UpdateVariables()
{
    m_variables.clear();

    m_variables.append({"choice_purchase_supported", "choice_purchased_adfree", "choice_is_web"});

    for(const CVariable &v : shared().variablesView->model()->variables())
        m_variables.append(v.name());
}

void CCodeEdit::UpdateLabels(const QString &scene)
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

                if(b->getType() == Chronicler::ActionBubble)
                {
                    CActionBubble *ab = static_cast<CActionBubble *>(b);
                    for(QString action : ab->actions()->stringList())
                    {
                        QStringList words = action.split(" ", QString::SkipEmptyParts);
                        if(words.length() > 1 && words[0] == "*label")
                            m_labels.append(words.last());
                    }
                }
            }
        }
    }
}

void CCodeEdit::UpdateScenes()
{
    m_scenes.clear();

    if(shared().projectView)
        for(CGraphicsView *v : shared().projectView->getViews())
            m_scenes.append(v->cScene()->name());
}

void CCodeEdit::UpdateCompletionModel()
{
    UpdateVariables();
    UpdateLabels();
    UpdateScenes();

    QStringList lst;
    lst.append(m_actions);
    lst.append(m_variables);
    lst.append(m_labels);
    lst.append(m_scenes);

    m_completionModel->setStringList(lst);
}
