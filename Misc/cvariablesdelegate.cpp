#include "cvariablesdelegate.h"

#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>

#include <QPainter>
#include <QBrush>

#include "Properties/cprojectview.h"
#include "Misc/cscenemodel.h"
#include "Misc/cvariablesmodel.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

CVariablesDelegate::CVariablesDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
    m_localEditor = new QCheckBox();

    m_sceneEditor = new QComboBox();
    m_sceneEditor->setModel(shared().projectView->model());

    m_lineEditor = new QLineEdit();
}

CVariablesDelegate::~CVariablesDelegate()
{
    m_sceneEditor->deleteLater();
    m_localEditor->deleteLater();
    m_lineEditor->deleteLater();
}


QWidget *CVariablesDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    if(index.column() == 0)
    {
        m_localEditor->setParent(parent);
        return m_localEditor;
    }
    else if(index.column() == 1)
    {
        m_sceneEditor->setParent(parent);
        return m_sceneEditor;
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
        bool value = index.model()->data(index, Qt::EditRole).toBool();

        QCheckBox *check = static_cast<QCheckBox *>(editor);
        check->setChecked(value);
    }
    else if(index.column() == 1)
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
        QCheckBox *check = static_cast<QCheckBox *>(editor);
        sm->setData(index, check->isChecked(), Qt::EditRole);
    }
    else if(index.column() == 1)
    {
        QComboBox *combo = static_cast<QComboBox *>(editor);
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


void CVariablesDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 0)
    {
        if(index.data(Qt::DisplayRole).toBool())
        {
            painter->setBrush(QBrush(Qt::red));
            painter->drawRect(option.rect);
        }
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}
