#include "cchoiceproperties.h"

#include <QListView>
#include <QPushButton>

#include "Misc/cchoicemodel.h"
#include "Bubbles/cchoicebubble.h"


CChoiceProperties::CChoiceProperties(QStringListModel *model, QWidget *parent)
    : CPropertiesWidget(model, parent)
{
    m_choices = new QListView();
    //    m_choices->setDragDropMode(QAbstractItemView::InternalMove);
    //    m_choices->setDefaultDropAction(Qt::MoveAction);
    m_choices->setEditTriggers(QAbstractItemView::DoubleClicked);
    //    m_choices->setDragDropOverwriteMode(false);
    //    m_choices->setDropIndicatorShown(true);

    QPushButton *btn = new QPushButton("up");
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(UpPressed()));
    m_layout->addWidget(btn);

    btn = new QPushButton("down");
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(DownPressed()));
    m_layout->addWidget(btn);

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

void CChoiceProperties::UpPressed()
{
    m_choiceBubble->choices()->MoveUp(m_choices->currentIndex().row());
}

void CChoiceProperties::DownPressed()
{
    m_choiceBubble->choices()->MoveDown(m_choices->currentIndex().row());
}
