#ifndef CCHOICEBUBBLE_H
#define CCHOICEBUBBLE_H


#include "cbubble.h"
#include "Misc/ctextitem.h"

class CChoiceBubble : public CBubble
{
    Q_OBJECT

public:
    friend class CConditionProperties;

public:
    CChoiceBubble(QMenu *contextMenu, const QPointF &pos, const QFont &font = QFont(), const QColor &fontColor = Qt::black,
                  const QColor &lineColor = Qt::black, QGraphicsItem *parent = 0);

public:
    virtual void SetFont(const QFont &font);
    virtual void SetFontColor(const QColor &color);

    void AddChoice(const QString &choice);
    void MoveChoice(int old_index, int new_index);

protected:
    virtual void UpdateShape();
    void AdjustMinSize();

private:
    QString m_label;
    QList<CTextItem *> m_choices;
};

#endif // CCHOICEBUBBLE_H
