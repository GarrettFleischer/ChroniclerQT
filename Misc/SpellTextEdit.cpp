#include "SpellTextEdit.h"
#include <QTextCursor>
#include <QTextBlock>
#include <QMenu>
#include <QContextMenuEvent>
#include <QFileInfo>
#include <QTextCodec>
#include <QSettings>
#include <QTextStream>
#include <QCoreApplication>

#include "Misc/highlighter.h"


SpellTextEdit::SpellTextEdit(QWidget *parent, QString SpellDic)
    : QTextEdit(parent)
{
    //    SpellDic = QCoreApplication::applicationDirPath() + "/en_GB.dic";

    createActions();
    // create misspell actions in context menu
    spell_dic=SpellDic.left(SpellDic.length()-4);
    pChecker = new Hunspell(spell_dic.toLatin1()+".aff",spell_dic.toLatin1()+".dic");

    QFileInfo fi(SpellDic);
    if (!(fi.exists() && fi.isReadable())){
        delete pChecker;
        pChecker=0;
    }
    // get user config dictionary
    QSettings setting;
    QString filePath=QFileInfo(setting.fileName()).absoluteFilePath();
    filePath=filePath+"/User_"+QFileInfo(spell_dic.toLatin1()+".dic").fileName();
    //	std::cout << qPrintable(filePath) << std::endl;
    fi=QFileInfo(filePath);
    if (fi.exists() && fi.isReadable()){
        pChecker->add_dic(filePath.toLatin1());
    }
    else filePath="";

    addedWords.clear();
}


SpellTextEdit::~SpellTextEdit()
{
    // write user dictionary
    QSettings setting;
    QString fileName=QFileInfo(setting.fileName()).absoluteFilePath();
    fileName=fileName+"/User_"+QFileInfo(spell_dic.toLatin1()+".dic").fileName();
    //	std::cout << qPrintable(fileName) << std::endl;
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        in.readLine();
        while (!in.atEnd()) {
            QString line = in.readLine();
            if(!addedWords.contains(line)) addedWords << line;
        }
        file.close();
    }
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        //		std::cout << "write" << std::endl;

        QTextStream out(&file);
        QByteArray encodedString;
        QString spell_encoding=QString(pChecker->get_dic_encoding());
        QTextCodec *codec = QTextCodec::codecForName(spell_encoding.toLatin1());
        out << addedWords.count() << "\n";
        foreach(QString elem, addedWords){
            encodedString = codec->fromUnicode(elem);
            out << encodedString.data() << "\n";
            //			std::cout << encodedString.data() << std::endl;
        }
    }
}

bool SpellTextEdit::setDict(const QString SpellDic)
{
    if(SpellDic!=""){
        //mWords.clear();
        spell_dic=SpellDic.left(SpellDic.length()-4);
        delete pChecker;
        pChecker = new Hunspell(spell_dic.toLatin1()+".aff",spell_dic.toLatin1()+".dic");
    }
    else spell_dic="";

    QFileInfo fi(SpellDic);
    if (!(fi.exists() && fi.isReadable()))
    {
        delete pChecker;
        pChecker=0;
    }
    else
    {
        // get user config dictionary
        QSettings setting;
        QString filePath=QFileInfo(setting.fileName()).absoluteFilePath();
        filePath=filePath+"/User_"+QFileInfo(spell_dic.toLatin1()+".dic").fileName();
        //		std::cout << qPrintable(filePath) << std::endl;
        fi=QFileInfo(filePath);
        if (fi.exists() && fi.isReadable()){
            pChecker->add_dic(filePath.toLatin1());
        }
        else filePath="";
    }

    return (spell_dic!="");
}


void SpellTextEdit::createActions() {
    for (int i = 0; i < MaxWords; ++i) {
        misspelledWordsActs[i] = new QAction(this);
        misspelledWordsActs[i]->setVisible(false);
        connect(misspelledWordsActs[i], SIGNAL(triggered()), this, SLOT(correctWord()));
    }
}

void SpellTextEdit::correctWord() {
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        QString replacement = action->text();
        QTextCursor cursor = cursorForPosition(lastPos);
        //QTextCursor cursor = textCursor();
        QString zeile = cursor.block().text();
        cursor.select(QTextCursor::WordUnderCursor);
        cursor.deleteChar();
        cursor.insertText(replacement);
    }
}

void SpellTextEdit::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = createStandardContextMenu();
    lastPos=event->pos();
    QTextCursor cursor = cursorForPosition(lastPos);
    QString zeile = cursor.block().text();
    int pos = cursor.columnNumber();
    int end = zeile.indexOf(QRegExp("\\W+"),pos);
    int begin = zeile.lastIndexOf(QRegExp("\\W+"),pos);
    zeile=zeile.mid(begin+1,end-begin-1);
    QStringList liste = getWordPropositions(zeile);
    if (!liste.isEmpty())
    {
        menu->addSeparator();
        QAction *a;
        a = menu->addAction(tr("Add .."), this, SLOT(slot_addWord()));
        a = menu->addAction(tr("Ignore .."), this, SLOT(slot_ignoreWord()));
        for (int i = 0; i < qMin(int(MaxWords),liste.size()); ++i) {
            misspelledWordsActs[i]->setText(liste.at(i).trimmed());
            misspelledWordsActs[i]->setVisible(true);
            menu->addAction(misspelledWordsActs[i]);
        }

    } // if  misspelled
    menu->exec(event->globalPos());
    delete menu;
}

QStringList SpellTextEdit::getWordPropositions(const QString word)
{
    QStringList wordList;
    if(pChecker){
        QByteArray encodedString;
        QString spell_encoding=QString(pChecker->get_dic_encoding());
        QTextCodec *codec = QTextCodec::codecForName(spell_encoding.toLatin1());
        encodedString = codec->fromUnicode(word);
        bool check = pChecker->spell(encodedString.data());
        if(!check){
            char ** wlst;
            int ns = pChecker->suggest(&wlst,encodedString.data());
            if (ns > 0)
            {
                for (int i=0; i < ns; i++)
                {
                    wordList.append(codec->toUnicode(wlst[i]));
                    //free(wlst[i]);
                }
                //free(wlst);
                pChecker->free_list(&wlst, ns);
            }// if ns >0
        }// if check
    }
    return wordList;
}

void SpellTextEdit::slot_addWord()
{
    QTextCursor cursor = cursorForPosition(lastPos);
    QString zeile = cursor.block().text();
    int pos = cursor.columnNumber();
    int end = zeile.indexOf(QRegExp("\\W+"),pos);
    int begin = zeile.left(pos).lastIndexOf(QRegExp("\\W+"),pos);
    zeile=zeile.mid(begin+1,end-begin-1);
    //    std::cout << qPrintable(zeile) << std::endl;
    QByteArray encodedString;
    QString spell_encoding=QString(pChecker->get_dic_encoding());
    QTextCodec *codec = QTextCodec::codecForName(spell_encoding.toLatin1());
    encodedString = codec->fromUnicode(zeile);
    pChecker->add(encodedString.data());
    addedWords.append(zeile);
    emit addWord(zeile);
}

void SpellTextEdit::slot_ignoreWord()
{
    QTextCursor cursor = cursorForPosition(lastPos);
    QString zeile = cursor.block().text();
    int pos = cursor.columnNumber();
    int end = zeile.indexOf(QRegExp("\\W+"),pos);
    int begin = zeile.left(pos).lastIndexOf(QRegExp("\\W+"),pos);
    zeile=zeile.mid(begin+1,end-begin-1);
    QByteArray encodedString;
    QString spell_encoding=QString(pChecker->get_dic_encoding());
    QTextCodec *codec = QTextCodec::codecForName(spell_encoding.toLatin1());
    encodedString = codec->fromUnicode(zeile);
    pChecker->add(encodedString.data());
    emit addWord(zeile);
}
