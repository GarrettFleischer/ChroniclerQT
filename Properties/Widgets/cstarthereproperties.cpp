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

    QLineEdit *label = new QLineEdit(this);
    QFormLayout *fl_label = new QFormLayout();
    fl_label->addRow("Label", label);

    QHBoxLayout *hl_view = new QHBoxLayout();
    hl_view->addWidget(m_view);
    hl_view->addWidget(btns);

    QVBoxLayout *vl_main = new QVBoxLayout(this);
    vl_main->addLayout(fl_label);
    vl_main->addLayout(hl_view);

    setLayout(vl_main);
}

void CStartHereProperties::setBubble(CBubble *bbl)
{
    CStartHereBubble *startHereBubble = static_cast<CStartHereBubble *>(bbl);

    m_view->setModel(startHereBubble->model());
    m_view->setFont(startHereBubble->getFont());
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
