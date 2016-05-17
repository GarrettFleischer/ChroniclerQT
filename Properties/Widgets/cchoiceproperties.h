#ifndef CCHOICEPROPERTIES_H
#define CCHOICEPROPERTIES_H

#include "clistpropertieswidget.h"

class CChoiceBubble;

class CChoiceProperties : public CListPropertiesWidget
{
    Q_OBJECT

public:
    explicit CChoiceProperties(QStringListModel *model, QWidget *parent = 0);

    virtual void setBubble(CBubble *bbl);

private:
    CChoiceBubble *m_choiceBubble;

protected slots:
    virtual void MoveUp() override;
    virtual void MoveDown() override;
    virtual void AddItem() override;
    virtual void RemoveItem() override;
};

#endif // CCHOICEPROPERTIES_H
