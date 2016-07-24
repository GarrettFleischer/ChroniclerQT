#include "cconditionbubble.h"

#include "Misc/Palette/cpaletteaction.h"

#include "Misc/ctextitem.h"
#include "Connections/cconnection.h"
#include "cgraphicsscene.h"


CConditionBubble::CConditionBubble(const QPointF &pos, CPaletteAction *palette, const QFont &font, QGraphicsItem *parent)
    : CBubble(pos, palette, font, parent), m_trueLink(0), m_falseLink(0)
{
    m_type = Chronicler::Condition;

    m_condition = new CTextItem("", QRectF(), this);
    m_condition->SetStyle(Qt::AlignCenter);
    
    setCursor(Qt::PointingHandCursor);

    AdjustMinSize();
    m_bounds = QRectF(-m_minSize.width()/2, -m_minSize.height()/2, m_minSize.width(), m_minSize.height());
    UpdatePolygon();

    setPalette(m_palette);
}

CConditionBubble::~CConditionBubble()
{
    dynamic_cast<CGraphicsScene *>(scene())->RemoveConnection(m_trueLink);
    dynamic_cast<CGraphicsScene *>(scene())->RemoveConnection(m_falseLink);
    delete m_trueLink;
    delete m_falseLink;
}


void CConditionBubble::UpdatePolygon()
{
    CBubble::UpdatePolygon();
    m_condition->Resize(boundingRect());
}

void CConditionBubble::AdjustMinSize()
{
    QFontMetrics fm(m_font);
    m_minSize.setHeight(fm.height());
}

void CConditionBubble::setFont(const QFont &font)
{
    CBubble::setFont(font);
    m_condition->setFont(m_font);
    AdjustMinSize();
    UpdatePolygon();
}

void CConditionBubble::setPalette(CPaletteAction *palette)
{
    m_condition->setColor(palette->getPalette().font);
    CBubble::setPalette(palette);
}

void CConditionBubble::setCondition(QString condition)
{
    m_conditionText = condition;
    QString txt = QString("*if ( ").append(condition).append(" )");
    m_condition->setText(txt);
    AdjustMinSize();
    UpdatePolygon();
}

void CConditionBubble::RemoveLink(CConnection *link)
{
    if(link)
    {
        if(link == m_trueLink)
            m_trueLink = 0;
        else if(link == m_falseLink)
            m_falseLink = 0;
    }
}

void CConditionBubble::RemoveLink(Chronicler::Anchor anchor)
{
    if(m_trueLink && m_trueLink->startAnchor() == anchor)
    {
        dynamic_cast<CGraphicsScene *>(scene())->RemoveConnection(m_trueLink);
        delete m_trueLink;
        m_trueLink = 0;
    }
    else if(m_falseLink && m_falseLink->startAnchor() == anchor)
    {
        dynamic_cast<CGraphicsScene *>(scene())->RemoveConnection(m_falseLink);
        delete m_falseLink;
        m_falseLink = 0;
    }
}

void CConditionBubble::AddLink(CConnection *link)
{
    if(link)
    {
        if(link->startAnchor() == Anchor::Left)
        {
            if(m_trueLink != link)
            {
                dynamic_cast<CGraphicsScene *>(scene())->RemoveConnection(m_trueLink);
                delete m_trueLink;
            }

            m_trueLink = link;
        }
        else if(link->startAnchor() == Anchor::Right)
        {
            if(m_falseLink != link)
            {
                dynamic_cast<CGraphicsScene *>(scene())->RemoveConnection(m_falseLink);
                delete m_falseLink;
            }

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


QDataStream &CConditionBubble::Deserialize(QDataStream &ds, const Chronicler::CVersion &version)
{
    CBubble::Deserialize(ds, version);

    bool trueLink, falseLink;

    ds >> m_conditionText >> trueLink >> falseLink;

    if(trueLink)
    {
        m_trueLink = dynamic_cast<CGraphicsScene *>(scene())->AddConnection();
        ds >> *m_trueLink;
    }
    if(falseLink)
    {
        m_falseLink = dynamic_cast<CGraphicsScene *>(scene())->AddConnection();
        ds >> *m_falseLink;
    }

    setCondition(m_conditionText);

    setPalette(m_palette);

    return ds;
}

QDataStream & CConditionBubble::Serialize(QDataStream &ds)
{
    CBubble::Serialize(ds);

    ds << m_conditionText << bool(m_trueLink) << bool(m_falseLink);

    if(m_trueLink)
         ds << *m_trueLink;
    if(m_falseLink)
        ds << *m_falseLink;

    return ds;
}

CConnection *CConditionBubble::trueLink()
{
    return m_trueLink;
}

CConnection *CConditionBubble::falseLink()
{
    return m_falseLink;
}
