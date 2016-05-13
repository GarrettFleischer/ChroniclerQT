#ifndef CCHOICEMODEL_H
#define CCHOICEMODEL_H

#include <QAbstractListModel>

class CChoice;

class CChoiceModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit CChoiceModel(const QList<CChoice *> &choices, QObject *parent = 0);

    virtual int rowCount(const QModelIndex &) const override;

    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

    void setChoices(const QList<CChoice *> &choices);
    QList<CChoice *> choices();

private:
    QList<CChoice *> m_choices;

    // QAbstractItemModel interface
public:
    virtual Qt::DropActions supportedDropActions() const override;
};

#endif // CCHOICEMODEL_H
