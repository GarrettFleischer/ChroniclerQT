#include "cchoiceedit.h"

#include "Properties/cvariablesview.h"
#include "Misc/Variables/cvariablesmodel.h"
#include <Misc/Variables/cvariable.h>

#include "Misc/chronicler.h"
using Chronicler::shared;


CChoiceEdit::CChoiceEdit(QWidget *parent)
    : CLineEdit(parent)
{
    setAlwaysEnabled(true);

    // insert actions alphabetically
    m_conditions.append({"*if () #", "*selectable_if () #" });

    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(UpdateCompletionModel()));
}

void CChoiceEdit::UpdateVariables()
{
    m_variables.clear();

    m_variables.append({"choice_purchase_supported", "choice_purchased_adfree", "choice_is_web"});

    for(const CVariable &v : shared().variablesView->model()->variables())
        m_variables.append(v.name());
}

void CChoiceEdit::UpdateCompletionModel()
{
    const int cursorIndex = toPlainText().mid(0, textCursor().position()).count(" ");

    if(cursorIndex == 0)
    {
        m_completionModel->setStringList(m_conditions);
    }
    else
    {
        UpdateVariables();
        m_completionModel->setStringList(m_variables);
    }
}
