#include "cactionproperties.h"

#include <QTableView>

#include "Misc/clistbuttons.h"

#include "Misc/Bubbles/cactionmodel.h"
#include "Misc/Bubbles/cactiondelegate.h"

#include "Bubbles/cactionbubble.h"



CActionProperties::CActionProperties(QWidget *parent)
    : CPropertiesWidget(parent)
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

    m_actionBubble = qgraphicsitem_cast<CActionBubble *>(bbl);
    if(m_actionBubble)
        m_view->setModel(m_actionBubble->actions());
    else
        m_view->setModel(0);
}


void CActionProperties::MoveUp()
{
    const int row = m_view->currentIndex().row();
    m_actionBubble->actions()->MoveUp(row);
    m_view->setCurrentIndex(m_actionBubble->actions()->index(row - 1, 0));
}

void CActionProperties::MoveDown()
{
    const int row = m_view->currentIndex().row();
    m_actionBubble->actions()->MoveDown(row);
    m_view->setCurrentIndex(m_actionBubble->actions()->index(row + 1, 0));
}

void CActionProperties::AddItem()
{
    m_actionBubble->actions()->insertRow(m_actionBubble->actions()->rowCount());
    m_view->edit(QModelIndex(m_view->model()->index(m_view->model()->rowCount() - 1, 0)));
}

void CActionProperties::RemoveItem()
{
    m_actionBubble->actions()->removeRow(m_view->currentIndex().row());
}
