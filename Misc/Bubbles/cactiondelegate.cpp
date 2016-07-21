#include "cactiondelegate.h"

#include <QComboBox>

CActionDelegate::CActionDelegate()
{

}

QWidget *CActionDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    if(index.column() == 0)
    {
        QComboBox *editor = new QComboBox(parent);
        editor->addItems({"*set", "*page_break", "*input_text", "*label", "*goto", "*finish", "*purchase", "*check_purchase", "*advertisement"});
    }
    if(index.column() == 1)
    {
        QString first_column = index.model()->index(index.row(), 0).data().toString();
        if(first_column == "*set")
        {

        }
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void CActionDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
}

void CActionDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
}

void CActionDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
}
