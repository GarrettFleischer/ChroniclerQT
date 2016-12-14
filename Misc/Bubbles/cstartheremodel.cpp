#include "cstartheremodel.h"

CStartHereModel::CStartHereModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

int CStartHereModel::rowCount(const QModelIndex &parent = QModelIndex()) const
{
    Q_UNUSED(parent)

    return m_variables.length();
}

int CStartHereModel::columnCount(const QModelIndex &parent = QModelIndex()) const
{
    Q_UNUSED(parent)

    return 2;
}

QVariant CStartHereModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::EditRole || role == Qt::DisplayRole)
    {
        if(index.column() == 0)
            return m_variables[index.row()].name();
        else if(index.column() == 1)
            return m_variables[index.row()].data();
    }

    return QVariant();
}

bool CStartHereModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::EditRole)
    {
        if(index.column() == 0)
        {
            m_variables[index.row()].setName(value.toString());
            return true;
        }
        else if(index.column() == 1)
        {
            m_variables[index.row()].setData(value.toString());
            return true;
        }
    }

    return false;
}

QVariant CStartHereModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section == 0)
            return "Name";
        else if(section == 1)
            return "Data";
    }

    return QVariant();
}

void CStartHereModel::AddItem(const CVariable &item)
{
    int row = rowCount();

    beginInsertRows(QModelIndex(), row, row);
    m_variables.append(item);
    endInsertRows();
}

void CStartHereModel::RemoveItem(const CVariable &item)
{
    int row = m_variables.indexOf(item);

    beginRemoveRows(QModelIndex(), row, row);
    m_variables.removeAt(row);
    endRemoveRows();
}

void CStartHereModel::RemoveItemAt(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    m_variables.removeAt(row);
    endRemoveRows();
}

QList<CVariable> CStartHereModel::variables() const
{
    return m_variables;
}

void CStartHereModel::setVariables(const QList<CVariable> &variables)
{
    beginResetModel();
    m_variables = variables;
    endResetModel();
}


Qt::ItemFlags CStartHereModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}
