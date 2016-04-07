#ifndef CCONDITIONPROPERTIES_H
#define CCONDITIONPROPERTIES_H


#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QIntValidator>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStringListModel>

#include "Misc/clineedit.h"

class CBubble;
class CConditionBubble;


class CConditionProperties : public QWidget
{
    Q_OBJECT

public:
    CConditionProperties(CBubble *bbl, QStringListModel *model, QWidget *parent = 0);

public:
    void SetBubble(CBubble *bbl);

    virtual void setFont(const QFont &font);

private:
    CConditionBubble *m_bbl;

    QStringListModel *m_model;
    QLineEdit *m_labelEdit;
    CLineEdit *m_conditionEdit;

    QCheckBox *m_lockEdit;
    QLineEdit *m_orderEdit;

signals:

private slots:
    void LabelChanged(QString title);
    void ConditionChanged();
    void OrderChanged(QString order);
    void LockedChanged(bool locked);
};

#endif // CCONDITIONPROPERTIES_H
