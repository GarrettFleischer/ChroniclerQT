#include "cvariablesmodel.h"

#include "cgraphicsscene.h"
#include "Properties/cprojectview.h"
#include "Misc/cscenemodel.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

CVariablesModel::CVariablesModel(QObject *parent)
    : QAbstractTableModel(parent)
{}


int CVariablesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_variables.length();
}

int CVariablesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 4;
}

QVariant CVariablesModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid())
    {
        if(index.column() == 0)
        {
            if(role == Qt::DisplayRole || role == Qt::EditRole)
            {
                if(m_variables[index.row()].scene)
                    return true;
                else
                    return false;
            }
        }
        else if(index.column() == 1)
        {
            if(role == Qt::DisplayRole || role == Qt::EditRole)
            {
                if(m_variables[index.row()].scene)
                    return m_variables[index.row()].scene->name();
                else
                    return "Global";
            }
            //            else if(role == Qt::EditRole)
            //                return QVariant::fromValue<CGraphicsScene *>(m_variables[index.row()].scene);
        }
        else if(index.column() == 2)
        {
            if(role == Qt::DisplayRole || role == Qt::EditRole)
                return m_variables[index.row()].name;
        }
        else
        {
            if(role == Qt::DisplayRole || role == Qt::EditRole)
                return m_variables[index.row()].data;
        }
    }

    return QVariant();
}

bool CVariablesModel::setData(const QModelIndex &index, const QVariant &variant, int role)
{
    if(index.isValid())
    {
        if(index.column() == 0)
        {
            if(!variant.toBool())
            {
                m_variables[index.row()].scene = Q_NULLPTR;
                return true;
            }
        }
        else if(index.column() == 1)
        {
            if(role == Qt::EditRole)
            {
                m_variables[index.row()].scene = shared().projectView->model()->sceneWithName(variant.toString());
                return true;
            }
        }
        else if(index.column() == 2)
        {
            if(role == Qt::EditRole)
            {
                m_variables[index.row()].name = variant.toString();
                return true;
            }
        }
        else
        {
            if(role == Qt::EditRole)
            {
                m_variables[index.row()].data = variant.toString();
                return true;
            }
        }
    }

    return false;
}

QVariant CVariablesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section == 0)
            return "Local";
        else if(section == 1)
            return "Scene";
        else if(section == 2)
            return "Name";
        else
            return "Data";
    }

    return QVariant();
}

Qt::ItemFlags CVariablesModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void CVariablesModel::AddItem(const Chronicler::CVariable &item)
{
    int row = rowCount();

    beginInsertRows(index(row, 0), row, row);
    m_variables.append(item);
    endInsertRows();
}

void CVariablesModel::RemoveItem(const Chronicler::CVariable &item)
{
    int row = m_variables.indexOf(item);

    beginRemoveRows(index(row, 0), row, row);
    m_variables.removeAll(item);
    endRemoveRows();
}

void CVariablesModel::RemoveItem(int row)
{
    beginRemoveRows(index(row, 0), row, row);
    m_variables.removeAt(row);
    endRemoveRows();
}
