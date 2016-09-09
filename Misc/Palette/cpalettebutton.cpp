#include "cpalettebutton.h"

#include <QMenu>
#include <QContextMenuEvent>
#include <QGuiApplication>

#include "Misc/qactionmenu.h"
#include "Misc/Palette/cpaletteaction.h"
#include "Properties/cpalettecreator.h"

#include "Properties/cprojectview.h"
#include "cgraphicsview.h"
#include "cgraphicsscene.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

CPaletteButton::CPaletteButton(QWidget *parent)
    : QToolButton(parent), m_editing(Q_NULLPTR)
{
    setPopupMode(QToolButton::MenuButtonPopup);
    setCheckable(true);
    setIcon(QIcon(":/images/icn_palette"));
    setToolTip(tr("Palette tool\nRight click on the tool to add new palette.\nRight click on an existing palette to edit it."));

    m_creator = new CPaletteCreator(this);
    connect(m_creator, SIGNAL(accepted()), this, SLOT(Saved()));

    // TODO add defaults for each bubble type
    m_current = shared().defaultStory;

    m_menu = new QActionMenu();
    Reset();
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

void CPaletteButton::setCurrent(CPaletteAction *palette)
{
    m_current = palette;

    QGuiApplication::restoreOverrideCursor();
    if(shared().cursorMode == Chronicler::Paint)
        QGuiApplication::setOverrideCursor(QCursor(m_current->icon().pixmap(32, 32)));
}

CPaletteAction *CPaletteButton::getCurrent()
{
    return m_current;
}


void CPaletteButton::contextMenuEvent(QContextMenuEvent *)
{
    m_editing = Q_NULLPTR;
    m_creator->setName(tr("New Palette"));
    m_creator->setPalette(CPalette());
    m_creator->show();
}

void CPaletteButton::SelectAction(QAction *action)
{
    setCurrent(dynamic_cast<CPaletteAction *>(action));
    shared().setMode(Chronicler::Paint);
}

void CPaletteButton::EditAction(QAction *action)
{
    m_editing = dynamic_cast<CPaletteAction *>(action);
    m_creator->setName(m_editing->text());
    m_creator->setPalette(m_editing->getPalette());
    m_creator->show();
}

void CPaletteButton::Saved()
{
    if(m_editing)
    {
        if(m_editing == shared().defaultAction || m_editing == shared().defaultChoice || m_editing == shared().defaultCode ||
           m_editing == shared().defaultCondition || m_editing == shared().defaultStart || m_editing == shared().defaultStory)
        {
            m_current = new CPaletteAction(this, m_creator->getPalette(), m_creator->getName());
            m_menu->insertAction(m_editing, m_current);
            m_menu->removeAction(m_editing);

            for(CGraphicsView *view : shared().projectView->getViews())
                for(CBubble *b : view->cScene()->bubbles())
                    if(b->getPalette() == m_editing)
                        b->setPalette(m_current);
        }
        else
        {
            m_editing->setText(m_creator->getName());
            m_editing->setPalette(m_creator->getPalette());
            m_current = m_editing;
            m_editing = Q_NULLPTR;
        }
    }
    else
    {
        m_current = new CPaletteAction(this, m_creator->getPalette(), m_creator->getName());
        m_menu->addAction(m_current);
    }

    shared().setMode(Chronicler::Paint);
}


QDataStream &CPaletteButton::Deserialize(QDataStream &ds, const Chronicler::CVersion &version)
{
    Q_UNUSED(version)

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
        {
            pwuid->setPalette(palette);
            pwuid->setText(name);
        }
        else
            m_menu->addAction(new CPaletteAction(this, palette, name, uid));
    }

    return ds;
}

QDataStream &CPaletteButton::Serialize(QDataStream &ds)
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

void CPaletteButton::Clear()
{
    m_menu->clear();
}

void CPaletteButton::Reset()
{
    Clear();

    m_menu->addAction(shared().defaultStory);
    m_menu->addAction(shared().defaultChoice);
    m_menu->addAction(shared().defaultAction);
    m_menu->addAction(shared().defaultCondition);
    m_menu->addAction(shared().defaultCode);
    m_menu->addAction(shared().defaultStart);
}
