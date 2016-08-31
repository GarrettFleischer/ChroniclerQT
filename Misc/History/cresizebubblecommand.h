#ifndef CRESIZEBUBBLECOMMAND_H
#define CRESIZEBUBBLECOMMAND_H

#include <QUndoCommand>
#include <QRectF>

class CBubble;

class CResizeBubbleCommand : public QUndoCommand
{
public:
    CResizeBubbleCommand(CBubble *bubble, const QRectF &oldSize, const QRectF &newSize);

    virtual void undo() Q_DECL_OVERRIDE;
    virtual void redo() Q_DECL_OVERRIDE;
    virtual int id() const Q_DECL_OVERRIDE;
    virtual bool mergeWith(const QUndoCommand *other) Q_DECL_OVERRIDE;

private:
    CBubble *m_bubble;
    QRectF m_oldSize;
    QRectF m_newSize;
};

#endif // CRESIZEBUBBLECOMMAND_H
