#include "cactionproperties.h"

#include <QListView>

#include "Misc/clistbuttons.h"

#include "Misc/cstringlistmodel.h"
#include "Misc/Bubbles/cactiondelegate.h"

#include "Bubbles/cactionbubble.h"


CActionProperties::CActionProperties(QWidget *parent)
    : CPropertiesWidget(parent), m_actionBubble(Q_NULLPTR)
{
    m_view = new QListView();
    m_view->setItemDelegate(new CActionDelegate(this));
    m_view->setAlternatingRowColors(true);
    m_view->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);


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

    m_actionBubble = dynamic_cast<CActionBubble *>(bbl);
    if(m_actionBubble)
        m_view->setModel(m_actionBubble->actions());
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
    m_actionBubble->actions()->AddItem("");

    m_view->setFocus();
    m_view->edit(m_view->currentIndex());
}

void CActionProperties::RemoveItem()
{
    m_actionBubble->actions()->RemoveItem(m_view->currentIndex().row());
    m_view->setFocus();
}

