#ifndef CACTIONEDIT_H
#define CACTIONEDIT_H

#include "Misc/clineedit.h"

class CGraphicsScene;

class CActionEdit : public CLineEdit
{
    Q_OBJECT

public:
    CActionEdit(QWidget *parent);

private:
    void UpdateVariables();
    void UpdateLabels(const QString &scene = "");
    void AddLabelsFromScene(CGraphicsScene *scene);
    void UpdateScenes();

    QStringList m_actions;
    QStringList m_variables;
    QStringList m_labels;
    QStringList m_scenes;

private slots:
    void UpdateCompletionModel();
};

#endif // CACTIONEDIT_H
