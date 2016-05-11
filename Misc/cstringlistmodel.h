#ifndef CLISTMODEL_H
#define CLISTMODEL_H


#include <QStringListModel>

class CStringListModel : public QStringListModel
{
public:
    CStringListModel(QObject *parent = 0);
    CStringListModel(const QStringList &strings, QObject *parent = 0);

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // CLISTMODEL_H
