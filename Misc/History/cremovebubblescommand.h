#ifndef CREMOVEBUBBLESCOMMAND_H
#define CREMOVEBUBBLESCOMMAND_H

#include <QUndoCommand>

#include "Misc/chronicler.h"
using Chronicler::BubbleType;

class CGraphicsScene;
class CBubble;

class CRemoveBubblesCommand : public QUndoCommand
{
public:
    CRemoveBubblesCommand(CGraphicsScene *scene, const QList<CBubble *> &bubbles);
    ~CRemoveBubblesCommand();

    virtual void undo() Q_DECL_OVERRIDE;
    virtual void redo() Q_DECL_OVERRIDE;

private:
    CGraphicsScene *m_scene;
    QList<CBubble *> m_bubbles;
    BubbleType m_type;

    bool m_did;
};

#endif // CREMOVEBUBBLESCOMMAND_H
