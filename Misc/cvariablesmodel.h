#ifndef CVARIABLESMODEL_H
#define CVARIABLESMODEL_H

#include <QAbstractTableModel>

#include "Misc/cvariable.h"

class CVariablesModel : public QAbstractTableModel
{
public:
    explicit CVariablesModel(QObject *parent = Q_NULLPTR);


    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &variant, int role) override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

    void AddItem(const CVariable &item);
    void RemoveItem(const CVariable &item);
    void RemoveItem(const int row);

    void Reset();

    QList<CVariable> variables() const;

private:
    QList<CVariable> m_variables;

};

#endif // CVARIABLESMODEL_H
