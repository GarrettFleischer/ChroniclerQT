#include "cactionproperties.h"

#include <QTableView>

#include "Misc/clistbuttons.h"

#include "Misc/Bubbles/cactionmodel.h"
#include "Misc/Bubbles/cactiondelegate.h"

#include "Bubbles/cactionbubble.h"



CActionProperties::CActionProperties(QWidget *parent)
    : CPropertiesWidget(parent), m_actionBubble(Q_NULLPTR)
{
    m_view = new QTableView();
    m_view->setItemDelegate(new CActionDelegate(this));
    m_view->setAlternatingRowColors(true);
    m_view->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    connect(m_view, SIGNAL(clicked(QModelIndex)), this, SLOT(SelectedChanged(QModelIndex)));


    CListButtons *btns = new CListButtons(this);
    connect(btns, SIGNAL(moveUp()), this, SLOT(MoveUp()));
    connect(btns, SIGNAL(moveDown()), this, SLOT(MoveDown()));
    connect(btns, SIGNAL(addItem()), this, SLOT(AddItem()));
    connect(btns, SIGNAL(removeItem()), this, SLOT(RemoveItem()));

    QHBoxLayout *hl_viewButtons = new QHBoxLayout();
    hl_viewButtons->addWidget(m_view);
    hl_viewButtons->addWidget(btns);

    m_layout->addLayout(hl_viewButtons);
}


void CActionProperties::setBubble(CBubble *bbl)
{
    CPropertiesWidget::setBubble(bbl);

    if(m_actionBubble)
    {
        disconnect(m_actionBubble->actions(), SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(OpenPersistentEditors(QModelIndex,int,int)));
        disconnect(m_actionBubble->actions(), SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)), this, SLOT(ClosePersistentEditors(QModelIndex,int,int)));
        disconnect(m_actionBubble->actions(), SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(DataChanged(QModelIndex,QModelIndex,QVector<int>)));
    }

    m_actionBubble = dynamic_cast<CActionBubble *>(bbl);
    if(m_actionBubble)
    {
        m_view->setModel(m_actionBubble->actions());
        connect(m_actionBubble->actions(), SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(OpenPersistentEditors(QModelIndex,int,int)));
        connect(m_actionBubble->actions(), SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)), this, SLOT(ClosePersistentEditors(QModelIndex,int,int)));
        connect(m_actionBubble->actions(), SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(DataChanged(QModelIndex,QModelIndex,QVector<int>)));

        if(m_actionBubble->actions()->rowCount())
            OpenPersistentEditors(QModelIndex(), 0, m_view->model()->rowCount() - 1);
    }
    else
        m_view->setModel(Q_NULLPTR);
}


void CActionProperties::MoveUp()
{
    const int row = m_view->currentIndex().row();
    m_actionBubble->actions()->MoveUp(row);
    m_view->setCurrentIndex(m_actionBubble->actions()->index(row - 1, 0));
    m_view->setFocus();
}

void CActionProperties::MoveDown()
{
    const int row = m_view->currentIndex().row();
    m_actionBubble->actions()->MoveDown(row);
    m_view->setCurrentIndex(m_actionBubble->actions()->index(row + 1, 0));
    m_view->setFocus();
}

void CActionProperties::AddItem()
{
    m_actionBubble->actions()->insertRow(m_actionBubble->actions()->rowCount());
    m_view->edit(QModelIndex(m_view->model()->index(m_view->model()->rowCount() - 1, 0)));

    m_view->setFocus();
}

void CActionProperties::RemoveItem()
{
    m_actionBubble->actions()->removeRow(m_view->currentIndex().row());
    m_view->setFocus();
}

void CActionProperties::OpenPersistentEditors(QModelIndex parent, int first, int last)
{
    Q_UNUSED(parent)

    for(int i = first; i <= last; ++i)
        for(int j = 0; j < m_view->model()->columnCount(); ++j)
            m_view->openPersistentEditor(m_view->model()->index(i, j));
}

void CActionProperties::ClosePersistentEditors(QModelIndex parent, int first, int last)
{
    Q_UNUSED(parent)

    for(int i = first; i <= last; ++i)
        for(int j = 0; j < m_view->model()->columnCount(m_view->model()->index(i, 0)); ++j)
            m_view->closePersistentEditor(m_view->model()->index(i, j));
}

void CActionProperties::DataChanged(QModelIndex first, QModelIndex last, QVector<int> roles)
{
    if(first.column() == 0 && roles.contains(Qt::EditRole))
    {
        ClosePersistentEditors(QModelIndex(), first.row(), last.row());
        OpenPersistentEditors(QModelIndex(), first.row(), last.row());
    }
}

