#include "clistbuttons.h"

#include <QVBoxLayout>
#include <QPushButton>

CListButtons::CListButtons(QWidget *parent, int activeButtons)
    : QWidget(parent)
{
    QVBoxLayout *vl_main = new QVBoxLayout(this);

    if(activeButtons & Up)
    {
        QPushButton *btn_up = new QPushButton(QIcon(":/images/icn_up"), "");
        btn_up->setToolTip(tr("Move item up"));
        connect(btn_up, SIGNAL(clicked(bool)), this, SIGNAL(moveUp()));

        vl_main->addWidget(btn_up);
    }

    if(activeButtons & Down)
    {
        QPushButton *btn_down = new QPushButton(QIcon(":/images/icn_down"), "");
        btn_down->setToolTip(tr("Move item down"));
        connect(btn_down, SIGNAL(clicked(bool)), this, SIGNAL(moveDown()));

        vl_main->addWidget(btn_down);
    }

    if(activeButtons & Add)
    {
        QPushButton *btn_add = new QPushButton(QIcon(":/images/icn_add"), "");
        btn_add->setToolTip(tr("Add new item"));
        connect(btn_add, SIGNAL(clicked(bool)), this, SIGNAL(addItem()));

        vl_main->addWidget(btn_add);
    }

    if(activeButtons & Remove)
    {
        QPushButton *btn_remove = new QPushButton(QIcon(":/images/icn_trash"), "");
        btn_remove->setToolTip(tr("Delete item.\nWarning! this cannot be undone!"));
        connect(btn_remove, SIGNAL(clicked(bool)), this, SIGNAL(removeItem()));

        vl_main->addWidget(btn_remove);
    }

    vl_main->addStretch(1);
}
