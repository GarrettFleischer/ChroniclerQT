#include "cchoiceproperties.h"

#include <QListView>
#include "Misc/cstringlistmodel.h"

#include "Bubbles/cchoicebubble.h"


CChoiceProperties::CChoiceProperties(QStringListModel *model, QWidget *parent)
    : CPropertiesWidget(model, parent)
{
    m_choices = new QListView();
    m_choices->setDragDropMode(QAbstractItemView::InternalMove);
    m_choices->setDefaultDropAction(Qt::MoveAction);
    m_choices->setEditTriggers(QAbstractItemView::DoubleClicked);
    m_choices->setDragDropOverwriteMode(false);
    m_choices->setDropIndicatorShown(true);

    m_layout->addWidget(m_choices);
}


void CChoiceProperties::setBubble(CBubble *bbl)
{
    CPropertiesWidget::setBubble(bbl);

    m_choiceBubble = qgraphicsitem_cast<CChoiceBubble *>(bbl);
    if(m_choiceBubble)
    {
        m_choices->setModel(m_choiceBubble->choices());
    }
    else
    {
        m_choices->setModel(0);
    }
}
