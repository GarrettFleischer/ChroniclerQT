#ifndef CCONDITIONPROPERTIES_H
#define CCONDITIONPROPERTIES_H

#include "cpropertieswidget.h"

class CConditionBubble;


class CConditionProperties : public CPropertiesWidget
{
    Q_OBJECT

public:
    CConditionProperties(QStringListModel *model, QWidget *parent = 0);

public:
    virtual void setBubble(CBubble *bbl);
    virtual void setFont(const QFont &font);

protected:
    CConditionBubble *m_bbl;

    CLineEdit *m_conditionEdit;

signals:

protected slots:
    virtual void LabelChanged(QString label);
    virtual void OrderChanged(QString order);
    virtual void LockedChanged(bool locked);

    void ConditionChanged();
};

#endif // CCONDITIONPROPERTIES_H
