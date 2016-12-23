#include "cactiondelegate.h"

#include <QStringListModel>

#include "Misc/Bubbles/cactionmodel.h"
#include "Misc/Bubbles/cactionedit.h"
#include "Misc/cshighlighter.h"

CActionDelegate::CActionDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
    m_editor = new CActionEdit(Q_NULLPTR);
}

CActionDelegate::~CActionDelegate()
{
    m_editor->deleteLater();
}

QWidget *CActionDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    m_editor->setParent(parent);

    return m_editor;
}

void CActionDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{   
    if(index.isValid())
    {
        CActionEdit *actionEdit = static_cast<CActionEdit *>(editor);
        actionEdit->setText(index.data(Qt::EditRole).toString());
    }
}

void CActionDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.isValid())
    {
        QAbstractListModel *actionModel = static_cast<QAbstractListModel *>(model);

        CActionEdit *actionEdit = static_cast<CActionEdit *>(editor);
        actionModel->setData(index, actionEdit->toPlainText());
    }
}

void CActionDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)

    editor->setGeometry(option.rect);
}

void CActionDelegate::destroyEditor(QWidget *editor, const QModelIndex &index) const
{
    Q_UNUSED(index)

    editor->setParent(Q_NULLPTR);
}

void CActionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QTextDocument doc;
    new CSHighlighter(&doc);

    doc.setPlainText(index.data().toString());
    doc.drawContents(painter, option.rect);
}
