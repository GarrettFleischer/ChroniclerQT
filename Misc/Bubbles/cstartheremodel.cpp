#include "cstartheremodel.h"

CStartHereModel::CStartHereModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

int CStartHereModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_variables.length();
}

int CStartHereModel::columnCount(const QModelIndex &parent) const
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
    Q_UNUSED(orientation)

    if(role == Qt::DisplayRole)
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
    m_variables.append(item);
}

void CStartHereModel::RemoveItem(const CVariable &item)
{
    m_variables.removeAll(item);
}

voie CStartHereModel::RemoveItemAt(const int row)
{
    m_variables.removeAt(row);
}
