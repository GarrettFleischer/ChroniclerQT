#ifndef CCHOICEEDIT_H
#define CCHOICEEDIT_H

#include "Misc/clineedit.h"

class CChoiceEdit : public CLineEdit
{
    Q_OBJECT

public:
    CChoiceEdit(QWidget *parent);

private:
    void UpdateVariables();

    QStringList m_conditions;
    QStringList m_variables;

private slots:
    void UpdateCompletionModel();
};

#endif // CCHOICEEDIT_H
