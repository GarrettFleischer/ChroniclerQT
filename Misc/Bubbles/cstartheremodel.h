#ifndef CSTARTHEREMODEL_H
#define CSTARTHEREMODEL_H

#include <QAbstractTableModel>

#include "Misc/Variables/cvariable.h"

class CStartHereModel : public QAbstractTableModel
{
public:
    CStartHereModel(QObject *parent = Q_NULLPTR);

    virtual int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    virtual int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;

    void AddItem(const CVariable &item);
    void RemoveItem(const CVariable &item);
    voie RemoveItemAt(const int row);

    QList<CVariable> variables() const;
    void setVariables(const QList<CVariable> &variables);

private:
    QList<CVariable> m_variables;
};

#endif // CSTARTHEREMODEL_H
