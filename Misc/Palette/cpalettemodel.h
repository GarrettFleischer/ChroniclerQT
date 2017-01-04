#ifndef CPALETTEMODEL_H
#define CPALETTEMODEL_H

#include <QAbstractListModel>

class CPaletteAction;

class CPaletteModel : public QAbstractListModel
{
public:
    CPaletteModel(QObject *parent = Q_NULLPTR);

    virtual int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;


private:
    QList<CPaletteAction *> m_actions;
};

#endif // CPALETTEMODEL_H
