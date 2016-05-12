#include "ctextitem.h"

#include <QPen>
#include <QFontMetrics>

CTextItem::CTextItem(const QString &text, const QRectF &bounds, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_text(text), m_bounds(bounds), m_textBounds(bounds), m_color(Qt::black), m_style(Qt::AlignLeft)
{
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemIsFocusable, false);
}


void CTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(QPen(m_color));
    painter->setFont(m_font);
    painter->drawText(boundingRect(), m_style, m_text, &m_textBounds);
}


QRectF CTextItem::textBounds(const QSizeF &minimum) const
{
    QFontMetrics fm(m_font);
    QSizeF size(minimum);
    QStringList lines = m_text.split("\n", QString::KeepEmptyParts);

    qreal height = fm.height() * lines.length() * 1.46;
    for(QString text : lines)
    {
        if(text.isEmpty())
            text = "W";

        qreal width = fm.width(text);// * 1.3;
        if(width > size.width())
            size.setWidth(width);
    }
    if(height > size.height())
        size.setHeight(height);

    return QRectF(boundingRect().topLeft(), size);
}


void CTextItem::setColor(const QColor &color)
{
    m_color = color;
    update();
}
