#include "clineedit.h"
#include "ctextedit.h"

CLineEdit::CLineEdit(QWidget *parent, QStringListModel *model, const QString &text)
    : CTextEdit(parent, model, text)
{
    m_acceptsReturn = false;

    setLineWrapMode(QTextEdit::NoWrap);
    setWordWrapMode(QTextOption::NoWrap);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}
