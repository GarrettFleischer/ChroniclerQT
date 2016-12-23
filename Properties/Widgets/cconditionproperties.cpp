#include "cconditionproperties.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QFontMetrics>

#include "Misc/Bubbles/cconditionedit.h"
#include "Bubbles/cconditionbubble.h"
#include "Misc/cshighlighter.h"

CConditionProperties::CConditionProperties(QWidget *parent)
    : CPropertiesWidget(parent), m_conditionBubble(0), m_conditionEdit(0)
{
    // Condition Widget
    QLabel *lblConditionStart = new QLabel(tr("*if ("), this);
    QLabel *lblConditionEnd = new QLabel(tr(")"), this);
    m_conditionEdit = new CConditionEdit(this);
    m_conditionEdit->setFixedHeight(m_conditionEdit->fontMetrics().height() * 2);
    connect(m_conditionEdit, SIGNAL(textChanged()), this, SLOT(ConditionChanged()));

    new CSHighlighter(m_conditionEdit->document());

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

void CConditionProperties::ConditionChanged()
{
    if(m_conditionBubble)
        m_conditionBubble->setCondition(m_conditionEdit->toPlainText());
}

