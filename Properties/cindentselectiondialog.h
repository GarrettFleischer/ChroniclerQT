#ifndef CINDENTSELECTIONDIALOG_H
#define CINDENTSELECTIONDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QSpinBox;
class QPushButton;
QT_END_NAMESPACE

#include "Misc/chronicler.h"
using Chronicler::CSIndent;

class CIndentSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    CIndentSelectionDialog(QWidget *parent);

    CSIndent getIndent();

private:
    QPushButton *m_type;
    QSpinBox *m_count;
    CSIndent m_indent;

private slots:
    void TabSelected();
    void SpaceSelected();
    void IndentChanged(int count);
};

#endif // CINDENTSELECTIONDIALOG_H
