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

    // Default Palettes
    m_story.fill = QColor(124, 140, 230);
    m_choice.fill = QColor(104, 160, 210);
    m_action.fill = QColor(161,88,136);
    m_condition.fill = QColor(151,118,166);
    m_code.fill = QColor(124, 140, 230);
    m_start.fill = Qt::darkGreen;
    m_true.fill = Qt::darkGreen;
    m_false.fill = Qt::darkRed;

    shared().defaultStory = new CPaletteAction(this, m_story, tr("Story"), 1);
    shared().defaultChoice = new CPaletteAction(this, m_choice, tr("Choice"), 2);
    shared().defaultAction = new CPaletteAction(this, m_action, tr("Action"), 3);
    shared().defaultCondition = new CPaletteAction(this, m_condition, tr("Condition"), 4);
    shared().defaultStart = new CPaletteAction(this, m_start, tr("Start"), 5);
    shared().defaultCode = new CPaletteAction(this, m_code, tr("Code"), 6);
    shared().defaultTrue = new CPaletteAction(this, m_true, "", 7);
    shared().defaultFalse = new CPaletteAction(this, m_false, "", 8);

    m_creator = new CPaletteCreator(this);
    connect(m_creator, SIGNAL(accepted()), this, SLOT(Saved()));

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

    return Q_NULLPTR;
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
        m_editing->setText(m_creator->getName());
        m_editing->setPalette(m_creator->getPalette());
        m_current = m_editing;
        m_editing = Q_NULLPTR;
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

    Reset();

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

QDataStream &CPaletteButton::Serialize(QDataStream &ds) const
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
    // reset defaults
    shared().defaultStory->setPalette(m_story);
    shared().defaultChoice->setPalette(m_choice);
    shared().defaultAction->setPalette(m_action);
    shared().defaultCondition->setPalette(m_condition);
    shared().defaultCode->setPalette(m_code);
    shared().defaultStart->setPalette(m_start);

    // reset menu
    Clear();
    m_menu->addAction(shared().defaultStory);
    m_menu->addAction(shared().defaultChoice);
    m_menu->addAction(shared().defaultAction);
    m_menu->addAction(shared().defaultCondition);
    m_menu->addAction(shared().defaultCode);
    m_menu->addAction(shared().defaultStart);
}
