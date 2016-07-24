#include "cscenecombobox.h"

#include <QStringListModel>
#include <QModelIndex>

#include "Misc/cscenemodel.h"

#include "cgraphicsview.h"
#include "cgraphicsscene.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

CSceneComboBox::CSceneComboBox(QWidget *parent, CSceneModel *model)
    : QComboBox(parent), m_model(Q_NULLPTR)
{
    setModel(model);
}

void CSceneComboBox::setModel(CSceneModel *model)
{
    if(m_model)
    {
        disconnect(m_model, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(UpdateModel()));
        disconnect(m_model, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(UpdateModel()));
        disconnect(m_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(ModelDataChanged(QModelIndex,QModelIndex,QVector<int>)));
    }

    m_model = model;

    if(m_model)
    {
        connect(m_model, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(UpdateModel()));
        connect(m_model, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(UpdateModel()));
        connect(m_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(ModelDataChanged(QModelIndex,QModelIndex,QVector<int>)));

        UpdateModel();
    }
}

void CSceneComboBox::UpdateModel()
{
    if(m_model)
    {
        QStringList lst;
        lst.append(shared().globalVariableTitle);

        for(CGraphicsView *view : m_model->views())
            lst.append(view->cScene()->name());

        QComboBox::setModel(new QStringListModel(lst, this));
    }
}

void CSceneComboBox::ModelDataChanged(QModelIndex first, QModelIndex last, QVector<int> roles)
{
    Q_UNUSED(roles)

    for(int row = first.row(); row <= last.row(); ++row)
        QComboBox::model()->setData(QComboBox::model()->index(row + 1, 0), first.model()->data(first.model()->index(row, 0)));
}
