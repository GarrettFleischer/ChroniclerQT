#include "Properties/cconditionproperties.h"

#include "Bubbles/cconditionbubble.h"

CConditionProperties::CConditionProperties(CBubble *bbl, QStringListModel *model, QWidget *parent)
    : QWidget(parent), m_model(model), m_labelEdit(0), m_conditionEdit(0), m_lockEdit(0), m_orderEdit(0)
{
    // WIDGETS
        // Title
        QLabel *lblLabel = new QLabel(tr("Label:"), this);
        m_labelEdit = new QLineEdit(this);
        connect(m_labelEdit, SIGNAL(textChanged(QString)), this, SLOT(LabelChanged(QString)));

        // Story
        QLabel *lblConditionStart = new QLabel(tr("*if ("), this);
        QLabel *lblConditionEnd = new QLabel(tr(")"), this);
        m_conditionEdit = new CLineEdit(this, m_model);
        connect(m_conditionEdit, SIGNAL(textChanged()), this, SLOT(ConditionChanged()));

        // Lock
        QLabel *lblLocked = new QLabel(tr("Locked:"), this);
        m_lockEdit = new QCheckBox(this);
        m_lockEdit->setFocusPolicy(Qt::NoFocus);
        connect(m_lockEdit, SIGNAL(toggled(bool)), this, SLOT(LockedChanged(bool)));

        // Order
        QLabel *lblOrder = new QLabel(tr("Order:"), this);
        m_orderEdit = new QLineEdit(this);
        m_orderEdit->setValidator(new QIntValidator(0,9999999));
        m_orderEdit->setMaximumWidth(50);
        m_orderEdit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        connect(m_orderEdit, SIGNAL(textChanged(QString)), this, SLOT(OrderChanged(QString)));
        
        
    // LABELS
        // Title
        QHBoxLayout *l1 = new QHBoxLayout();
        l1->addWidget(lblLabel);
        l1->addWidget(m_labelEdit);

        // Story
        QHBoxLayout *l2 = new QHBoxLayout();
        l2->addWidget(lblConditionStart);
        l2->addWidget(m_conditionEdit);
        l2->addWidget(lblConditionEnd);

        // Order
        QHBoxLayout *l3 = new QHBoxLayout();
        l3->addWidget(lblOrder);
        l3->addWidget(m_orderEdit);

        // Lock
        QHBoxLayout *l4 = new QHBoxLayout();
        l4->addWidget(lblLocked);
        l4->addWidget(m_lockEdit);
        l4->setAlignment(lblLocked, Qt::AlignRight);


//    // LAYOUT
    QVBoxLayout *lOrderLock = new QVBoxLayout();
    lOrderLock->addLayout(l3);
    lOrderLock->addLayout(l4);

    QHBoxLayout *lTitleOrder = new QHBoxLayout();
    lTitleOrder->addLayout(l1);
    lTitleOrder->addLayout(lOrderLock);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addLayout(lTitleOrder);
    layout->addLayout(l2);
    layout->addStretch(1);

    setLayout(layout);


    SetBubble(bbl);
}


void CConditionProperties::LabelChanged(QString title)
{
    if(m_bbl)
        m_bbl->m_label = title;
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


void CConditionProperties::SetBubble(CBubble *bbl)
{
    m_bbl = qgraphicsitem_cast<CConditionBubble *>(bbl);

    if(m_bbl)
    {
        setEnabled(true);
        
        m_labelEdit->setText(m_bbl->m_label);
        m_labelEdit->setFont(m_bbl->GetFont());
        //m_labelEdit->setFocus();
        
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


