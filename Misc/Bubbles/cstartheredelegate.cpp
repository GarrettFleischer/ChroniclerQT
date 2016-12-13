#include "cstartheredelegate.h"

#include <QLineEdit>

#include "cstarthereedit.h"

CStartHereDelegate::CStartHereDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}


QWidget *CStartHereDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    if(index.column() == 0)
        return new CStartHereEdit(parent);

    return new QLineEdit(parent);
}

void CStartHereDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.isValid())
    {
        if(index.column() == 0)
        {
            CStartHereEdit *startHereEdit = static_cast<CStartHereEdit *>(editor);
            startHereEdit->setText(index.data(Qt::EditRole).toString());
        }
        else
        {
            QLineEdit *dataEdit = static_cast<QLineEdit *>(editor);
            dataEdit->setText(index.data(Qt::EditRole).toString());
        }
    }
}

void CStartHereDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.isValid())
    {
        if(index.column() == 0)
        {
            CStartHereEdit *startHereEdit = static_cast<CStartHereEdit *>(editor);
            model->setData(index, startHereEdit->toPlainText());
        }
        else
        {
            QLineEdit *dataEdit = static_cast<QLineEdit *>(editor);
            model->setData(index, dataEdit->text());
        }
    }
}

void CStartHereDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)

    editor->setGeometry(option.rect);
}
