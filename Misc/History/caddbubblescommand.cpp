#include "caddbubblescommand.h"

#include "cgraphicsscene.h"
#include "Bubbles/cbubble.h"

#include "Misc/chronicler.h"


CAddBubblesCommand::CAddBubblesCommand(CGraphicsScene *scene, const QList<CBubble *> &bubbles)
    : m_scene(scene), m_bubbles(bubbles), m_undid(false)
{
    setText(QString("add bubble") + (bubbles.length() > 1 ? "s" : ""));
}

CAddBubblesCommand::~CAddBubblesCommand()
{
    if(m_undid)
        for(CBubble *bubble : m_bubbles)
            bubble->deleteLater();
}


void CAddBubblesCommand::undo()
{
    for(CBubble *bubble : m_bubbles)
        m_scene->RemoveBubble(bubble);

    m_undid = true;
}

void CAddBubblesCommand::redo()
{
    for(CBubble *bubble : m_bubbles)
        m_scene->AddBubble(bubble);

    m_undid = false;
}

int CAddBubblesCommand::id() const
{
    return Chronicler::AddBubblesCommand;
}
