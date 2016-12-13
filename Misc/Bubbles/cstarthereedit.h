#ifndef CSTARTHEREEDIT_H
#define CSTARTHEREEDIT_H

#include <QRegExpValidator>

#include "Misc/clineedit.h"


class CStartHereEdit : public CLineEdit
{
public:
    CStartHereEdit(QWidget *parent = Q_NULLPTR);

private:
    QRegExpValidator m_validator;
};

#endif // CSTARTHEREEDIT_H
