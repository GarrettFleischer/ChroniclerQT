#include "cchoiceproperties.h"

#include <QListWidget>

#include "Bubbles/cchoicebubble.h"


CChoiceProperties::CChoiceProperties(QStringListModel *model, QWidget *parent)
    : CPropertiesWidget(model, parent)
{
    m_choices = new QListWidget();
    m_choices->setDragDropMode(QAbstractItemView::DragDrop);
    m_choices->setDefaultDropAction(Qt::MoveAction);

    m_choices->addItem("Code");
    m_choices->addItem("Eat");
    m_choices->addItem("Sleep");
    m_choices->addItem("In that order...");

    m_layout->addWidget(m_choices);
}


void CChoiceProperties::setBubble(CBubble *bbl)
{
    CPropertiesWidget::setBubble(bbl);

    m_choiceBubble = qgraphicsitem_cast<CChoiceBubble *>(bbl);
    if(m_choiceBubble)
    {
        // update properties...
    }
    else
    {

    }
}

void CChoiceProperties::setFont(const QFont &font)
{

}

void CChoiceProperties::ChoicesChanged(const QStringList &list)
{

}
