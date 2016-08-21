#include "caddbubblescommand.h"

#include "cgraphicsscene.h"
#include "Bubbles/cbubble.h"


CAddBubblesCommand::CAddBubblesCommand(CGraphicsScene *scene, const QList<CBubble *> &bubbles)
    : m_scene(scene), m_bubbles(bubbles), m_undid(false)
{
//    QString name;

//    switch ((m_type = m_bubble->getType()))
//    {
//    case Chronicler::Story:
//        name = "story";
//        break;
//    case Chronicler::Action:
//        name = "action";
//        break;
//    case Chronicler::Choice:
//        name = "choice";
//        break;
//    case Chronicler::Condition:
//        name = "condition";
//        break;
//    default:
//        break;
//    }

//    setText("Add " + name + "bubble");
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
