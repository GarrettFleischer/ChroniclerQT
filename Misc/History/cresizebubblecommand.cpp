#include "cresizebubblecommand.h"

#include "Bubbles/cbubble.h"

#include "Misc/chronicler.h"

CResizeBubbleCommand::CResizeBubbleCommand(CBubble *bubble, const QRectF &oldSize, const QRectF &newSize)
    : m_bubble(bubble), m_oldSize(oldSize), m_newSize(newSize)
{
    setText("resize bubble");
}


void CResizeBubbleCommand::undo()
{
    m_bubble->setBounds(m_oldSize);
}

void CResizeBubbleCommand::redo()
{
    m_bubble->setBounds(m_newSize);
}

int CResizeBubbleCommand::id() const
{
    return Chronicler::ResizeBubbleCommand;
}

bool CResizeBubbleCommand::mergeWith(const QUndoCommand *other)
{
    const CResizeBubbleCommand *resizeCommand = dynamic_cast<const CResizeBubbleCommand *>(other);

    if(resizeCommand && resizeCommand->m_bubble == m_bubble)
    {
        m_newSize = resizeCommand->m_newSize;

        return true;
    }

    return false;
}
