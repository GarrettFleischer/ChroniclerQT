#ifndef CSCENECOMBOBOX_H
#define CSCENECOMBOBOX_H

#include <QComboBox>

QT_BEGIN_NAMESPACE
class QStringListModel;
QT_END_NAMESPACE

class CSceneModel;


class CSceneComboBox : public QComboBox
{
    Q_OBJECT

public:
    CSceneComboBox(QWidget *parent = Q_NULLPTR, CSceneModel *model = Q_NULLPTR);

    void setModel(CSceneModel *model);

private:
    CSceneModel *m_model;

private slots:
    void UpdateModel();
};

#endif // CSCENECOMBOBOX_H
