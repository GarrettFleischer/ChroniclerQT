#ifndef CCHOICEPROPERTIES_H
#define CCHOICEPROPERTIES_H

#include "cpropertieswidget.h"

class CChoiceBubble;

class CChoiceProperties : public CPropertiesWidget
{
    Q_OBJECT
public:
    explicit CChoiceProperties(QStringListModel *model, QWidget *parent = 0);

    virtual void setBubble(CBubble *bbl);
    virtual void setFont(const QFont &font);

private:
    CChoiceBubble *m_bbl;


signals:

protected slots:
    virtual void LabelChanged(QString label);
    virtual void OrderChanged(QString order);
    virtual void LockedChanged(bool locked);
};

#endif // CCHOICEPROPERTIES_H
