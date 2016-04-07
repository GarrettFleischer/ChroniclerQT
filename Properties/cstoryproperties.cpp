#include "cstoryproperties.h"

#include "Bubbles/cstorybubble.h"

CStoryProperties::CStoryProperties(CBubble *bbl, QStringListModel *model, QWidget *parent)
    : QWidget(parent), m_model(model), m_titleEdit(0), m_storyEdit(0), m_lockEdit(0), m_orderEdit(0)
{
    // WIDGETS
        // Title
        QLabel *lblTitle = new QLabel(tr("Title:"), this);
        m_titleEdit = new QLineEdit(this);
        //m_title->setMaximumWidth(400);
        connect(m_titleEdit, SIGNAL(textChanged(QString)), this, SLOT(TitleChanged(QString)));

        // Story
        QLabel *lblStory = new QLabel(tr("Story:"), this);
        m_storyEdit = new CTextEdit(this, m_model);
        connect(m_storyEdit, SIGNAL(textChanged()), this, SLOT(StoryChanged()));

        // Lock
        QLabel *lblLocked = new QLabel(tr("Locked:"), this);
        m_lockEdit = new QCheckBox(this);
        m_lockEdit->setFocusPolicy(Qt::NoFocus);
        connect(m_lockEdit, SIGNAL(toggled(bool)), this, SLOT(LockedChanged(bool)));

        // Order
        QLabel *lblOrder = new QLabel(tr("Order:"), this);
        m_orderEdit = new QLineEdit(this);
        m_orderEdit->setValidator(new QIntValidator(0, 9999999));
        m_orderEdit->setMaximumWidth(50);
        m_orderEdit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        connect(m_orderEdit, SIGNAL(textChanged(QString)), this, SLOT(OrderChanged(QString)));


    // LABELS
        // Title
        QHBoxLayout *l1 = new QHBoxLayout();
        l1->addWidget(lblTitle);
        l1->addWidget(m_titleEdit);
        //l1->addStretch(0.9);

        // Story
        QVBoxLayout *l2 = new QVBoxLayout();
        l2->addWidget(lblStory);
        l2->addWidget(m_storyEdit);

        // Order
        QHBoxLayout *l3 = new QHBoxLayout();
        l3->addWidget(lblOrder);
        l3->addWidget(m_orderEdit);

        // Lock
        QHBoxLayout *l4 = new QHBoxLayout();
        l4->addWidget(lblLocked);
        l4->addWidget(m_lockEdit);
        l4->setAlignment(lblLocked, Qt::AlignRight);


    // LAYOUT
    QVBoxLayout *lOrderLock = new QVBoxLayout();
    lOrderLock->addLayout(l3);
    lOrderLock->addLayout(l4);


    QHBoxLayout *lTitleOrder = new QHBoxLayout();
    lTitleOrder->addLayout(l1);
    lTitleOrder->addLayout(lOrderLock);

    QWidget *TitleOrderRestrictor = new QWidget();
    TitleOrderRestrictor->setLayout(lTitleOrder);
    TitleOrderRestrictor->setMaximumWidth(600);


    QVBoxLayout *lTitleStory = new QVBoxLayout();
    lTitleStory->addWidget(TitleOrderRestrictor);
    lTitleStory->addLayout(l2);

    QWidget *TitleStoryRestrictor = new QWidget();
    TitleStoryRestrictor->setLayout(lTitleStory);
    TitleStoryRestrictor->setMaximumSize(QSize(1280, 720));


    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(TitleStoryRestrictor);

    setLayout(layout);


    SetBubble(bbl);
}


void CStoryProperties::TitleChanged(QString title)
{
    if(m_bbl)
        m_bbl->m_title->SetText(title);
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


void CStoryProperties::SetBubble(CBubble *bbl)
{
    m_bbl = qgraphicsitem_cast<CStoryBubble *>(bbl);

    if(m_bbl)
    {
        setEnabled(true);

        m_titleEdit->setText(m_bbl->m_title->Text());
        m_titleEdit->setFont(bbl->GetFont());
        m_titleEdit->setFocus();

        m_storyEdit->setText(m_bbl->m_story->Text());
        m_storyEdit->setFont(bbl->GetFont());

        m_lockEdit->setChecked(m_bbl->m_locked);

        m_orderEdit->setText(QString().number(m_bbl->m_order));
        m_orderEdit->setEnabled(m_lockEdit->isChecked());
    }
    else
    {
        m_titleEdit->setText(tr(""));
        m_storyEdit->setText(tr(""));
        m_lockEdit->setChecked(false);
        m_orderEdit->setText(tr(""));

        setEnabled(false);
    }
}


void CStoryProperties::setFont(const QFont &font)
{
    QWidget::setFont(font);
    m_titleEdit->setFont(font);
    m_storyEdit->setFont(font);
    m_lockEdit->setFont(font);
    m_orderEdit->setFont(font);
}


