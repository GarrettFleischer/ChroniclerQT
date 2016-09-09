#ifndef CACTIONCOMPLETIONMODEL_H
#define CACTIONCOMPLETIONMODEL_H

#include "Misc/ctextedit.h"

class CCodeEdit : public CTextEdit
{
    Q_OBJECT

public:
    explicit CCodeEdit(QWidget *parent = Q_NULLPTR);

private:
    void UpdateVariables();
    void UpdateLabels(const QString &scene = "");
    void UpdateScenes();

    QStringList m_actions;
    QStringList m_variables;
    QStringList m_labels;
    QStringList m_scenes;

public slots:
    void UpdateCompletionModel();
};

#endif // CACTIONCOMPLETIONMODEL_H
