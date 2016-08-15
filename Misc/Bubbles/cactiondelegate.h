#ifndef CACTIONDELEGATE_H
#define CACTIONDELEGATE_H

#include <QStyledItemDelegate>

QT_BEGIN_NAMESPACE
class QStringListModel;
QT_END_NAMESPACE

class CActionEdit;

class CActionDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    CActionDelegate(QObject *parent = Q_NULLPTR);
    virtual ~CActionDelegate();

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual void destroyEditor(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    CActionEdit *m_editor;
    QStringListModel *m_emptyModel;

};

#endif // CACTIONDELEGATE_H
