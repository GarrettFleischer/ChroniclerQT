#ifndef CVARIABLESVIEW_H
#define CVARIABLESVIEW_H

#include <QWidget>
#include "Misc/cserializable.h"

#include <QModelIndex>

QT_BEGIN_NAMESPACE
class QTableView;
class QSortFilterProxyModel;
class QPushButton;
QT_END_NAMESPACE

class CVariablesModel;
class CVariable;
class CGraphicsScene;

class CVariablesView : public QWidget, public CSerializable
{
    Q_OBJECT
public:
    explicit CVariablesView(QWidget *parent = 0);

    virtual ~CVariablesView();

    void Reset();

    CVariablesModel *model() const;

    QList<CVariable> getVariablesForScene(CGraphicsScene *scene);

protected:
    virtual QDataStream &Deserialize(QDataStream &ds, const CVersion &version) Q_DECL_OVERRIDE;
    virtual QDataStream &Serialize(QDataStream &ds) Q_DECL_OVERRIDE;

private:
    QTableView              *m_view;
    CVariablesModel         *m_model;
    QSortFilterProxyModel   *m_sortModel;

private slots:
    void RowsInserted(QModelIndex parent, int first, int last);
    void RowsAboutToBeRemoved(QModelIndex parent, int first, int last);

    void AddItem();
    void RemoveItem();

};

#endif // CVARIABLESVIEW_H
