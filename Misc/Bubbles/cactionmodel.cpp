#include "cactionmodel.h"

CActionModel::CActionModel(QObject *parent)
    : QAbstractTableModel(parent)
{}


int CActionModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_actions.length();
}

int CActionModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 4;
}

QVariant CActionModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole || Qt::EditRole)
        return m_actions[index.row()][index.column()];

    return QVariant();
}

bool CActionModel::setData(const QModelIndex &index, const QVariant &variant, int role)
{
    if(role == Qt::DisplayRole || Qt::EditRole)
    {
        m_actions[index.row()][index.column()] = variant.toString();

        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
        return true;
    }

    return false;
}

bool CActionModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)

    for(int i = row; i < row + count; ++i)
        m_actions.insert(i, QStringList());

    return true;
}

bool CActionModel::removeRows(int row, int count, const QModelIndex &parent)
{

    Q_UNUSED(parent)

    for(int i = row; i < row + count; ++i)
        m_actions.removeAt(i);

    return true;
}

Qt::ItemFlags CActionModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index) // TODO use index.column() = 0 to return correct ItemIsEnabled flag

    return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}
