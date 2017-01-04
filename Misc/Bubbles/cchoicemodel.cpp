#include "cchoicemodel.h"

#include <QModelIndex>
#include <QModelIndexList>
#include <QMimeData>
#include <QByteArray>
#include <QDataStream>

#include "Bubbles/cchoice.h"


CChoiceModel::CChoiceModel(QObject *parent)
    : QAbstractListModel(parent)
{}

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
        return QVariant(m_choices[index.row()]->text());

    return QVariant();
}

bool CChoiceModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && (role == Qt::EditRole || role == Qt::DisplayRole))
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

Qt::DropActions CChoiceModel::supportedDragActions() const
{
    return Qt::MoveAction;
}

Qt::DropActions CChoiceModel::supportedDropActions() const
{
    return Qt::MoveAction;
}


bool CChoiceModel::insertRows(int row, int count, const QModelIndex &index)
{
    if (row < 0 || row >= rowCount(index) || index.isValid())
        return false;

    beginInsertRows(QModelIndex(), row, row + count - 1);

    CBubble *par = dynamic_cast<CBubble *>(parent());
    for(int i = row, end = row + count; i < end; ++i)
        m_choices.insert(i, new CChoice(par->getPaletteAction(), par->getFont(), par));

    endInsertRows();

    return true;
}

bool CChoiceModel::removeRows(int row, int count, const QModelIndex &)
{
    beginRemoveRows(QModelIndex(), row, row + count-1);

    for (int i = 0; i < count; ++i)
    {
        delete m_choices[row];
        m_choices.removeAt(row);
    }

    endRemoveRows();

    return true;
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

void CChoiceModel::MoveUp(const int index)
{
    if(index > 0 && index < m_choices.length())
    {
        beginMoveRows(QModelIndex(), index, index, QModelIndex(), index - 1);
        m_choices.swap(index, index - 1);
        endMoveRows();
    }
}

void CChoiceModel::MoveDown(const int row)
{
    if(row >= 0 && row < m_choices.length() - 1)
        MoveUp(row + 1);
}

void CChoiceModel::AddItem(CChoice *choice)
{
    const int index = m_choices.length();
    beginInsertRows(QModelIndex(), index, index);
    m_choices.append(choice);
    endInsertRows();
}

void CChoiceModel::RemoveItem(const int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    delete m_choices[row];
    m_choices.removeAt(row);
    endRemoveRows();
}
