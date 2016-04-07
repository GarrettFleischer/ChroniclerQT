#include "clineedit.h"
#include "ctextedit.h"

#include <QFontMetrics>


CLineEdit::CLineEdit(QWidget *parent, QStringListModel *model, const QString &text)
    : CTextEdit(parent, model, text)
{
    m_acceptsReturn = false;

    setLineWrapMode(QTextEdit::NoWrap);
    setWordWrapMode(QTextOption::NoWrap);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    AdjustHeight();
}

void CLineEdit::SetFont(const QFont &font)
{
    CTextEdit::setFont(font);
    AdjustHeight();
}

//void CLineEdit::keyPressEvent(QKeyEvent *e)
//{
//    CTextEdit::keyPressEvent(e);

//    if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
//    {
//        QTextCursor cursor = textCursor();
//        setPlainText(toPlainText().remove('\r').remove('\n'));
//        setTextCursor(cursor);
//    }
//}

void CLineEdit::AdjustHeight()
{
    QFontMetrics fm(font());
    qreal height = fm.boundingRect("W").height() * 2;
    setMinimumHeight(height);
    setMaximumHeight(height);

}
