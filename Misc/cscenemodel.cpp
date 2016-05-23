#include "cscenemodel.h"

#include "cgraphicsview.h"
#include "cgraphicsscene.h"

CSceneModel::CSceneModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int CSceneModel::rowCount(const QModelIndex &) const
{
    return m_scenes.length();
}

QVariant CSceneModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_scenes.length())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return QVariant(m_scenes[index.row()]->cScene()->name());

    return QVariant();
}

bool CSceneModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && (role == Qt::EditRole || role == Qt::DisplayRole))
    {
        m_scenes[index.row()]->cScene()->setName(value.toString());

        emit dataChanged(index, index);
        return true;
    }

    return false;
}

Qt::ItemFlags CSceneModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | ((index.row() > 0) ? Qt::ItemIsEditable : Qt::NoItemFlags);
}


void CSceneModel::setViews(const QList<CGraphicsView *> &views)
{
    beginResetModel();
    m_scenes = views;
    endResetModel();
}

QList<CGraphicsView *> CSceneModel::views()
{
    return m_scenes;
}

void CSceneModel::MoveUp(const int index)
{
    if(index > 1 && index < m_scenes.length())
    {
        beginMoveRows(QModelIndex(), index, index, QModelIndex(), index - 1);
        m_scenes.swap(index, index - 1);
        endMoveRows();
    }
}

void CSceneModel::MoveDown(const int index)
{
    if(index > 0 && index < m_scenes.length() - 1)
        MoveUp(index + 1);
}

void CSceneModel::AddItem(CGraphicsView *view)
{
    const int index = m_scenes.length();
    beginInsertRows(QModelIndex(), index, index);
    m_scenes.append(view);
    endInsertRows();
}

void CSceneModel::RemoveItem(const int index)
{
    //if(index > 0)
    //{
        beginRemoveRows(QModelIndex(), index, index);
        delete m_scenes[index];
        m_scenes.removeAt(index);
        endRemoveRows();
    //}
}
