#include "cremovebubblescommand.h"

#include "cgraphicsscene.h"
#include "Bubbles/cbubble.h"

CRemoveBubblesCommand::CRemoveBubblesCommand(CGraphicsScene *scene, const QList<CBubble *> &bubbles)
    : m_scene(scene), m_bubbles(bubbles), m_did(true)
{
//    QString name;

//    switch ((m_type = m_bubbles->getType()))
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

//    setText("Remove " + name + "bubble");
}

CRemoveBubblesCommand::~CRemoveBubblesCommand()
{
    if(m_did)
        for(CBubble *bubble : m_bubbles)
            bubble->deleteLater();
}

void CRemoveBubblesCommand::undo()
{
    for(CBubble *bubble : m_bubbles)
        m_scene->AddBubble(bubble);

    m_did = false;
}

void CRemoveBubblesCommand::redo()
{
    for(CBubble *bubble : m_bubbles)
        m_scene->RemoveBubble(bubble);

    m_did = true;
}
