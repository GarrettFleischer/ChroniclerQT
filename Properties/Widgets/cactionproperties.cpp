#include "cactionproperties.h"

#include <QListView>
#include "Misc/cstringlistmodel.h"

#include "Bubbles/cactionbubble.h"


CActionProperties::CActionProperties(QStringListModel *model, QWidget *parent)
    : CPropertiesWidget(model, parent)
{
    m_actions = new QListView();
    m_actions->setDragDropMode(QAbstractItemView::InternalMove);
    m_actions->setDefaultDropAction(Qt::MoveAction);
    m_actions->setEditTriggers(QAbstractItemView::DoubleClicked);
    m_actions->setDragDropOverwriteMode(false);
    m_actions->setDropIndicatorShown(true);

    m_layout->addWidget(m_actions);
}


void CActionProperties::setBubble(CBubble *bbl)
{
    CPropertiesWidget::setBubble(bbl);

    m_actionBubble = qgraphicsitem_cast<CActionBubble *>(bbl);
    if(m_actionBubble)
    {
        m_actions->setModel(m_actionBubble->actions());
    }
    else
    {
        m_actions->setModel(0);
    }
}
