#ifndef CACTIONPROPERTIES_H
#define CACTIONPROPERTIES_H

#include "clistpropertieswidget.h"

QT_BEGIN_NAMESPACE
class QListView;
QT_END_NAMESPACE

class CActionBubble;

class CActionProperties : public CListPropertiesWidget
{
    Q_OBJECT

public:
    explicit CActionProperties(QStringListModel *model, QWidget *parent = 0);

    virtual void setBubble(CBubble *bbl) override;

private:
    CActionBubble *m_actionBubble;

protected slots:
    virtual void MoveUp() override;
    virtual void MoveDown() override;
    virtual void AddItem() override;
    virtual void RemoveItem() override;
};

#endif // CACTIONPROPERTIES_H
