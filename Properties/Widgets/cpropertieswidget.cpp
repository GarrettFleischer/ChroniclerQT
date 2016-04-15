#include "cpropertieswidget.h"

#include <QLabel>
#include <QLineEdit>
#include <QIntValidator>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStringListModel>

#include "Misc/clineedit.h"


CPropertiesWidget::CPropertiesWidget(QStringListModel *model, QWidget *parent)
    : QWidget(parent), m_model(model), m_labelEdit(0), m_lockEdit(0), m_orderEdit(0)
{
    // WIDGETS
        // Title
        QLabel *lblTitle = new QLabel(tr("Label:"), this);
        m_labelEdit = new QLineEdit(this);
        connect(m_labelEdit, SIGNAL(textChanged(QString)), this, SLOT(LabelChanged(QString)));

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
        QHBoxLayout *lTitle = new QHBoxLayout();
        lTitle->addWidget(lblTitle);
        lTitle->addWidget(m_labelEdit);

        // Order
        QHBoxLayout *lOrder = new QHBoxLayout();
        lOrder->addWidget(lblOrder);
        lOrder->addWidget(m_orderEdit);

        // Lock
        QHBoxLayout *lLock = new QHBoxLayout();
        lLock->addWidget(lblLocked);
        lLock->addWidget(m_lockEdit);
        lLock->setAlignment(lblLocked, Qt::AlignRight);


//    // LAYOUT
    QVBoxLayout *lOrderLock = new QVBoxLayout();
    lOrderLock->addLayout(lOrder);
    lOrderLock->addLayout(lLock);

    QHBoxLayout *lTitleOrder = new QHBoxLayout();
    lTitleOrder->addLayout(lTitle);
    lTitleOrder->addLayout(lOrderLock);

    QWidget *titleOrderRestrictor = new QWidget();
    titleOrderRestrictor->setLayout(lTitleOrder);
    titleOrderRestrictor->setMaximumWidth(600);

    m_layout = new QVBoxLayout();
    m_layout->addWidget(titleOrderRestrictor);

    setLayout(m_layout);
}

void CPropertiesWidget::setFont(const QFont &font)
{
    QWidget::setFont(font);
    m_labelEdit->setFont(font);
    m_lockEdit->setFont(font);
    m_orderEdit->setFont(font);
}
