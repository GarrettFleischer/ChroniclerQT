#include "cstarthereproperties.h"

#include <QTableView>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

#include "Misc/clistbuttons.h"
#include "Bubbles/cstartherebubble.h"
#include "Misc/Bubbles/cstartheremodel.h"
#include "Misc/Bubbles/cstartheredelegate.h"
#include "Bubbles/cstartherebubble.h"

CStartHereProperties::CStartHereProperties(QWidget *parent)
    : QWidget(parent)
{
    CListButtons *btns = new CListButtons(this, CListButtons::Add | CListButtons::Remove);
    connect(btns, SIGNAL(addItem()), this, SLOT(AddItem()));
    connect(btns, SIGNAL(removeItem()), this, SLOT(RemoveItem()));

    m_view = new QTableView(this);
    m_view->setItemDelegate(new CStartHereDelegate(this));
    m_view->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

    m_label = new QLineEdit(this);
    connect(m_label, SIGNAL(textChanged(QString)), this, SLOT(LabelChanged(QString)));
    QFormLayout *fl_label = new QFormLayout();
    fl_label->addRow("Label:", m_label);

    QHBoxLayout *hl_view = new QHBoxLayout();
    hl_view->addWidget(m_view);
    hl_view->addWidget(btns);

    QVBoxLayout *vl_main = new QVBoxLayout(this);
    vl_main->addLayout(fl_label);
    vl_main->addLayout(hl_view);
    vl_main->addStretch(0.5);

    setLayout(vl_main);
}

void CStartHereProperties::setBubble(CBubble *bbl)
{
    m_bubble = dynamic_cast<CStartHereBubble *>(bbl);

    if(m_bubble)
    {
        m_label->setText(m_bubble->getLabel());
        m_view->setModel(m_bubble->model());
        m_view->setFont(m_bubble->getFont());

        setEnabled(true);
    }
    else
    {
        m_view->setModel(Q_NULLPTR);

        setEnabled(false);
    }
}

void CStartHereProperties::AddItem()
{
    static_cast<CStartHereModel *>(m_view->model())->AddItem(CVariable());
}

void CStartHereProperties::RemoveItem()
{
    if(m_view->currentIndex().isValid())
        static_cast<CStartHereModel *>(m_view->model())->RemoveItemAt(m_view->currentIndex().row());
}

void CStartHereProperties::LabelChanged(QString label)
{
    m_bubble->setLabel(label);
}
