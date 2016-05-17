#ifndef CPROJECTVIEW_H
#define CPROJECTVIEW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QListView;
class CSceneModel;
class QPushButton;
class QMenu;
QT_END_NAMESPACE

#include <QModelIndex>

class CGraphicsScene;


class CProjectView : public QWidget
{
    Q_OBJECT
public:
    CProjectView(QMenu *editMenu, QWidget *parent = 0);

    void Save();
    void SaveAs();
    void Load(const QString &file);

    QList<CGraphicsScene *> scenes();

private:
    void CreateBubbles();

    // view/model
    QListView  *m_modelView;
    CSceneModel *m_sceneModel;

    // list buttons
    QPushButton *m_upButton;
    QPushButton *m_downButton;
    QPushButton *m_addButton;
    QPushButton *m_removeButton;

    // for scene instantiation
    QMenu *m_editMenu;

    // project data
    QString m_path;

    // TODO
    // member variable for project history

private slots:
    void SelectedChanged(QModelIndex current);

    void MoveUp();
    void MoveDown();
    void AddItem();
    void RemoveItem();

};

#endif // CPROJECTVIEW_H
