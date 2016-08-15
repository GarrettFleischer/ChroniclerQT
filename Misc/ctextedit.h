#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QtCore>
#include <QtGui>

#include <QTextEdit>
#include <QCompleter>
#include <QAbstractItemView>
#include <QKeyEvent>
#include <QScrollBar>
#include <QStringListModel>
#include <QApplication>
#include <QFile>
#include <QByteArray>



class CTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    CTextEdit(QWidget * parent, QStringListModel * model = Q_NULLPTR, const QString & text = "");

    void setCompleter(QCompleter * completer);
    QCompleter *completer() const;

    QStringListModel *model() const;
    void setModel(QStringListModel *model);

    void setAlwaysEnabled(bool alwaysEnabled);

protected:
    virtual void keyPressEvent(QKeyEvent *e) override;
    virtual void focusInEvent(QFocusEvent *e) override;

private:
    QString textUnderCursor() const;
    QStringList *listFromFile(const QString & fileName);

protected:
    QCompleter * m_completer;
    QStringListModel * m_completionModel;
    QStringListModel * m_filtered;
    bool m_enabled;
    bool m_alwaysEnabled;

    bool m_acceptsReturn;

    bool m_ctrlHeld;

private slots:
    void insertCompletion(const QString &completion);
};

#endif // TEXTEDIT_H
