#include "cconditionbubble.h"

#include "Misc/ctextitem.h"
#include "Connections/cconnection.h"


CConditionBubble::CConditionBubble(QMenu *contextMenu, const QPointF &pos, const CPalette &palette, const QFont &font, QGraphicsItem *parent)
    : CBubble(contextMenu, pos, palette, font, parent), m_trueLink(0), m_falseLink(0)
{
    m_type = Chronicler::Condition;

    m_palette.fill = QColor(151,118,166);

    m_condition = new CTextItem("", QRectF(), this);
    m_condition->SetStyle(Qt::AlignHCenter);
    
    setCursor(Qt::PointingHandCursor);

    AdjustMinSize();

    setPolygon(QPolygonF(QRectF(-m_minSize.width()/2, -m_minSize.height()/2, m_minSize.width(), m_minSize.height())));
    UpdatePolygon();
}

CConditionBubble::~CConditionBubble()
{
    delete m_trueLink;
    delete m_falseLink;
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

void CConditionBubble::RemoveLink(CConnection *link)
{
    if(link)
    {
        if(link->startAnchor() == Anchor::Left)
        {
            m_trueLink = 0;
        }
        else if(link->startAnchor() == Anchor::Right)
        {
            m_falseLink = 0;
        }
    }
}

void CConditionBubble::AddLink(CConnection *link)
{
    if(link)
    {
        if(link->startAnchor() == Anchor::Left)
        {
            if(m_trueLink != link)
                delete m_trueLink;

            m_trueLink = link;
        }
        else if(link->startAnchor() == Anchor::Right)
        {
            if(m_falseLink != link)
                delete m_falseLink;

            m_falseLink = link;
        }
    }
}

QList<CConnection *> CConditionBubble::links()
{
    return {m_trueLink, m_falseLink};
}

Anchor CConditionBubble::OutputAnchorAtPosition(const QPointF &pos)
{
    if(pos.x() < sceneBoundingRect().center().x())
        return Anchor::Left;

    return Anchor::Right;
}

Chronicler::Anchor CConditionBubble::InputAnchorAtPosition(const QPointF &)
{
    return Anchor::Up;
}
