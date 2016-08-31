#ifndef CREMOVESCENECOMMAND_H
#define CREMOVESCENECOMMAND_H

#include <QUndoCommand>

class CGraphicsView;

class CRemoveSceneCommand : public QUndoCommand
{
public:
    CRemoveSceneCommand(CGraphicsView *view, int index);

    virtual void undo() Q_DECL_OVERRIDE;
    virtual void redo() Q_DECL_OVERRIDE;
    virtual int id() const Q_DECL_OVERRIDE;

private:
    CGraphicsView *m_view;
    int m_index;
};

#endif // CREMOVESCENECOMMAND_H
