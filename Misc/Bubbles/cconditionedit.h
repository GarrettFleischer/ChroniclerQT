#ifndef CCONDITIONEDIT_H
#define CCONDITIONEDIT_H

#include "Misc/clineedit.h"

class CConditionEdit : public CLineEdit
{
    Q_OBJECT

public:
    CConditionEdit(QWidget *parent);

private:
    void UpdateVariables();

    QStringList m_actions;
    QStringList m_variables;

private slots:
    void UpdateCompletionModel();
};

#endif // CCONDITIONEDIT_H
