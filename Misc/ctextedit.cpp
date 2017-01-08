#include "ctextedit.h"

#include "Misc/cshighlighter.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

CTextEdit::CTextEdit(QWidget * parent, QStringListModel * model, const QString & text)
    : SpellTextEdit(parent), m_completer(0), m_completionModel(model), m_enabled(true), m_alwaysEnabled(false), m_acceptsReturn(true), m_acceptsTab(false), m_ctrlHeld(false)
{
    setText(text);

    if(!m_completionModel)
        m_completionModel = new QStringListModel(this);

    m_filtered = new QStringListModel(this);

    setCompleter(new QCompleter(m_filtered, this));

    connect(shared().escapeAction, SIGNAL(triggered(bool)), this, SLOT(EscapePressed()));

    QString dictPath = QCoreApplication::applicationDirPath() + "/en_US.dic";
    setDict(dictPath);

    Highlighter *highlighter = new Highlighter(document(), dictPath, true);
    connect(this, SIGNAL(addWord(QString)), highlighter, SLOT(slot_addWord(QString)));
}


void CTextEdit::setCompleter(QCompleter *completer)
{
    if (m_completer)
        QObject::disconnect(m_completer, 0, 0, 0);

    m_completer = completer;

    if (m_completer)
    {
        m_completionModel->setParent(m_completer);

        m_completer->setWidget(this);
        m_completer->setCompletionMode(QCompleter::PopupCompletion);
        m_completer->setCaseSensitivity(Qt::CaseInsensitive);
        m_completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
        m_completer->setModel(m_filtered);

        QObject::connect(m_completer, SIGNAL(activated(QString)),
                         this, SLOT(insertCompletion(QString)));
    }
}


QCompleter *CTextEdit::completer() const
{
    return m_completer;
}


QStringList *CTextEdit::listFromFile(const QString & fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return new QStringList();

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif

    QStringList * words = new QStringList();

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (!line.isEmpty())
            (*words) << line.trimmed();
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    return words;
}

bool CTextEdit::acceptsTab() const
{
    return m_acceptsTab;
}

void CTextEdit::setAcceptsTab(bool acceptsTab)
{
    m_acceptsTab = acceptsTab;
}

void CTextEdit::setAlwaysEnabled(bool alwaysEnabled)
{
    m_alwaysEnabled = alwaysEnabled;
    if(m_alwaysEnabled)
        m_completer->popup();
}

QStringListModel *CTextEdit::model() const
{
    return m_completionModel;
}

void CTextEdit::setModel(QStringListModel *model)
{
    m_completionModel = model;
}


int moveCursorToWordStart(QTextCursor & tc)
{
    tc.movePosition(tc.Left);
    tc.movePosition(tc.Right, QTextCursor::KeepAnchor);

    int offset = 1;

    while(tc.selection().toPlainText() != " " && tc.selection().toPlainText() != "\n" && tc.position() > 1)
    {
        tc.movePosition(tc.Left);
        tc.movePosition(tc.Left);
        tc.movePosition(tc.Right, QTextCursor::KeepAnchor);

        offset++;
    }

    // if it found a space or newline, deselect the space, and move to the start
    if(tc.position() > 1)
    {
        tc.movePosition(tc.Right);
        offset--;
    }

    return offset;
}

QString CTextEdit::textUnderCursor() const
{
    QString selected_text = "";

    QTextCursor tc = textCursor();
    int offset = moveCursorToWordStart(tc);

    tc.movePosition(tc.Right, QTextCursor::KeepAnchor, offset);

    if(tc.selectedText() == "*" || tc.selectedText() == "$" || tc.selectedText() == "${")
        selected_text = tc.selectedText();

    tc.select(QTextCursor::WordUnderCursor);

    selected_text += tc.selectedText();

    return selected_text;
}


void CTextEdit::insertCompletion(const QString& completion)
{
    if (m_completer->widget() != this)
        return;

    QTextCursor tc = textCursor();
    int offset = moveCursorToWordStart(tc);

    for(int i = 0; i < offset; ++i)
        tc.movePosition(tc.Right, QTextCursor::KeepAnchor);

    tc.movePosition(tc.EndOfWord, QTextCursor::KeepAnchor);

    tc.insertText(completion);

    setTextCursor(tc);
}

void CTextEdit::EscapePressed()
{
    if(!m_completer->popup()->isHidden())
        m_enabled = false;
    else
        m_enabled = true;
}


void CTextEdit::focusInEvent(QFocusEvent *e)
{
    if (m_completer)
        m_completer->setWidget(this);
    QTextEdit::focusInEvent(e);
}


void CTextEdit::keyPressEvent(QKeyEvent *e)
{
    const bool isEscape = (e->key() == Qt::Key_Escape);
    if(isEscape)
        EscapePressed();
    const bool isReturn = (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter);
    const bool isTab = (e->key() == Qt::Key_Tab);


    if (m_completer && m_completer->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
        switch (e->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
        default:
            break;
        }
    }

    if ((!m_completer || !isEscape) && !(isReturn && !m_acceptsReturn) && !(isTab && !m_acceptsTab))
    {
        if(isTab)
            textCursor().insertText("    ");
        else
            QTextEdit::keyPressEvent(e);
    }


    // exit if disabled, or one of the modifiers is pressed
    const bool shiftMod = (e->modifiers() & Qt::ShiftModifier);
    const bool ctrlMod = (e->modifiers() & Qt::ControlModifier);
    if (!m_completer || ctrlMod || (shiftMod && e->text().isEmpty()))
        return;

    QString completionPrefix = textUnderCursor().replace("*","\\*");
    completionPrefix = completionPrefix.replace("$", "\\$");
    completionPrefix = completionPrefix.replace("{", "\\{");

    QStringList filteredList = m_completionModel->stringList().filter(QRegExp("*"+completionPrefix+"*", m_completer->caseSensitivity(), QRegExp::WildcardUnix));
    m_filtered->setStringList(filteredList);


    if (!m_alwaysEnabled && ((!m_enabled) || (shiftMod || e->text().isEmpty() || (completionPrefix.isEmpty() && !isEscape) ||
                                              (!filteredList.isEmpty() && completionPrefix == filteredList.first()))))
    {
        m_completer->popup()->hide();
    }
    else
    {
        m_completer->popup()->setCurrentIndex(m_completer->completionModel()->index(0, 0));

        QRect cr = cursorRect();
        cr.setWidth(m_completer->popup()->sizeHintForColumn(0)
                    + m_completer->popup()->verticalScrollBar()->sizeHint().width());

        m_completer->complete(cr); // pop it up!
    }
}
