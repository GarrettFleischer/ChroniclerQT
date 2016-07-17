#ifndef CVARIABLESDELEGATE_H
#define CVARIABLESDELEGATE_H

#include <QStyledItemDelegate>

QT_BEGIN_NAMESPACE
class QComboBox;
class QLineEdit;
QT_END_NAMESPACE

class CSceneComboBox;

class CVariablesDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit CVariablesDelegate(QObject *parent = Q_NULLPTR);
    ~CVariablesDelegate();


    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual void destroyEditor(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;

    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const Q_DECL_OVERRIDE;

    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    CSceneComboBox *m_sceneEditor;
    QLineEdit *m_lineEditor;

private slots:
    void PersistantEditorModified(QWidget *editor, const QModelIndex &index);

};

#endif // CVARIABLESDELEGATE_H
