#include "ccodeproperties.h"

#include "Bubbles/ccodebubble.h"
#include "Misc/Bubbles/ccodeedit.h"
#include "Misc/cshighlighter.h"

CCodeProperties::CCodeProperties(QWidget *parent)
    : CPropertiesWidget(parent)
{
    // Code Widget
    m_codeEdit = new CCodeEdit(this);
    connect(m_codeEdit, SIGNAL(textChanged()), this, SLOT(CodeChanged()));

    new CSHighlighter(m_codeEdit->document());

    m_layout->addWidget(m_codeEdit);

    setBubble(Q_NULLPTR);
}


void CCodeProperties::setBubble(CBubble *bubble)
{
    CPropertiesWidget::setBubble(bubble);

    m_codeBubble = dynamic_cast<CCodeBubble *>(bubble);
    if(m_codeBubble)
    {
        m_codeEdit->setText(m_codeBubble->getCode());
        m_codeEdit->setFont(m_codeBubble->getFont());
    }
    else
        m_codeEdit->setText("");
}

void CCodeProperties::CodeChanged()
{
    if(m_codeBubble)
        m_codeBubble->setCode(m_codeEdit->toPlainText());
}
