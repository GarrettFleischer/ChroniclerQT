#ifndef CMOVEBUBBLECOMMAND_H
#define CMOVEBUBBLECOMMAND_H

#include <QUndoCommand>
#include <QPointF>

class CBubble;

class CMoveBubbleCommand : public QUndoCommand
{
public:
    struct MoveData {
        CBubble *bubble;
        QPointF oldPos;
        QPointF newPos;

        MoveData(CBubble *b, const QPointF &o, const QPointF &n) : bubble(b), oldPos(o), newPos(n) {}
        bool operator ==(const MoveData &rhs) { return bubble == rhs.bubble; }
    };

    CMoveBubbleCommand(const QList<MoveData> &data);

    virtual void undo() Q_DECL_OVERRIDE;
    virtual void redo() Q_DECL_OVERRIDE;

private:
    QList<MoveData> m_moveData;

};

#endif // CMOVEBUBBLECOMMAND_H
