#include "cpalettebutton.h"

#include <QMenu>
#include <QContextMenuEvent>

#include "Misc/cactionmenu.h"
#include "Misc/cpaletteaction.h"
#include "Properties/cpalettecreator.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

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
    m_current = shared().defaultStory;

    m_menu = new CActionMenu();
    m_menu->addAction(shared().defaultStory);
    m_menu->addAction(shared().defaultChoice);
    m_menu->addAction(shared().defaultAction);
    m_menu->addAction(shared().defaultCondition);
    m_menu->addAction(shared().defaultStart);
    connect(m_menu, SIGNAL(leftTriggered(QAction*)), this, SLOT(SelectAction(QAction*)));
    connect(m_menu, SIGNAL(rightTriggered(QAction*)), this, SLOT(EditAction(QAction*)));

    setMenu(m_menu);
}

CPaletteAction *CPaletteButton::getPalette()
{
    return m_current;
}

CPaletteAction *CPaletteButton::getPaletteWithUID(Chronicler::t_uid uid)
{
    for(QAction *action : m_menu->actions())
    {
        CPaletteAction *a = dynamic_cast<CPaletteAction *>(action);
        if(a->getUID() == uid)
            return a;
    }

    return 0;
}


void CPaletteButton::contextMenuEvent(QContextMenuEvent *)
{
    m_editing = 0;
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


QDataStream &CPaletteButton::Read(QDataStream &ds, const QString &)
{
    qint32 len;
    ds >> len;

    for(int i = 0; i < len; ++i)
    {
        t_uid uid;
        QString name;
        CPalette palette;

        ds >> uid >> name >> palette;

        CPaletteAction *pwuid = getPaletteWithUID(uid);
        if(pwuid)
            pwuid->setPalette(palette);
        else
            m_menu->addAction(new CPaletteAction(this, palette, name, uid));
    }

    return ds;
}

QDataStream &CPaletteButton::Write(QDataStream &ds)
{
    QList<QAction *> actions = m_menu->actions();
    ds << static_cast<qint32>(actions.length());

    for(QAction *action : actions)
    {
        CPaletteAction *a = dynamic_cast<CPaletteAction *>(action);
        CPalette p = a->getPalette();
        ds << a->getUID() << a->text() << p;
    }

    return ds;
}
