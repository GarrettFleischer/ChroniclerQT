#ifndef CCHOICEDELEGATE_H
#define CCHOICEDELEGATE_H

#include <QStyledItemDelegate>

class CChoiceEdit;

class CChoiceDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    CChoiceDelegate(QObject *parent = Q_NULLPTR);
    ~CChoiceDelegate();

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual void destroyEditor(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    CChoiceEdit *m_editor;

private slots:
    void EditorUpdated();
};

#endif // CCHOICEDELEGATE_H
