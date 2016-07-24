#ifndef CPROJECTVIEW_H
#define CPROJECTVIEW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QListView;
class QPushButton;
class QMenu;
class QLineEdit;
class QSaveFile;
class QTextStream;
QT_END_NAMESPACE

#include <QModelIndex>

class CGraphicsView;
class CGraphicsScene;
class CSceneModel;
class CBubble;
class CConnection;

#include <Misc/chronicler.h>
using Chronicler::CSIndent;
using Chronicler::CVersion;

class CProjectView : public QWidget
{
    Q_OBJECT

public:
    CProjectView(QWidget *parent = 0);

    QList<CGraphicsView *> getViews();

    CBubble *BubbleWithUID(uint uid);

    const CVersion &getVersion() const;

    CSceneModel *model();

private:
    // Private Methods
    void CreateBubbles();

    void CalculateOrder(CConnection *connection, QList<CConnection *> &processed, qint64 order);
    QString BubbleToChoiceScript(const QList<CBubble *> &bubbles, QList<CBubble *> &processed, int indent_level, CBubble *bubble);
    bool LabelNeeded(CBubble *bubble, const QList<CBubble *> &bubbles);
    QString MakeLabel(CBubble *bubble, const QList<CBubble *> &bubbles);

    void SaveToFile(QSaveFile &file);

    // Private Members
    QLineEdit *m_title;
    QLineEdit *m_author;

    // view/model
    QListView  *m_modelView;
    CSceneModel *m_sceneModel;

    // project data
    CVersion m_version;

    // for quicksave
    QString m_path;

    // for keeping track of autosaves
    qint64 m_autosave_num;

    // TODO
    // member variable for project history

signals:
    void sceneSelected(CGraphicsView *);

public slots:
    void SaveProject();
    void SaveProjectAs();
    void OpenProject(QString filepath = "");
    void ImportChoiceScript(const QString &filepath = "");
    void NewProject();
    void CloseProject();
    void ExportChoiceScript();

private slots:
    void SelectedChanged(QModelIndex current);
    void DataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void ProjectNameChanged();

    void Autosave();

    void MoveUp();
    void MoveDown();
    void AddItem();
    void RemoveItem();

};

#endif // CPROJECTVIEW_H
