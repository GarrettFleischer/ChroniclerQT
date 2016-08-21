#ifndef CMOVEBUBBLECOMMAND_H
#define CMOVEBUBBLECOMMAND_H

#include <QUndoCommand>
#include <QPointF>

class CBubble;

class CMoveBubbleCommand : public QUndoCommand
{
public:
    CMoveBubbleCommand(CBubble *bubble, const QPointF &oldPos, const QPointF &newPos);

    virtual void undo() Q_DECL_OVERRIDE;
    virtual void redo() Q_DECL_OVERRIDE;
    virtual bool mergeWith(const QUndoCommand *other) Q_DECL_OVERRIDE;

private:
    struct MoveData {
        CBubble *bubble;
        QPointF oldPos;
        QPointF newPos;

        MoveData(CBubble *b, const QPointF &o, const QPointF &n) : bubble(b), oldPos(o), newPos(n) {}
        bool operator ==(const MoveData &rhs) { return bubble == rhs.bubble; }
    };

    QList<MoveData> m_moveData;

};

#endif // CMOVEBUBBLECOMMAND_H
