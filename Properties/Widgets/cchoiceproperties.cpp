#include "cchoiceproperties.h"

#include <QListView>
//#include <QPushButton>

#include "Misc/cchoicemodel.h"
#include "Bubbles/cchoicebubble.h"
#include "Bubbles/cchoice.h"


CChoiceProperties::CChoiceProperties(QWidget *parent)
    : CListPropertiesWidget(parent)
{}


void CChoiceProperties::setBubble(CBubble *bbl)
{
    CListPropertiesWidget::setBubble(bbl);

    m_choiceBubble = qgraphicsitem_cast<CChoiceBubble *>(bbl);
    if(m_choiceBubble)
    {
        m_modelView->setModel(m_choiceBubble->choices());
    }
    else
    {
        m_modelView->setModel(0);
    }
}

void CChoiceProperties::MoveUp()
{
    m_choiceBubble->choices()->MoveUp(m_modelView->currentIndex().row());
}

void CChoiceProperties::MoveDown()
{
    m_choiceBubble->choices()->MoveDown(m_modelView->currentIndex().row());
}

void CChoiceProperties::AddItem()
{
    m_choiceBubble->choices()->AddItem(new CChoice(CBubble::GenerateUID(), m_choiceBubble->getPalette(), m_choiceBubble->getFont(), m_choiceBubble));
    m_modelView->edit(QModelIndex(m_modelView->model()->index(m_modelView->model()->rowCount() - 1, 0)));
}


void CChoiceProperties::RemoveItem()
{
    m_choiceBubble->choices()->RemoveItem(m_modelView->currentIndex().row());
}
