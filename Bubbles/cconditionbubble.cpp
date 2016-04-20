#include "cconditionbubble.h"

CConditionBubble::CConditionBubble(QMenu *contextMenu, const QPointF &pos, const CPalette &palette, const QFont &font, QGraphicsItem *parent)
    : CBubble(contextMenu, pos, palette, font, parent)
{
    m_type = Chronicler::Condition;

    m_palette.fill = QColor(151,118,166);

    m_condition = new CTextItem("", QRectF(), this);
    m_condition->SetStyle(Qt::AlignHCenter);
    
    setCursor(Qt::PointingHandCursor);

    AdjustMinSize();

    setPolygon(QPolygonF(QRectF(-m_minSize.width()/2, -m_minSize.height()/2, m_minSize.width()*2, m_minSize.height()*2)));
    UpdatePolygon();
}


void CConditionBubble::UpdatePolygon()
{
    qreal padding = 10;
    QRectF tb = m_condition->textBounds(m_minSize);
    m_condition->Resize(tb);

    QRectF b(tb.x() - padding, tb.y() - padding,
             tb.width() + padding*2, tb.height() + padding*2);

    QPainterPath path;
    path.addRoundedRect(b, 10, 10);

    setPolygon(path.toFillPolygon());
}

void CConditionBubble::AdjustMinSize()
{
    QFontMetrics fm(m_font);

    m_minSize.setWidth(100);
    m_minSize.setHeight(fm.height());
}

void CConditionBubble::setFont(const QFont &font)
{
    CBubble::setFont(font);
    m_condition->setFont(m_font);
    UpdatePolygon();
}

void CConditionBubble::setPalette(const Chronicler::CPalette &palette)
{
    m_condition->setColor(palette.font);
    CBubble::setPalette(palette);
}

void CConditionBubble::setCondition(QString condition)
{
    m_conditionText = condition;
    QString txt = QString("*if ( ").append(condition).append(" )");
    m_condition->setText(txt);
    UpdatePolygon();
}

