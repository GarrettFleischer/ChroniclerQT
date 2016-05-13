#ifndef CCHOICE_H
#define CCHOICE_H

#include "csinglelinkbubble.h"

class CTextItem;

class CChoice : public CSingleLinkBubble
{
    Q_OBJECT

public:
    explicit CChoice(const CPalette &palette, const QFont &font = QFont(), QGraphicsItem *parent = 0, const QString &choice = "");
    
    virtual void setPalette(const Chronicler::CPalette &palette) override;
    virtual void setFont(const QFont &font) override;

    virtual Chronicler::Anchor OutputAnchorAtPosition(const QPointF &pos) override;
    virtual Chronicler::Anchor InputAnchorAtPosition(const QPointF &) override;

    void setChoice(const QString &choice);
    QString choice() const;

    void setWidth(const qreal &width);

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    virtual void UpdatePolygon() override;

private:
    void AdjustMinSize();

    CTextItem *m_choice;

};

#endif // CCHOICE_H
