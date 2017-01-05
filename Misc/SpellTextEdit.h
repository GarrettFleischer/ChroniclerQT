#ifndef SPELLTEXTEDIT_H_
#define SPELLTEXTEDIT_H_

#include <QTextEdit>
#include <QAction>
#include <QContextMenuEvent>
#include "hunspell/hunspell.hxx"
//#include "settingaspell.h"

class SpellTextEdit : public QTextEdit
{
	Q_OBJECT

public:
    SpellTextEdit(QWidget *parent = 0,QString SpellDic="");
    virtual ~SpellTextEdit();
	QStringList getWordPropositions(const QString word);
	bool setDict(const QString SpellDic);

signals:
	void addWord(QString word);

protected:
	void createActions();
    void contextMenuEvent(QContextMenuEvent *event);

private slots:
	void correctWord();
	void slot_addWord();
	void slot_ignoreWord();

private:
    enum { MaxWords = 5 };
    QAction *misspelledWordsActs[MaxWords];

    QString spell_dic;
    Hunspell *pChecker;

    QPoint lastPos;

    QStringList addedWords;
};

#endif /*SPELLTEXTEDIT_H_*/
