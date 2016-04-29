#include "cactionbubble.h"

#include <QStringList>

CActionBubble::CActionBubble(QMenu *contextMenu, const QPointF &pos, const Chronicler::CPalette &palette, const QFont &font, QGraphicsItem *parent)
    : CBubble(contextMenu, pos, palette, font, parent)
{
    m_type = Chronicler::Action;

    m_palette.fill = QColor(151,118,166);

    AdjustMinSize();

    setPolygon(QPolygonF(QRectF(-m_minSize.width()/2, -m_minSize.height()/2, m_minSize.width()*2, m_minSize.height()*2)));
    UpdatePolygon();
}


void CActionBubble::UpdatePolygon()
{
//    qreal padding = 10;
//    QRectF tb = m_actions[0]->textBounds(m_minSize);
//    m_actions[0]->Resize(tb);

//    QRectF b(tb.x() - padding, tb.y() - padding,
//             tb.width() + padding*2, tb.height() + padding*2);

//    QPainterPath path;
//    path.addRoundedRect(b, 10, 10);

//    setPolygon(path.toFillPolygon());
}

void CActionBubble::AdjustMinSize()
{
    QFontMetrics fm(m_font);

    m_minSize.setWidth(100);
    m_minSize.setHeight(fm.height());
}


void CActionBubble::AddLink(CConnection *link, int position)
{
}

void CActionBubble::RemoveLink(CConnection *link)
{
}

QList<CConnection *> CActionBubble::links()
{
    return {};
}
