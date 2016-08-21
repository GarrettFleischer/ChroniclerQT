#include "cmovebubblecommand.h"

#include "Bubbles/cbubble.h"

CMoveBubbleCommand::CMoveBubbleCommand(CBubble *bubble, const QPointF &oldPos, const QPointF &newPos)
{
    m_moveData.append(MoveData(bubble, oldPos, newPos));
    setText("Move bubbles");
}

void CMoveBubbleCommand::undo()
{
    QUndoCommand::undo();
    for(int i = 0; i < m_moveData.length(); ++i)
        m_moveData[i].bubble->setPos(m_moveData[i].oldPos);
}

void CMoveBubbleCommand::redo()
{
    QUndoCommand::redo();
    for(int i = 0; i < m_moveData.length(); ++i)
        m_moveData[i].bubble->setPos(m_moveData[i].newPos);
}


bool CMoveBubbleCommand::mergeWith(const QUndoCommand *other)
{
    const CMoveBubbleCommand *moveCommand = dynamic_cast<const CMoveBubbleCommand *>(other);

    if(moveCommand)
    {
        for(const MoveData &md : moveCommand->m_moveData)
        {
            if(!m_moveData.contains(md))
                m_moveData.append(md);
            else
            {
                m_moveData.removeAll(md);
                m_moveData.append(md);
            }
        }

        return true;
    }

    return false;
}
