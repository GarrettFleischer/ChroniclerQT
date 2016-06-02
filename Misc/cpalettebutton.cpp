#include "cpalettebutton.h"

#include <QMenu>
#include <QContextMenuEvent>

#include "Misc/cpaletteaction.h"
#include "Properties/cpalettecreator.h"

#include <QDebug>

CPaletteButton::CPaletteButton(QWidget *parent)
    : QToolButton(parent)
{
    setPopupMode(QToolButton::MenuButtonPopup);
    setCheckable(true);
    setIcon(QIcon(":/images/icn_palette"));
    setToolTip("Palette creator/painter");

    m_creator = new CPaletteCreator(this);
    connect(m_creator, SIGNAL(accepted()), this, SLOT(Saved()));

    // testing...
    m_menu = new QMenu();
    m_menu->addAction(new CPaletteAction(this));
    setMenu(m_menu);
}


void CPaletteButton::contextMenuEvent(QContextMenuEvent *)
{
//    m_menu->actionAt(event->globalPos());
    m_creator->setPalette(CPalette());
    m_creator->show();
}

void CPaletteButton::Saved()
{
    m_menu->addAction(new CPaletteAction(this, m_creator->getPalette(), m_creator->getName()));
}
