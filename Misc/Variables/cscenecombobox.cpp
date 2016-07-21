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
        disconnect(model, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(UpdateModel()));
        disconnect(model, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(UpdateModel()));
    }

    m_model = model;
    UpdateModel();

    connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(UpdateModel()));
    connect(model, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(UpdateModel()));
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
