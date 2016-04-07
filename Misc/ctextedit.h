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
    CTextEdit(QWidget * parent, QStringListModel * model, const QString & text = "");

    void setCompleter(QCompleter * completer);
    QCompleter *completer() const;

protected:
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void focusInEvent(QFocusEvent *e) Q_DECL_OVERRIDE;

private:
    QString textUnderCursor() const;
    QStringList *listFromFile(const QString & fileName);

protected:
    QCompleter * m_completer;
    //QStringList * m_list;
    QStringListModel * m_model;
    QStringListModel * m_filtered;
    bool m_enabled;

    bool m_acceptsReturn;

private slots:
    void insertCompletion(const QString &completion);
};

#endif // TEXTEDIT_H
