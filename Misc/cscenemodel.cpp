#include "cscenemodel.h"

#include "cgraphicsview.h"
#include "cgraphicsscene.h"

CSceneModel::CSceneModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int CSceneModel::rowCount(const QModelIndex &) const
{
    return m_views.length();
}

QVariant CSceneModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_views.length())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return QVariant(m_views[index.row()]->cScene()->name());

    return QVariant();
}

bool CSceneModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && (role == Qt::EditRole || role == Qt::DisplayRole))
    {
        m_views[index.row()]->cScene()->setName(uniqueName(value.toString(), index.row()));

        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
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
    m_views = views;
    endResetModel();
}

QList<CGraphicsView *> CSceneModel::views()
{
    return m_views;
}

CGraphicsScene *CSceneModel::sceneWithName(const QString &name)
{
    for(CGraphicsView *view : m_views)
        if(view->cScene()->name() == name)
            return view->cScene();

    return Q_NULLPTR;
}

QString CSceneModel::uniqueName(const QString &scene, int row)
{
    int val = 1;
    QString name = scene;
    bool found = true;

    while(found)
    {
        found = false;
        for(int i = 0; i < m_views.length(); ++i)
        {
            if(i != row && m_views[i]->cScene()->name() == name)
            {
                name = scene + "_" + QString::number(++val);
                found = true;
                break;
            }
        }
    }

    return name;
}

void CSceneModel::MoveUp(const int index)
{
    if(index > 1 && index < m_views.length())
    {
        beginMoveRows(QModelIndex(), index, index, QModelIndex(), index - 1);
        m_views.swap(index, index - 1);
        endMoveRows();
    }
}

void CSceneModel::MoveDown(const int index)
{
    if(index > 0 && index < m_views.length() - 1)
        MoveUp(index + 1);
}

void CSceneModel::AddItem(CGraphicsView *view)
{
    const int index = m_views.length();

    beginInsertRows(QModelIndex(), index, index);
    view->cScene()->setName(uniqueName(view->cScene()->name(), -1));
    m_views.append(view);
    endInsertRows();
}

void CSceneModel::RemoveItem(const int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    delete m_views[index];
    m_views.removeAt(index);
    endRemoveRows();
}

void CSceneModel::Reset()
{
    beginResetModel();
    while(m_views.length())
        RemoveItem(0);
    endResetModel();
}
