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
    m_bbl = qgraphicsitem_cast<CChoiceBubble *>(bbl);

    if(m_bbl)
    {
        setEnabled(true);

        //m_labelEdit->setText(m_bbl->m_label);
        //m_labelEdit->setFont(m_bbl->GetFont());

        //m_conditionEdit->setText(m_bbl->m_conditionText);
        //m_conditionEdit->setFont(m_bbl->GetFont());

        //m_lockEdit->setChecked(m_bbl->m_locked);

        //m_orderEdit->setText(QString().number(m_bbl->m_order));
        //m_orderEdit->setEnabled(m_lockEdit->isChecked());
    }
    else
    {
//        m_labelEdit->setText(tr(""));
//        m_conditionEdit->setText(tr(""));
//        m_lockEdit->setChecked(false);
//        m_orderEdit->setText(tr(""));

        setEnabled(false);
    }
}

void CChoiceProperties::setFont(const QFont &font)
{
}

void CChoiceProperties::LabelChanged(QString label)
{
}

void CChoiceProperties::OrderChanged(QString order)
{
}

void CChoiceProperties::LockedChanged(bool locked)
{
}
