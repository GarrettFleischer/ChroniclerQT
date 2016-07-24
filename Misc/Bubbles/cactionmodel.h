#ifndef CACTIONMODEL_H
#define CACTIONMODEL_H

#include <QAbstractTableModel>

class CActionModel : public QAbstractTableModel
{
public:
    CActionModel(QObject * parent = Q_NULLPTR);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    virtual bool setData(const QModelIndex &index, const QVariant &variant, int role) Q_DECL_OVERRIDE;
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    QList<QStringList> actions() const;
    void setActions(const QList<QStringList> &actions);

    QList<QString> stringList() const;

    void MoveUp(const int row);
    void MoveDown(const int row);


private:
    QList<QStringList> m_actions;
};

#endif // CACTIONMODEL_H
