#ifndef CSCENEMODEL_H
#define CSCENEMODEL_H

#include <QAbstractListModel>

class CGraphicsScene;

class CSceneModel : public QAbstractListModel
{
public:
    explicit CSceneModel(CGraphicsScene *startup, QObject *parent = 0);

    virtual int rowCount(const QModelIndex &index = QModelIndex()) const override;

    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

    void setScenes(const QList<CGraphicsScene *> &scenes);
    QList<CGraphicsScene *> scenes();

    void MoveUp(const int index);
    void MoveDown(const int index);
    void AddItem(CGraphicsScene *scene);
    void RemoveItem(const int index);

private:
    QList<CGraphicsScene *> m_scenes;

};

#endif // CSCENEMODEL_H
