#include "cvariablesdelegate.h"

//#include <QComboBox>
#include <QLineEdit>

#include "Misc/Variables/cvariablesmodel.h"

#include "Misc/Variables/cscenecombobox.h"

#include "Properties/cprojectview.h"
#include "Misc/cscenemodel.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

CVariablesDelegate::CVariablesDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

QWidget *CVariablesDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    static QRegExpValidator validator(QRegExp("[a-zA-Z][a-zA-Z0-9_]*"));

    if(index.column() == 0)
    {
        CSceneComboBox *box = new CSceneComboBox(parent, shared().projectView->model());
        connect(box, SIGNAL(currentIndexChanged(int)), this, SLOT(PersistentEditorChanged()));
        return box;
    }
    else if(index.column() == 1)
    {
        QLineEdit *line = new QLineEdit(parent);
        line->setValidator(&validator);
        return line;
    }

    return new QLineEdit(parent);
}

void CVariablesDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.column() == 0)
    {
        QString value = index.model()->data(index, Qt::EditRole).toString();

        QComboBox *combo = static_cast<QComboBox *>(editor);
        combo->setCurrentText(value);
    }
    else
    {
        QString value = index.model()->data(index, Qt::EditRole).toString();

        QLineEdit *line = static_cast<QLineEdit *>(editor);
        line->setText(value);
    }
}

void CVariablesDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    CVariablesModel *sm = static_cast<CVariablesModel *>(model);

    if(index.column() == 0)
    {
        QComboBox *combo = static_cast<QComboBox *>(editor);
        if(combo->currentIndex() == 0)
            sm->setData(index, QVariant(), Qt::EditRole);
        else
            sm->setData(index, combo->currentText(), Qt::EditRole);
    }
    else
    {
        QLineEdit *line = static_cast<QLineEdit *>(editor);
        sm->setData(index, line->text(), Qt::EditRole);
    }
}

void CVariablesDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)

    editor->setGeometry(option.rect);
}

void CVariablesDelegate::PersistentEditorChanged()
{
    emit commitData(static_cast<QWidget *>(sender()));
}
