#include "ctextedit.h"


CTextEdit::CTextEdit(QWidget * parent, QStringListModel * model, const QString & text)
: QTextEdit(parent), m_completer(0), m_model(model), m_enabled(true), m_acceptsReturn(true)
{
//    setTabChangesFocus(true);
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
    m_completer->setWrapAround(false);
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
    selected_text = tc.selectedText();


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
    bool isShortcut = (e->key() == Qt::Key_Escape);
    if(isShortcut)
        m_enabled = !m_enabled;

    bool isReturn = (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter);
    bool isTab = (e->key() == Qt::Key_Tab);


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

    if ((!m_completer || !isShortcut) && !(isReturn && !m_acceptsReturn) && !isTab)
        QTextEdit::keyPressEvent(e);


    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!m_completer || (ctrlOrShift && e->text().isEmpty()))
        return;

    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    QStringList filteredList = m_model->stringList().filter(completionPrefix, m_completer->caseSensitivity());
    m_filtered->setStringList(filteredList);


    if (!m_enabled || (hasModifier || e->text().isEmpty() || completionPrefix.isEmpty() ||
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
