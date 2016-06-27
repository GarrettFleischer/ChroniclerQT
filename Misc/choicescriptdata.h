#ifndef CHOICESCRIPTDATA_H
#define CHOICESCRIPTDATA_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QTextStream;
class QFile;
QT_END_NAMESPACE

class CGraphicsScene;
class CBubble;
class CConnection;

#include "Misc/cscenemodel.h"

#include "Misc/chronicler.h"
using Chronicler::CSIndent;

class ChoiceScriptData : public QObject
{
    Q_OBJECT

public:
    ChoiceScriptData(QFile &startup, const Chronicler::CSIndent &csindent);

    CSceneModel *getModel();

    const QString getTitle() const;
    const QString getAuthor() const;

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

    QList<CSBlock> ProcessFile(QFile &file, const CSIndent &csindent);

    // Choicescript processing
    QString CSStripIndent(const QString &line, const CSIndent &csindent);
    quint8 CSIndentLevel(const QString &line, const CSIndent &csindent);
    QList<CSLine> CSProcLines(QTextStream &stream, const CSIndent &csindent);
    QList<CSBlock> CSProcBlocks(const QList<CSLine> &lines);
    CSBlock CSProcBlock(const QList<CSLine> & lines, int &index);
    QStringList CSProcSceneList(const QList<CSBlock> &blocks);
    QList<CSBubble> CSProcBubbles(const QList<CSBlock> &blocks, CGraphicsScene *scene);
    CBubble *CSProcBubble(const CSBlock &csblock, QList<CSBubble> &deferredLinks, CGraphicsScene *scene, int row, int column, CBubble *prev);
    void CSLinkBubbles(QList<CSBubble> &csbubbles, CGraphicsScene *scene);
    CBubble *CSBubbleWithLabel(CGraphicsScene *scene, const QString &label);

    // Private Members
    CSceneModel m_model;
    QString m_title;
    QString m_author;

};

#endif // CHOICESCRIPTDATA_H
