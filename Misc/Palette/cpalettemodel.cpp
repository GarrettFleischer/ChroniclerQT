#include "cpalettemodel.h"

#include "cpaletteaction.h"
#include "Misc/chronicler.h"
using Chronicler::shared;
using Chronicler::CPalette;

CPaletteModel::CPaletteModel(QObject *parent)
    : QAbstractListModel(parent)
{}


int CPaletteModel::rowCount(const QModelIndex &parent) const
{
    return m_actions.length();
}

QVariant CPaletteModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_actions.length())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return QVariant(m_actions[index.row()]->text());

    return QVariant();
}

bool CPaletteModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && (role == Qt::EditRole || role == Qt::DisplayRole))
    {
        m_actions[index.row()]->setText(value.toString());

        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
        return true;
    }

    return false;
}

Qt::ItemFlags CPaletteModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}
