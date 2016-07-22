#include "cactiondelegate.h"

#include <QComboBox>
#include <QLineEdit>

#include <QAbstractTableModel>

#include "Misc/Bubbles/cactionmodel.h"

#include "Misc/Variables/cvariable.h"
#include "Properties/cvariablesview.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

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

        return editor;
    }
    else
    {
        QString first_column = index.model()->index(index.row(), 0).data().toString();

        if(first_column == "*set")
        {
            if(index.column() == 1)
            {
                return createVariablesBox(parent);
            }
            else if(index.column() == 2)
            {
                QComboBox *editor = new QComboBox(parent);
                editor->addItems({"=", "%=", "+=", "-="});

                return editor;
            }
            else if(index.column() == 3)
                return QStyledItemDelegate::createEditor(parent, option, index);
        }
        else if(first_column == "*input_text")
        {
            return createVariablesBox(parent);
        }
        else if(first_column == "*label" || first_column == "*goto" ||
                first_column == "*purchase" || first_column == "*check_purchase")
        {
            if(index.column() == 1)
                return QStyledItemDelegate::createEditor(parent, option, index);
        }
    }


    return Q_NULLPTR;
}

void CActionDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.column() == 0)
    {
        QComboBox *box = static_cast<QComboBox *>(editor);
        box->setCurrentText(index.data(Qt::EditRole).toString());
    }
    else
    {
        QString first_column = index.model()->index(index.row(), 0).data().toString();

        if(first_column == "*set")
        {
            if(index.column() < 3)
            {
                QComboBox *box = static_cast<QComboBox *>(editor);
                box->setCurrentText(index.data(Qt::EditRole).toString());
            }
            else
            {
                QLineEdit *line = static_cast<QLineEdit *>(editor);
                line->setText(index.data(Qt::EditRole).toString());
            }
        }
        else if(first_column == "*input_text")
        {
            if(index.column() == 1)
            {
                QComboBox *box = static_cast<QComboBox *>(editor);
                box->setCurrentText(index.data(Qt::EditRole).toString());
            }
        }
        else if(first_column == "*label" || first_column == "*goto" ||
                first_column == "*purchase" || first_column == "*check_purchase")
        {
            if(index.column() == 1)
            {
                QLineEdit *line = static_cast<QLineEdit *>(editor);
                line->setText(index.data(Qt::EditRole).toString());
            }
        }
    }
}

void CActionDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QAbstractTableModel *actionModel = static_cast<QAbstractTableModel *>(model);

    if(index.column() == 0)
    {
        QComboBox *box = static_cast<QComboBox *>(editor);
        actionModel->setData(index, box->currentText());
    }
    else
    {
        QString first_column = index.model()->index(index.row(), 0).data().toString();

        if(first_column == "*set")
        {
            if(index.column() < 3)
            {
                QComboBox *box = static_cast<QComboBox *>(editor);
                actionModel->setData(index, box->currentText());
            }
            else
            {
                QLineEdit *line = static_cast<QLineEdit *>(editor);
                actionModel->setData(index, line->text());
            }
        }
        else if(first_column == "*input_text")
        {
            if(index.column() == 1)
            {
                QComboBox *box = static_cast<QComboBox *>(editor);
                actionModel->setData(index, box->currentText());
            }
        }
        else if(first_column == "*label" || first_column == "*goto" ||
                first_column == "*purchase" || first_column == "*check_purchase")
        {
            if(index.column() == 1)
            {
                QLineEdit *line = static_cast<QLineEdit *>(editor);
                actionModel->setData(index, line->text());
            }
        }
    }
}

void CActionDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)

    editor->setGeometry(option.rect);
}

QComboBox *CActionDelegate::createVariablesBox(QWidget *parent) const
{
    QComboBox *editor = new QComboBox(parent);
    QList<CVariable> variables = shared().variablesView->getVariablesForScene(Q_NULLPTR);

    for(const CVariable &v : variables)
        editor->addItem(v.name());

    return editor;
}
