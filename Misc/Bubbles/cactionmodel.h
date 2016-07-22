#ifndef CACTIONMODEL_H
#define CACTIONMODEL_H

#include <QAbstractTableModel>

class CActionModel : public QAbstractTableModel
{
public:
    CActionModel(QObject * parent = Q_NULLPTR);

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    virtual int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    virtual bool setData(const QModelIndex &index, const QVariant &variant, int role) Q_DECL_OVERRIDE;
    virtual bool insertRows(int row, int count, const QModelIndex &parent) Q_DECL_OVERRIDE;
    virtual bool removeRows(int row, int count, const QModelIndex &parent) Q_DECL_OVERRIDE;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    QList<QStringList> m_actions;
};

#endif // CACTIONMODEL_H
