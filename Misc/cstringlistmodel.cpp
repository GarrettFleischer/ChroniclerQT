#include "cstringlistmodel.h"

CStringListModel::CStringListModel(QObject *parent)
    : QStringListModel(parent)
{}

CStringListModel::CStringListModel(const QStringList &strings, QObject *parent)
    : QStringListModel(strings, parent)
{}

Qt::ItemFlags CStringListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

    return flags | (index.isValid() ? Qt::ItemIsDragEnabled : Qt::ItemIsDropEnabled);
}


void CStringListModel::MoveUp(const int index)
{
    if(index > 0 && index < stringList().length())
    {
        QStringList lst = stringList();
        lst.swap(index, index - 1);
        setStringList(lst);
    }
}

void CStringListModel::MoveDown(const int index)
{
    if(index >= 0 && index < stringList().length() - 1)
        MoveUp(index + 1);
}

void CStringListModel::AddItem(const QString &action)
{
    QStringList lst = stringList();
    lst.append(action);
    setStringList(lst);
}

void CStringListModel::RemoveItem(const int index)
{
    QStringList lst = stringList();
    lst.removeAt(index);
    setStringList(lst);
}
