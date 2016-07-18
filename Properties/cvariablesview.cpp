#include "cvariablesview.h"

#include <QVBoxLayout>

#include <QTableView>
#include <QSortFilterProxyModel>
#include <QHeaderView>


#include "Misc/cvariablesmodel.h"
#include "Misc/cvariablesdelegate.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

CVariablesView::CVariablesView(QWidget *parent)
    : QWidget(parent)
{
    m_model = new CVariablesModel(this);

    m_sortModel = new QSortFilterProxyModel(this);
    m_sortModel->setDynamicSortFilter(true);
    m_sortModel->setSourceModel(m_model);
    connect(m_sortModel, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(RowsInserted(QModelIndex,int,int)));
    connect(m_sortModel, SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)), this, SLOT(RowsAboutToBeRemoved(QModelIndex,int,int)));

    m_view = new QTableView(this);
    m_view->setAlternatingRowColors(true);
    m_view->setSortingEnabled(true);
    m_view->setModel(m_sortModel);
    m_view->setItemDelegate(new CVariablesDelegate(this));

    QVBoxLayout *vl_main = new QVBoxLayout(this);
    vl_main->addWidget(m_view);
}

CVariablesView::~CVariablesView()
{
    Reset();
}

void CVariablesView::Reset()
{
    for(int i = 0; i < m_sortModel->rowCount(); ++i)
        m_view->closePersistentEditor(m_sortModel->index(i, 0));

    m_model->Reset();
}

CVariablesModel *CVariablesView::model() const
{
    return m_model;
}

QDataStream &CVariablesView::Deserialize(QDataStream &ds, const QString &version)
{
    if(shared().versionToInt(version) > 920)
    {
        qint64 count;
        ds >> count;

        for(qint64 i = 0; i < count; ++i)
        {
            CVariable v;
            ds >> v;
            m_model->AddItem(v);
        }
    }

    m_view->sortByColumn(0);

    return ds;
}

QDataStream &CVariablesView::Serialize(QDataStream &ds)
{
    ds << static_cast<qint64>(m_model->rowCount());
    for(CVariable &v : m_model->variables())
        ds << v;

    return ds;
}

void CVariablesView::RowsInserted(QModelIndex parent, int first, int last)
{
    Q_UNUSED(parent)

    for(int i = first; i <= last; ++i)
        m_view->openPersistentEditor(m_sortModel->index(i, 0));
}

void CVariablesView::RowsAboutToBeRemoved(QModelIndex parent, int first, int last)
{
    Q_UNUSED(parent)

    for(int i = first; i <= last; ++i)
        m_view->closePersistentEditor(m_sortModel->index(i, 0));
}
