#include "cchoiceproperties.h"

#include <QListWidget>

#include "Bubbles/cchoicebubble.h"


CChoiceProperties::CChoiceProperties(QStringListModel *model, QWidget *parent)
    : CPropertiesWidget(model, parent)
{
    m_choices = new QListWidget();
    m_choices->setDragDropMode(QAbstractItemView::DragDrop);
    m_choices->setDefaultDropAction(Qt::MoveAction);

    m_layout->addWidget(m_choices);
}


void CChoiceProperties::setBubble(CBubble *bbl)
{
    CPropertiesWidget::setBubble(bbl);

    m_choiceBubble = qgraphicsitem_cast<CChoiceBubble *>(bbl);
    if(m_choiceBubble)
    {
        m_choices->clear();
        m_choices->addItems(m_choiceBubble->getChoices());
    }
    else
    {
        m_choices->clear();
    }
}

void CChoiceProperties::setFont(const QFont &font)
{

}

void CChoiceProperties::ChoicesChanged(const QStringList &list)
{

}
