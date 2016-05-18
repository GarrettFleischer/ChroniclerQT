#ifndef CLISTVIEW_H
#define CLISTVIEW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QListView;
class QPushButton;
QT_END_NAMESPACE


class CListViewWidget : public QWidget
{
    Q_OBJECT

public:
    CListViewWidget(QWidget *parent);

private:
    // view/model
    QListView  *m_modelView;
//    CSceneModel *m_sceneModel;

    // list buttons
    QPushButton *m_upButton;
    QPushButton *m_downButton;
    QPushButton *m_addButton;
    QPushButton *m_removeButton;

private slots:
//    void SelectedChanged(QModelIndex current);

//    void MoveUp();
//    void MoveDown();
//    void AddItem();
//    void RemoveItem();

};

#endif // CLISTVIEW_H
