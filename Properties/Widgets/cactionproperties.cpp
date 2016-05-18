#include "cactionproperties.h"

#include <QListView>
#include "Misc/cstringlistmodel.h"

#include "Bubbles/cactionbubble.h"


CActionProperties::CActionProperties(QWidget *parent)
    : CListPropertiesWidget(parent)
{}


void CActionProperties::setBubble(CBubble *bbl)
{
    CListPropertiesWidget::setBubble(bbl);

    m_actionBubble = qgraphicsitem_cast<CActionBubble *>(bbl);
    if(m_actionBubble)
    {
        m_modelView->setModel(m_actionBubble->actions());
    }
    else
    {
        m_modelView->setModel(0);
    }
}


void CActionProperties::MoveUp()
{
    const int index = m_modelView->currentIndex().row();
    m_actionBubble->actions()->MoveUp(index);
    m_modelView->setCurrentIndex(m_actionBubble->actions()->index(index - 1));
}

void CActionProperties::MoveDown()
{
    const int index = m_modelView->currentIndex().row();
    m_actionBubble->actions()->MoveDown(index);
    m_modelView->setCurrentIndex(m_actionBubble->actions()->index(index + 1));
}

void CActionProperties::AddItem()
{
    m_actionBubble->actions()->AddItem("");
    m_modelView->edit(QModelIndex(m_modelView->model()->index(m_modelView->model()->rowCount() - 1, 0)));
}

void CActionProperties::RemoveItem()
{
    m_actionBubble->actions()->RemoveItem(m_modelView->currentIndex().row());
}
