#include "cstoryproperties.h"

#include <QLabel>

#include "Misc/ctextedit.h"
#include "Bubbles/cstorybubble.h"



CStoryProperties::CStoryProperties(QStringListModel *model, QWidget *parent)
    : CPropertiesWidget(model, parent), m_storyEdit(0)
{

    // Story Widget
    QLabel *lblStory = new QLabel(tr("Story:"), this);
    m_storyEdit = new CTextEdit(this, m_model);
    connect(m_storyEdit, SIGNAL(textChanged()), this, SLOT(StoryChanged()));

    // Story Layout
    QVBoxLayout *lStory = new QVBoxLayout();
    lStory->addWidget(lblStory);
    lStory->addWidget(m_storyEdit);

    m_layout->addLayout(lStory);

    setBubble(0);
}


void CStoryProperties::setBubble(CBubble *bbl)
{
    m_bbl = qgraphicsitem_cast<CStoryBubble *>(bbl);

    if(m_bbl)
    {
        setEnabled(true);

        m_labelEdit->setText(m_bbl->m_title->Text());
        m_labelEdit->setFont(bbl->GetFont());

        m_storyEdit->setText(m_bbl->m_story->Text());
        m_storyEdit->setFont(bbl->GetFont());

        m_lockEdit->setChecked(m_bbl->m_locked);

        m_orderEdit->setText(QString().number(m_bbl->m_order));
        m_orderEdit->setEnabled(m_lockEdit->isChecked());
    }
    else
    {
        m_labelEdit->setText(tr(""));
        m_storyEdit->setText(tr(""));
        m_lockEdit->setChecked(false);
        m_orderEdit->setText(tr(""));

        setEnabled(false);
    }
}


void CStoryProperties::setFont(const QFont &font)
{
    CPropertiesWidget::setFont(font);
    m_storyEdit->setFont(font);
}


void CStoryProperties::LabelChanged(QString label)
{
    if(m_bbl)
        m_bbl->m_title->SetText(label);
}

void CStoryProperties::StoryChanged()
{
    if(m_bbl)
        m_bbl->m_story->SetText(m_storyEdit->toPlainText());
}

void CStoryProperties::OrderChanged(QString order)
{
    if(m_bbl)
        m_bbl->m_order = order.toInt();
}

void CStoryProperties::LockedChanged(bool locked)
{
    if(m_bbl)
    {
        m_bbl->m_locked = locked;
        m_orderEdit->setEnabled(locked);
    }
}
