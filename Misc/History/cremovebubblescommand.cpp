#include "cremovebubblescommand.h"

#include "cgraphicsscene.h"
#include "Bubbles/cbubble.h"
#include "Connections/cconnection.h"


CRemoveBubblesCommand::CRemoveBubblesCommand(CGraphicsScene *scene, const QList<CBubble *> &bubbles)
    : m_scene(scene), m_bubbles(bubbles), m_did(true)
{
    setText(QString("remove bubble") + (m_bubbles.length() > 1 ? "s" : ""));

    for(CBubble *bbl : m_bubbles)
    {
        m_connections.append(bbl->connections());
        m_connections.append(bbl->links());
    }
}

CRemoveBubblesCommand::~CRemoveBubblesCommand()
{
    if(m_did)
    {
        for(CBubble *bubble : m_bubbles)
            bubble->deleteLater();

        for(CConnection *c : m_connections)
            c->deleteLater();
    }
}

void CRemoveBubblesCommand::undo()
{
    for(CBubble *bubble : m_bubbles)
        m_scene->AddBubble(bubble);

    for(CConnection *c : m_connections)
        m_scene->AddConnection(c);

    m_did = false;
}

void CRemoveBubblesCommand::redo()
{
    for(CBubble *bubble : m_bubbles)
        m_scene->RemoveBubble(bubble);

    for(CConnection *c : m_connections)
        m_scene->RemoveConnection(c);

    m_did = true;
}
