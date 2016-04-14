#include "cconditionproperties.h"

#include <QLabel>
#include <QLineEdit>
#include <QIntValidator>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStringListModel>

#include "Misc/clineedit.h"
#include "Bubbles/cconditionbubble.h"

CConditionProperties::CConditionProperties(QStringListModel *model, QWidget *parent)
    : CPropertiesWidget(model, parent), m_conditionEdit(0)
{
    // Condition Widget
    QLabel *lblConditionStart = new QLabel(tr("*if ("), this);
    QLabel *lblConditionEnd = new QLabel(tr(")"), this);
    m_conditionEdit = new CLineEdit(this, m_model);
    connect(m_conditionEdit, SIGNAL(textChanged()), this, SLOT(ConditionChanged()));

    // Condition Layout
    QHBoxLayout *lCondition = new QHBoxLayout();
    lCondition->addWidget(lblConditionStart);
    lCondition->addWidget(m_conditionEdit);
    lCondition->addWidget(lblConditionEnd);

    m_layout->addLayout(lCondition);
    m_layout->addStretch(1);

    setBubble(0);
}


void CConditionProperties::LabelChanged(QString label)
{
    if(m_bbl)
        m_bbl->m_label = label;
}

void CConditionProperties::ConditionChanged()
{
    if(m_bbl)
        m_bbl->SetCondition(m_conditionEdit->toPlainText());
}

void CConditionProperties::OrderChanged(QString order)
{
    if(m_bbl)
        m_bbl->m_order = order.toInt();
}

void CConditionProperties::LockedChanged(bool locked)
{
    if(m_bbl)
    {
        m_bbl->m_locked = locked;
        m_orderEdit->setEnabled(locked);
    }
}


void CConditionProperties::setBubble(CBubble *bbl)
{
    m_bbl = qgraphicsitem_cast<CConditionBubble *>(bbl);

    if(m_bbl)
    {
        setEnabled(true);
        
        m_labelEdit->setText(m_bbl->m_label);
        m_labelEdit->setFont(m_bbl->GetFont());
        
        m_conditionEdit->setText(m_bbl->m_conditionText);
        m_conditionEdit->setFont(m_bbl->GetFont());
        
        m_lockEdit->setChecked(m_bbl->m_locked);
        
        m_orderEdit->setText(QString().number(m_bbl->m_order));
        m_orderEdit->setEnabled(m_lockEdit->isChecked());
    }
    else
    {
        m_labelEdit->setText(tr(""));
        m_conditionEdit->setText(tr(""));
        m_lockEdit->setChecked(false);
        m_orderEdit->setText(tr(""));

        setEnabled(false);
    }
}


void CConditionProperties::setFont(const QFont &font)
{
    QWidget::setFont(font);
    m_labelEdit->setFont(font);
    m_conditionEdit->setFont(font);
    m_lockEdit->setFont(font);
    m_orderEdit->setFont(font);
}
