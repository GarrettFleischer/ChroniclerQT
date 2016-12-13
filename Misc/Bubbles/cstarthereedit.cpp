#include "cstarthereedit.h"

#include "Properties/cvariablesview.h"
#include "Misc/Variables/cvariablesmodel.h"
#include "Misc/Variables/cvariable.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

CStartHereEdit::CStartHereEdit(QWidget *parent)
    : CLineEdit(parent)
{
    QStringList variables;
    for(const CVariable &v : shared().variablesView->model()->variables())
        variables.append(v.name());

    m_completionModel->setStringList(variables);
}
