#ifndef CADDBUBBLECOMMAND_H
#define CADDBUBBLECOMMAND_H

#include <QUndoCommand>

#include "Misc/chronicler.h"
using Chronicler::BubbleType;

class CGraphicsScene;
class CBubble;

class CAddBubblesCommand : public QUndoCommand
{
public:
    CAddBubblesCommand(CGraphicsScene *scene, const QList<CBubble *> &bubbles);
    ~CAddBubblesCommand();

    virtual void undo() Q_DECL_OVERRIDE;
    virtual void redo() Q_DECL_OVERRIDE;

private:
    CGraphicsScene *m_scene;
    QList<CBubble *> m_bubbles;
    BubbleType m_type;

    bool m_undid;

};

#endif // CADDBUBBLECOMMAND_H
