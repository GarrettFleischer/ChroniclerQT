#include "cstarthereproperties.h"

#include <QTableView>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>

#include "Misc/clistbuttons.h"
#include "Bubbles/cstartherebubble.h"
#include "Misc/Bubbles/cstartheremodel.h"
#include "Misc/Bubbles/cstartheredelegate.h"
#include "Bubbles/cstartherebubble.h"
#include "Misc/Bubbles/ccodeedit.h"

CStartHereProperties::CStartHereProperties(QWidget *parent)
    : QWidget(parent)
{
    CListButtons *btns = new CListButtons(this, CListButtons::Add | CListButtons::Remove);
    connect(btns, SIGNAL(addItem()), this, SLOT(AddItem()));
    connect(btns, SIGNAL(removeItem()), this, SLOT(RemoveItem()));

    m_variablesView = new QTableView(this);
    m_variablesView->setItemDelegate(new CStartHereDelegate(this));
    m_variablesView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

    m_customCodeEdit = new CCodeEdit(this);
    connect(m_customCodeEdit, SIGNAL(textChanged()), this, SLOT(CodeChanged()));

    m_label = new QLineEdit(this);
    connect(m_label, SIGNAL(textChanged(QString)), this, SLOT(LabelChanged(QString)));
    QFormLayout *fl_label = new QFormLayout();
    fl_label->addRow("Label:", m_label);

    QHBoxLayout *hl_view = new QHBoxLayout();
    hl_view->addWidget(m_variablesView);
    hl_view->addWidget(btns);

    QVBoxLayout *vl_main = new QVBoxLayout(this);
    vl_main->addLayout(fl_label);
    vl_main->addLayout(hl_view);
    vl_main->addWidget(new QLabel("Custom Code", this));
    vl_main->addWidget(m_customCodeEdit);
    vl_main->addStretch(0.5);

    setLayout(vl_main);
}

void CStartHereProperties::setBubble(CBubble *bbl)
{
    m_bubble = dynamic_cast<CStartHereBubble *>(bbl);

    if(m_bubble)
    {
        m_label->setText(m_bubble->getLabel());
        m_customCodeEdit->setText(m_bubble->customCode());
        m_variablesView->setModel(m_bubble->model());
        m_variablesView->setFont(m_bubble->getFont());

        setEnabled(true);
    }
    else
        setEnabled(false);
}

void CStartHereProperties::AddItem()
{
    static_cast<CStartHereModel *>(m_variablesView->model())->AddItem(CVariable());
}

void CStartHereProperties::RemoveItem()
{
    if(m_variablesView->currentIndex().isValid())
        static_cast<CStartHereModel *>(m_variablesView->model())->RemoveItemAt(m_variablesView->currentIndex().row());
}

void CStartHereProperties::LabelChanged(QString label)
{
    m_bubble->setLabel(label);
}

void CStartHereProperties::CodeChanged()
{
    m_bubble->setCustomCode(m_customCodeEdit->toPlainText());
}
