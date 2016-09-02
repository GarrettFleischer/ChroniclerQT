#ifndef CLINEEDIT_H
#define CLINEEDIT_H

#include <QWidget>
#include <QStringListModel>
#include <QFont>

#include "Misc/ctextedit.h"

class CLineEdit : public CTextEdit
{
public:
    CLineEdit(QWidget *parent, QStringListModel *model = Q_NULLPTR, const QString &text = "");
};

#endif // CLINEEDIT_H
