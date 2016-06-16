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

class CProjectView : public QWidget
{
    Q_OBJECT

public:
    CProjectView(QWidget *parent = 0);

    QList<CGraphicsView *> views();

    CBubble *BubbleWithUID(uint uid);

    const QString version() const;

private:
    // Enums and Structs
    enum CSType { Empty, Title, Author, Create, Temp, SceneList, StatChart, ChoiceAction, FakeChoice, Choice, If, Else, ElseIf, Action, Text, Label, Finish, GoTo };

    struct CSLine
    {
        CSType type = Empty;
        QString line;
        quint8 indent;
        QList<CSLine *> children;

        CSLine(QString _line) : line(_line){}

        bool operator ==(const CSLine &rhs) { return this == &rhs; }
    };

    struct CSBlock
    {
        CSType type = Empty;
        QString text;
        QList<CSBlock> children;
        QString label;
        int width = 1;
        int height = 1;

        void AddChild(const CSBlock &block)
        {
            if(block.type != Empty)
                children.append(block);
        }
    };

    struct CSBubble
    {
        CBubble *bubble = Q_NULLPTR;
        QString link;
        Chronicler::Anchor anchor;

        bool operator ==(const CSBubble &rhs) { return bubble == rhs.bubble; }
    };

    struct CSIndent
    {
        char type = ' ';
        quint8 count = 4;
    };

    // Private Methods
    void CreateBubbles();

    void CalculateOrder(CConnection *connection, QList<CConnection *> &processed, qint64 order);
    QString BubbleToChoiceScript(const QList<CBubble *> &bubbles, QList<CBubble *> &processed, int indent_level, CBubble *bubble);
    bool LabelNeeded(CBubble *bubble, const QList<CBubble *> &bubbles);
    QString MakeLabel(CBubble *bubble, const QList<CBubble *> &bubbles);

    void SaveToFile(QSaveFile &file);

    // Choicescript processing
    QString CSStripIndent(const QString &line, const CSIndent &csindent);
    quint8 CSIndentLevel(const QString &line, const CSIndent &csindent);
    QList<CSLine> CSProcLines(QTextStream &stream, const CSIndent &csindent);
    QList<CSBlock> CSProcBlocks(const QList<CSLine> &lines);
    CSBlock CSProcBlock(const QList<CSLine> & lines, int &index);
    QList<CSBubble> CSProcBubbles(const QList<CSBlock> &blocks, CGraphicsScene *scene);
    CBubble *CSProcBubble(const CSBlock &csblock, QList<CSBubble> &deferredLinks, CGraphicsScene *scene, int row, int column, CBubble *prev);
    void CSLinkBubbles(QList<CSBubble> &csbubbles, CGraphicsScene *scene);
    CBubble *CSBubbleWithLabel(CGraphicsScene *scene, const QString &label);


    // Private Members
    QLineEdit *m_name;

    // view/model
    QListView  *m_modelView;
    CSceneModel *m_sceneModel;

    // list buttons
    QPushButton *m_upButton;
    QPushButton *m_downButton;
    QPushButton *m_addButton;
    QPushButton *m_removeButton;

    // project data
    QString m_version;

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
    void OpenProject(const QString &filepath);
    void ImportChoiceScript(const QString &filepath);
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
