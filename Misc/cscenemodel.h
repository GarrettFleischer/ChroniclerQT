#ifndef CSCENEMODEL_H
#define CSCENEMODEL_H

#include <QAbstractListModel>

class CGraphicsView;
class CGraphicsScene;

class CSceneModel : public QAbstractListModel
{
public:
    explicit CSceneModel(QObject *parent = 0);

    virtual int rowCount(const QModelIndex &index = QModelIndex()) const override;

    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

    void setViews(const QList<CGraphicsView *> &views);
    QList<CGraphicsView *> views();

    CGraphicsScene *sceneWithName(const QString &name);

    void MoveUp(const int index);
    void MoveDown(const int index);
    void AddItem(CGraphicsView *view);
    void RemoveItem(const int index);

    void Reset();

private:
    QList<CGraphicsView *> m_views;

};

#endif // CSCENEMODEL_H
