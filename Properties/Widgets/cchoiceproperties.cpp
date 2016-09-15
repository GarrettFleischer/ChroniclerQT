#include "cchoiceproperties.h"

#include <QListView>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "Misc/Bubbles/cchoicemodel.h"
#include "Misc/Bubbles/cchoicedelegate.h"
#include "Bubbles/cchoicebubble.h"
#include "Bubbles/cchoice.h"
#include "Misc/clistbuttons.h"



CChoiceProperties::CChoiceProperties(QWidget *parent)
    : CPropertiesWidget(parent)
{
    CListButtons *buttons = new CListButtons(this);
    connect(buttons, SIGNAL(moveUp()), this, SLOT(MoveUp()));
    connect(buttons, SIGNAL(moveDown()), this, SLOT(MoveDown()));
    connect(buttons, SIGNAL(addItem()), this, SLOT(AddItem()));
    connect(buttons, SIGNAL(removeItem()), this, SLOT(RemoveItem()));

    m_view = new QListView();
    m_view->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    m_view->setItemDelegate(new CChoiceDelegate(this));

    QHBoxLayout *hl_viewButtons = new QHBoxLayout();
    hl_viewButtons->addWidget(m_view);
    hl_viewButtons->addWidget(buttons);

    m_layout->addLayout(hl_viewButtons);
}


void CChoiceProperties::setBubble(CBubble *bbl)
{
    CPropertiesWidget::setBubble(bbl);

    m_choiceBubble = dynamic_cast<CChoiceBubble *>(bbl);
    if(m_choiceBubble)
    {
        m_view->setModel(m_choiceBubble->choices());
    }
    else
    {
        m_view->setModel(Q_NULLPTR);
    }
}

void CChoiceProperties::MoveUp()
{
    m_choiceBubble->choices()->MoveUp(m_view->currentIndex().row());
}

void CChoiceProperties::MoveDown()
{
    m_choiceBubble->choices()->MoveDown(m_view->currentIndex().row());
}

void CChoiceProperties::AddItem()
{
    m_choiceBubble->choices()->AddItem(new CChoice(m_choiceBubble->getPalette(), m_choiceBubble->getFont(), m_choiceBubble));
    m_view->edit(QModelIndex(m_view->model()->index(m_view->model()->rowCount() - 1, 0)));
}


void CChoiceProperties::RemoveItem()
{
    m_choiceBubble->choices()->RemoveItem(m_view->currentIndex().row());
}
