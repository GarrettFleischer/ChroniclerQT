#include "cpalettebutton.h"

#include <QMenu>
#include <QContextMenuEvent>

#include "Misc/cactionmenu.h"
#include "Misc/cpaletteaction.h"
#include "Properties/cpalettecreator.h"

#include <QDebug>

CPaletteButton::CPaletteButton(QWidget *parent)
    : QToolButton(parent), m_editing(0)
{
    setPopupMode(QToolButton::MenuButtonPopup);
    setCheckable(true);
    setIcon(QIcon(":/images/icn_palette"));
    setToolTip("Palette creator/painter");

    m_creator = new CPaletteCreator(this);
    connect(m_creator, SIGNAL(accepted()), this, SLOT(Saved()));

    // TODO add defaults for each bubble type
    m_current = new CPaletteAction(this);

    m_menu = new CActionMenu();
    m_menu->addAction(m_current);
    connect(m_menu, SIGNAL(leftTriggered(QAction*)), this, SLOT(SelectAction(QAction*)));
    connect(m_menu, SIGNAL(rightTriggered(QAction*)), this, SLOT(EditAction(QAction*)));

    setMenu(m_menu);
}

const Chronicler::CPalette &CPaletteButton::getPalette() const
{
    return m_current->getPalette();
}


void CPaletteButton::contextMenuEvent(QContextMenuEvent *)
{
    m_creator->setName("New Palette");
    m_creator->setPalette(CPalette());
    m_creator->show();
}

void CPaletteButton::SelectAction(QAction *action)
{
    m_current = dynamic_cast<CPaletteAction *>(action);
}

void CPaletteButton::EditAction(QAction *action)
{
    CPaletteAction *a = dynamic_cast<CPaletteAction *>(action);
    m_editing = a;
    m_creator->setName(a->text());
    m_creator->setPalette(a->getPalette());
    m_creator->show();
}

void CPaletteButton::Saved()
{
    if(m_editing)
    {
        m_editing->setText(m_creator->getName());
        m_editing->setPalette(m_creator->getPalette());
    }
    else
        m_menu->addAction(new CPaletteAction(this, m_creator->getPalette(), m_creator->getName()));
}
