#include "ctextedit.h"


CTextEdit::CTextEdit(QWidget * parent, QStringListModel * model, const QString & text)
: QTextEdit(parent), m_completer(0), m_model(model), m_enabled(true), m_acceptsReturn(true), m_ctrlHeld(false)
{
    m_filtered = new QStringListModel(this);
    setText(text);
    setCompleter(new QCompleter(m_filtered, this));
}


void CTextEdit::setCompleter(QCompleter *completer)
{
    if (m_completer)
        QObject::disconnect(m_completer, 0, 0, 0);

    m_completer = completer;

    if (!m_completer)
        return;

    m_model->setParent(m_completer);
    m_completer->setWidget(this);
    m_completer->setCompletionMode(QCompleter::PopupCompletion);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    m_completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    m_completer->setModel(m_filtered);

    QObject::connect(m_completer, SIGNAL(activated(QString)),
                     this, SLOT(insertCompletion(QString)));
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


int moveCursorToWordStart(QTextCursor & tc)
{
    //tc.movePosition(tc.StartOfWord);
    tc.movePosition(tc.Left);
    tc.movePosition(tc.Right, QTextCursor::KeepAnchor);

    int offset = 1;
    while(tc.selectedText() != " " && tc.position() > 1)
    {
        tc.movePosition(tc.Left);
        tc.movePosition(tc.Left);
        tc.movePosition(tc.Right, QTextCursor::KeepAnchor);

        offset++;
    }

    // if it found a space, deselect the space, and move to the start
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

    for(int i = 0; i < offset; ++i)
        tc.movePosition(tc.Right, QTextCursor::KeepAnchor);

    if(tc.selectedText() == "*" || tc.selectedText() == "$" || tc.selectedText() == "${" )
        selected_text = tc.selectedText();

    if(!selected_text.isEmpty())
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
    {
        if(!m_completer->popup()->isHidden())
            m_enabled = false;
        else
            m_enabled = true;
    }

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

    if ((!m_completer || !isEscape) && !(isReturn && !m_acceptsReturn) && !isTab)
        QTextEdit::keyPressEvent(e);


    // exit if disabled, or one of the modifiers is pressed
    const bool shiftMod = (e->modifiers() & Qt::ShiftModifier);
    const bool ctrlMod = (e->modifiers() & Qt::ControlModifier);
    if (!m_completer || ctrlMod || (shiftMod && e->text().isEmpty()))
        return;

    QString completionPrefix = textUnderCursor().replace("*","\\*");

    QStringList filteredList = m_model->stringList().filter(QRegExp("*"+completionPrefix+"*", m_completer->caseSensitivity(), QRegExp::WildcardUnix));
    m_filtered->setStringList(filteredList);


    if ((!m_enabled) || (shiftMod || e->text().isEmpty() || (completionPrefix.isEmpty() && !isEscape) ||
                       (!filteredList.isEmpty() && completionPrefix == filteredList.first())))
    {
        m_completer->popup()->hide();
    }
    else
    {
        m_completer->popup()->setCurrentIndex(m_completer->completionModel()->index(0, 0));

        QRect cr = cursorRect();
        cr.setWidth(m_completer->popup()->sizeHintForColumn(0)
                    + m_completer->popup()->verticalScrollBar()->sizeHint().width());

        m_completer->complete(cr); // popup it up!
    }
}
