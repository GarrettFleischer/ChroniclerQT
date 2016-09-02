#include "cconditionedit.h"

#include "Properties/cvariablesview.h"
#include "Misc/Variables/cvariablesmodel.h"
#include "Misc/Variables/cvariable.h"

#include "Misc/cstringlistmodel.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

CConditionEdit::CConditionEdit(QWidget *parent)
    : CLineEdit(parent)
{
    setAlwaysEnabled(true);

    // insert actions alphabetically
    m_actions.append({"=", "!=", ">", "<", ">=", "<=", "and", "or", "not"});

    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(UpdateCompletionModel()));
}

void CConditionEdit::UpdateVariables()
{
    m_variables.clear();

    m_variables.append({"choice_purchase_supported", "choice_purchased_adfree", "choice_is_web"});

    for(const CVariable &v : shared().variablesView->model()->variables())
        m_variables.append(v.name());
}

void CConditionEdit::UpdateCompletionModel()
{
    QStringList words = toPlainText().split(" ");
    const int cursorIndex = toPlainText().mid(0, textCursor().position()).count(" ");

    if(cursorIndex > 0 && !m_actions.contains(words[cursorIndex - 1]))
    {
        m_completionModel->setStringList(m_actions);
    }
    else
    {
        UpdateVariables();
        m_completionModel->setStringList(m_variables);
    }
}
