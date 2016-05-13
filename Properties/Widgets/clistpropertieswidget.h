#ifndef CLISTPROPERTIESWIDGET_H
#define CLISTPROPERTIESWIDGET_H

#include "cpropertieswidget.h"

#include <QModelIndex>

QT_BEGIN_NAMESPACE
class QListView;
class QPushButton;
QT_END_NAMESPACE


class CListPropertiesWidget : public CPropertiesWidget
{
    Q_OBJECT

public:
    CListPropertiesWidget(QStringListModel *model, QWidget *parent);

    virtual void setBubble(CBubble *bbl) override;

protected:
    QListView *m_modelView;

    QPushButton *m_upButton;
    QPushButton *m_downButton;
    QPushButton *m_addButton;
    QPushButton *m_removeButton;

protected slots:
    virtual void MoveUp() = 0;
    virtual void MoveDown() = 0;
    virtual void AddItem() = 0;
    virtual void RemoveItem() = 0;

private slots:
    void SelectedChanged(QModelIndex current);

};

#endif // CLISTPROPERTIESWIDGET_H
