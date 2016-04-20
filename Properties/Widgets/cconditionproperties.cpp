#include "cconditionproperties.h"

#include <QLabel>
#include <QHBoxLayout>

#include "Misc/clineedit.h"
#include "Bubbles/cconditionbubble.h"

CConditionProperties::CConditionProperties(QStringListModel *model, QWidget *parent)
    : CPropertiesWidget(model, parent), m_conditionBubble(0), m_conditionEdit(0)
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


void CConditionProperties::setBubble(CBubble *bbl)
{
    CPropertiesWidget::setBubble(bbl);

    m_conditionBubble = qgraphicsitem_cast<CConditionBubble *>(bbl);
    if(m_conditionBubble)
    {
        m_conditionEdit->setText(m_conditionBubble->getCondition());
        m_conditionEdit->setFont(m_conditionBubble->getFont());
    }
    else
    {
        m_conditionEdit->setText(tr(""));
    }
}


void CConditionProperties::setFont(const QFont &font)
{
    CPropertiesWidget::setFont(font);
    m_conditionEdit->setFont(font);
}


void CConditionProperties::ConditionChanged()
{
    if(m_conditionBubble)
        m_conditionBubble->setCondition(m_conditionEdit->toPlainText());
}

