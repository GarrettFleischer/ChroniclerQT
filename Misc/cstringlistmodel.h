#ifndef CSTRINGLISTMODEL_H
#define CSTRINGLISTMODEL_H


#include <QStringListModel>

class CStringListModel : public virtual QStringListModel
{
public:
    CStringListModel(QObject *parent = 0);
    CStringListModel(const QStringList &strings, QObject *parent = 0);

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    void MoveUp(const int index);
    void MoveDown(const int index);
    void AddItem(const QString &action);
    void RemoveItem(const int index);
};

#endif // CSTRINGLISTMODEL_H
