#include "cvariablesdelegate.h"

//#include <QComboBox>
#include <QLineEdit>

#include "Misc/cvariablesmodel.h"

#include "Misc/cscenecombobox.h"

#include "Properties/cprojectview.h"
#include "Misc/cscenemodel.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

CVariablesDelegate::CVariablesDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
    m_sceneEditor = new CSceneComboBox();
    m_sceneEditor->setModel(shared().projectView->model());

    m_lineEditor = new QLineEdit();
}

CVariablesDelegate::~CVariablesDelegate()
{
    m_sceneEditor->deleteLater();
    m_lineEditor->deleteLater();
}


QWidget *CVariablesDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    if(index.column() == 0)
    {
//        m_sceneEditor->setParent(parent);
//        return m_sceneEditor;
        return new CSceneComboBox(parent, shared().projectView->model());
    }
    else
    {
        m_lineEditor->setParent(parent);
        return m_lineEditor;
    }
}

void CVariablesDelegate::destroyEditor(QWidget *editor, const QModelIndex &index) const
{
    Q_UNUSED(editor)
    Q_UNUSED(index)
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
