#include "cconditionbubble.h"

CConditionBubble::CConditionBubble(QMenu *contextMenu, QGraphicsItem *parent)
    : CBubble(contextMenu, parent), m_label("")
{
    m_type = Condition;

    m_color = QColor(151,118,166);

    m_condition = new CTextItem("", QRectF(), this);
    m_condition->SetStyle(Qt::AlignHCenter);
    
    setCursor(Qt::PointingHandCursor);

    AdjustMinSize();

    setPolygon(QPolygonF(QRectF(-m_minSize.width()/2, -m_minSize.height()/2, m_minSize.width()*2, m_minSize.height()*2)));
    UpdateShape();
}


void CConditionBubble::UpdateShape()
{
    qreal padding = 10;
    QRectF tb = m_condition->textBounds(m_minSize);
    m_condition->Resize(tb);

    QRectF b(tb.x() - padding, tb.y() - padding,
             tb.width() + padding*2, tb.height() + padding*2);

    QPainterPath path;
    path.addRoundedRect(b, 10, 10);

    m_polygon = path.toFillPolygon();

    setPolygon(m_polygon);
}

void CConditionBubble::AdjustMinSize()
{
    QFontMetrics fm(m_font);

    m_minSize.setWidth(100);
    m_minSize.setHeight(fm.height());
}


void CConditionBubble::SetFont(const QFont &font)
{
    CBubble::SetFont(font);
    m_condition->SetFont(m_font);
    UpdateShape();
}

void CConditionBubble::SetFontColor(const QColor &color)
{
    CBubble::SetFontColor(color);
    m_condition->SetColor(m_fontColor);
}



void CConditionBubble::SetCondition(const QString &condition)
{
    m_conditionText = condition;
    QString txt = QString("*if ( ").append(condition).append(" )");
    m_condition->SetText(txt);
    UpdateShape();
}

