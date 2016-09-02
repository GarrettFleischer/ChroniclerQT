#include "cremovescenecommand.h"

#include <QTabWidget>

#include "Properties/cprojectview.h"
#include "Misc/cscenemodel.h"

#include "cgraphicsview.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

CRemoveSceneCommand::CRemoveSceneCommand(CGraphicsView *view, int index)
    : m_view(view), m_index(index)
{
    setText("remove scene");
}

CRemoveSceneCommand::~CRemoveSceneCommand()
{
    m_view->deleteLater();
}

void CRemoveSceneCommand::undo()
{
    shared().projectView->model()->InsertItem(m_view, m_index);
}

void CRemoveSceneCommand::redo()
{
    shared().projectView->model()->ExtractItem(m_view);
    shared().sceneTabs->removeTab(shared().sceneTabs->indexOf(m_view));
}

int CRemoveSceneCommand::id() const
{
    return Chronicler::RemoveSceneCommand;
}
