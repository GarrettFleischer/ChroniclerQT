#ifndef CSTARTHEREDELEGATE_H
#define CSTARTHEREDELEGATE_H

#include <QStyledItemDelegate>

class CStartHereDelegate : public QStyledItemDelegate
{
public:
    explicit CStartHereDelegate(QObject *parent = Q_NULLPTR);

    // QAbstractItemDelegate interface
public:
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // CSTARTHEREDELEGATE_H
