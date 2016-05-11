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
