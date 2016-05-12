#ifndef CACTIONPROPERTIES_H
#define CACTIONPROPERTIES_H

#include "cpropertieswidget.h"

QT_BEGIN_NAMESPACE
class QListView;
QT_END_NAMESPACE

class CActionBubble;

class CActionProperties : public CPropertiesWidget
{
    Q_OBJECT

public:
    explicit CActionProperties(QStringListModel *model, QWidget *parent = 0);

    virtual void setBubble(CBubble *bbl) override;

private:
    CActionBubble *m_actionBubble;

    QListView *m_actions;
};

#endif // CACTIONPROPERTIES_H
