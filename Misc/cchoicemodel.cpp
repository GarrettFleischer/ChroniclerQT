#include "cchoicemodel.h"

#include <QModelIndex>

#include "Bubbles/cchoice.h"


CChoiceModel::CChoiceModel(const QList<CChoice *> &choices, QObject *parent)
    : QAbstractListModel(parent), m_choices(choices)
{}

int CChoiceModel::rowCount(const QModelIndex &) const
{
    return m_choices.length();
}

QVariant CChoiceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_choices.length())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return QVariant(m_choices[index.row()]->choice());

    return QVariant();
}

bool CChoiceModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        m_choices[index.row()]->setChoice(value.toString());

        emit dataChanged(index, index);
        return true;
    }

    return false;
}

Qt::ItemFlags CChoiceModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

    return flags | (index.isValid() ? Qt::ItemIsDragEnabled : Qt::ItemIsDropEnabled);
}


void CChoiceModel::setChoices(const QList<CChoice *> &choices)
{
    beginResetModel();
    m_choices = choices;
    endResetModel();
}

QList<CChoice *> CChoiceModel::choices()
{
    return m_choices;
}


Qt::DropActions CChoiceModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}
