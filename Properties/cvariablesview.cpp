#include "cvariablesview.h"

#include <QVBoxLayout>

#include <QTableView>
#include <QSortFilterProxyModel>
#include <QHeaderView>


#include "Misc/cvariablesmodel.h"
#include "Misc/cvariablesdelegate.h"

CVariablesView::CVariablesView(QWidget *parent)
    : QWidget(parent)
{
    m_model = new CVariablesModel(this);
    m_sortModel = new QSortFilterProxyModel(this);
    m_sortModel->setDynamicSortFilter(true);
    m_sortModel->setSourceModel(m_model);

//    connect(m_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(ModelChanged()));

    m_view = new QTableView(this);
    m_view->setAlternatingRowColors(true);
    m_view->setSortingEnabled(true);
    m_view->setModel(m_sortModel);
    m_view->setItemDelegate(new CVariablesDelegate(this));

    QVBoxLayout *vl_main = new QVBoxLayout(this);
    vl_main->addWidget(m_view);

    m_model->AddItem(CVariable("name", "John", Q_NULLPTR));
    m_model->AddItem(CVariable("occupation", "Smith", Q_NULLPTR));
    m_model->AddItem(CVariable("age", "23", Q_NULLPTR));
//    m_view->openPersistentEditor(m_sortModel->index(0, 0));
//    m_view->openPersistentEditor(m_sortModel->index(1, 0));
//    m_view->openPersistentEditor(m_sortModel->index(2, 0));
}

void CVariablesView::ModelChanged()
{
//    m_view->sortByColumn(0, Qt::AscendingOrder);
}
