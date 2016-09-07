#include "cchoicedelegate.h"

#include <Misc/Bubbles/cchoiceedit.h>
#include "Misc/Bubbles/cchoicemodel.h"

CChoiceDelegate::CChoiceDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
    m_editor = new CChoiceEdit(Q_NULLPTR);
//    connect(m_editor, SIGNAL(textChanged()), this, SLOT(EditorUpdated()));
}

CChoiceDelegate::~CChoiceDelegate()
{
    m_editor->deleteLater();
}

QWidget *CChoiceDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    m_editor->setParent(parent);

    return m_editor;
}

void CChoiceDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.isValid())
    {
        CChoiceEdit *choiceEdit = static_cast<CChoiceEdit *>(editor);
        choiceEdit->setText(index.data(Qt::EditRole).toString());
    }
}

void CChoiceDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.isValid())
    {
        QAbstractListModel *choiceModel = static_cast<QAbstractListModel *>(model);

        CChoiceEdit *choiceEdit = static_cast<CChoiceEdit *>(editor);
        choiceModel->setData(index, choiceEdit->toPlainText());
    }
}

void CChoiceDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)

    editor->setGeometry(option.rect);
}

void CChoiceDelegate::destroyEditor(QWidget *editor, const QModelIndex &index) const
{
    Q_UNUSED(editor)
    Q_UNUSED(index)
}

void CChoiceDelegate::EditorUpdated()
{
    QTextCursor c(m_editor->textCursor());
    commitData(m_editor);
    m_editor->setTextCursor(c);
}
