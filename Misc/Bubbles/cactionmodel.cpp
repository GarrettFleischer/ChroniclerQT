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
    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if(index.isValid() && index.column() < m_actions[index.row()].length())
            return m_actions[index.row()][index.column()];
    }

    return QVariant();
}

bool CActionModel::setData(const QModelIndex &index, const QVariant &variant, int role)
{
    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if(index.isValid() && index.column() < m_actions[index.row()].length())
        {
            m_actions[index.row()][index.column()] = variant.toString();

            if(index.column() == 0)
            {
                for(int i = 1; i < m_actions[index.row()].length(); ++i)
                    m_actions[index.row()][i] = "";

                emit dataChanged(index, CActionModel::index(index.row(), columnCount() - 1), {Qt::DisplayRole, Qt::EditRole});
            }
            else
                emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

            return true;
        }
    }

    return false;
}

bool CActionModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)

    beginInsertRows(QModelIndex(), row, row + count - 1);

    for(int i = row; i < row + count; ++i)
        m_actions.insert(i, QStringList({"", "", "", ""}));

    endInsertRows();

    return true;
}

bool CActionModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for(int i = row; i < row + count; ++i)
        m_actions.removeAt(i);

    endRemoveRows();

    return true;
}

Qt::ItemFlags CActionModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index) // TODO use first column to return correct ItemIsEnabled flag

    return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}

QList<QStringList> CActionModel::actions() const
{
    return m_actions;
}

void CActionModel::setActions(const QList<QStringList> &actions)
{

    m_actions = actions;
}

QStringList CActionModel::stringList() const
{
    QStringList lst;

    for(int i = 0; i < m_actions.length(); ++i)
    {
        lst.append("");

        for(const QString &str : m_actions[i])
            lst[i] += str + " ";
    }

    return lst;
}

void CActionModel::MoveUp(const int row)
{
    if(row > 0 && row < m_actions.length())
    {
        beginMoveRows(QModelIndex(), row, row, QModelIndex(), row - 1);
        m_actions.swap(row, row - 1);
        endMoveRows();
    }
}

void CActionModel::MoveDown(const int row)
{
    if(row >= 0 && row < m_actions.length() - 1)
        MoveUp(row + 1);
}

void CActionModel::ResetRow()
{

}


QVariant CActionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(role)

    return "";
}
