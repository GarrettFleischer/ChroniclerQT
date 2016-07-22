#ifndef CACTIONDELEGATE_H
#define CACTIONDELEGATE_H

#include <QStyledItemDelegate>

QT_BEGIN_NAMESPACE
class QComboBox;
QT_END_NAMESPACE

class CActionDelegate : public QStyledItemDelegate
{
public:
    CActionDelegate();

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;


private:
    QComboBox *createVariablesBox(QWidget *parent) const;
};

#endif // CACTIONDELEGATE_H
