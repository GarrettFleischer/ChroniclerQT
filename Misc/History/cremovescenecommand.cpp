#include "cremovescenecommand.h"

#include "Properties/cprojectview.h"
#include "Misc/cscenemodel.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

CRemoveSceneCommand::CRemoveSceneCommand(CGraphicsView *view, int index)
    : m_view(view), m_index(index)
{
    setText("remove scene");
}


void CRemoveSceneCommand::undo()
{
    shared().projectView->model()->InsertItem(m_view, m_index);
}

void CRemoveSceneCommand::redo()
{
    shared().projectView->model()->ExtractItem(m_view);
}

int CRemoveSceneCommand::id() const
{
    return Chronicler::RemoveSceneCommand;
}
