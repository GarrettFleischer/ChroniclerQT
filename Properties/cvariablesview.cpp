#include "cvariablesview.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include <QTableView>
#include <QSortFilterProxyModel>


#include "Misc/cvariablesmodel.h"
#include "Misc/cvariablesdelegate.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

CVariablesView::CVariablesView(QWidget *parent)
    : QWidget(parent)
{
    m_addButton = new QPushButton(QIcon(":/images/icn_add"), "");
    m_addButton->setToolTip("Add new item");
    connect(m_addButton, SIGNAL(clicked(bool)), this, SLOT(AddItem()));

    m_removeButton = new QPushButton(QIcon(":/images/icn_trash"), "");
    m_removeButton->setToolTip("<qt>Delete item.<br>This cannot be undone!</qt>");
    connect(m_removeButton, SIGNAL(clicked(bool)), this, SLOT(RemoveItem()));

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

    QVBoxLayout *vl_buttons = new QVBoxLayout();
    vl_buttons->addWidget(m_addButton);
    vl_buttons->addWidget(m_removeButton);
    vl_buttons->addStretch(1);

    QHBoxLayout *hl_main = new QHBoxLayout(this);
    hl_main->addWidget(m_view);
    hl_main->addLayout(vl_buttons);
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

void CVariablesView::AddItem()
{
    m_model->AddItem(CVariable());
}

void CVariablesView::RemoveItem()
{
    if(m_view->currentIndex().isValid())
    {
//        m_view->closePersistentEditor(m_sortModel->index(m_view->currentIndex().row(), 0));
        m_sortModel->removeRow(m_view->currentIndex().row());
    }
}
