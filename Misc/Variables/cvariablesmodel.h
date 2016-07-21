#ifndef CVARIABLESMODEL_H
#define CVARIABLESMODEL_H

#include <QAbstractTableModel>

#include "Misc/Variables/cvariable.h"

class CVariablesModel : public QAbstractTableModel
{
public:
    explicit CVariablesModel(QObject *parent = Q_NULLPTR);


    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    virtual bool setData(const QModelIndex &index, const QVariant &variant, int role) Q_DECL_OVERRIDE;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;

    void setVariables(const QList<CVariable> &variables);
    void AddItem(const CVariable &item);
    void RemoveItem(const CVariable &item);

    void Reset();

    QList<CVariable> variables() const;

private:
    void RefactorBubbles(const CVariable &current, QString newname);

    QList<CVariable> m_variables;

};

#endif // CVARIABLESMODEL_H
