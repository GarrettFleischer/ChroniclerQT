#ifndef CCHOICEMODEL_H
#define CCHOICEMODEL_H

#include <QAbstractListModel>

#include "Misc/chronicler.h"
using Chronicler::CPalette;

class CChoice;

class CChoiceModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit CChoiceModel(const QList<CChoice *> &choices, QObject *parent = 0);

    virtual int rowCount(const QModelIndex &) const override;

    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    virtual bool insertRows(int row, int count, const QModelIndex &parent) override;
    virtual bool removeRows(int row, int count, const QModelIndex &) override;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual Qt::DropActions supportedDragActions() const override;
    virtual Qt::DropActions supportedDropActions() const override;

    void setChoices(const QList<CChoice *> &choices);
    QList<CChoice *> choices();

    void MoveUp(const int index);
    void MoveDown(const int index);
    void AddItem(CChoice *choice);
    void RemoveItem(const int index);

private:
    QList<CChoice *> m_choices;

};

#endif // CCHOICEMODEL_H
