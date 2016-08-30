#include "cmovebubblecommand.h"

#include "Bubbles/cbubble.h"


CMoveBubbleCommand::CMoveBubbleCommand(const QList<MoveData> &data)
    : m_moveData(data)
{
        setText(QString("move bubble") + (data.length() > 1 ? "s" : ""));
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

