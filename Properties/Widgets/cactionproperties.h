#ifndef CACTIONPROPERTIES_H
#define CACTIONPROPERTIES_H

#include "cpropertieswidget.h"

#include <QModelIndex>

QT_BEGIN_NAMESPACE
class QListView;
QT_END_NAMESPACE

class CActionBubble;


class CActionProperties : public CPropertiesWidget
{
    Q_OBJECT

public:
    explicit CActionProperties(QWidget *parent = 0);

    virtual void setBubble(CBubble *bbl) override;

private:
    QListView *m_view;
    CActionBubble *m_actionBubble;


private slots:
    void MoveUp();
    void MoveDown();
    void AddItem();
    void RemoveItem();
};

#endif // CACTIONPROPERTIES_H
