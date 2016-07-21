#include "cactiondelegate.h"

#include <QComboBox>

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
                QComboBox *editor = new QComboBox(parent);
                QList<CVariable> variables = shared().variablesView->getVariablesForScene(Q_NULLPTR);

                for(const CVariable &v : variables)
                    editor->addItem(v.name());

                return editor;
            }
            else if(index.column() == 2)
            {
                QComboBox *editor = new QComboBox(parent);

                editor->addItems({"=", "%=", "+=", "-="});

                return editor;
            }
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
