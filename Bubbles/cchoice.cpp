#include "cchoice.h"

#include <QFontMetrics>
#include <QPainterPath>

#include "Misc/ctextitem.h"

#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>

#include "Connections/cconnection.h"
#include "cgraphicsscene.h"


CChoice::CChoice(const Chronicler::CPalette &palette, const QFont &font, QGraphicsItem *parent, const QString &choice)
    : CSingleLinkBubble(QPointF(), palette, font, parent)
{
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);

    connect(qgraphicsitem_cast<CBubble *>(parent), SIGNAL(PositionChanged()), this, SIGNAL(PositionChanged()));

    m_choice = new CTextItem(choice, m_bounds, this);
    m_choice->SetStyle(Qt::AlignCenter);

    AdjustMinSize();
    m_bounds = QRectF(0, 0, m_minSize.width(), m_minSize.height());
    UpdatePolygon();
}

void CChoice::setPalette(const Chronicler::CPalette &palette)
{
    CSingleLinkBubble::setPalette(palette);
    m_choice->setColor(palette.font);
}

void CChoice::setFont(const QFont &font)
{
    CSingleLinkBubble::setFont(font);
    m_choice->setFont(font);
    AdjustMinSize();
    UpdatePolygon();
}

void CChoice::setChoice(const QString &choice)
{
    m_choice->setText(choice);
}

QString CChoice::choice() const
{
    return m_choice->Text();
}

void CChoice::setWidth(const qreal &width)
{
    m_bounds.setWidth(width);

    UpdatePolygon();
}

void CChoice::UpdatePolygon()
{
    m_choice->Resize(m_bounds);

    QPainterPath path;
    path.addRect(m_bounds);

    setPolygon(path.toFillPolygon());
}

void CChoice::AdjustMinSize()
{
    QFontMetrics fm(m_font);
    m_minSize.setHeight(fm.height());
}


void CChoice::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    event->ignore();
}

void CChoice::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    CSingleLinkBubble::mousePressEvent(event);
    event->ignore();
}

void CChoice::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    CSingleLinkBubble::mouseMoveEvent(event);
    event->ignore();
}

void CChoice::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    CSingleLinkBubble::hoverMoveEvent(event);
    event->ignore();
}

void CChoice::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    CSingleLinkBubble::mouseReleaseEvent(event);
    event->ignore();
}

Chronicler::Anchor CChoice::OutputAnchorAtPosition(const QPointF &pos)
{
    if(pos.x() < sceneBoundingRect().center().x())
        return Chronicler::Left;

    return Chronicler::Right;
}

// Inputs are applied to the CChoiceBubble that contains this choice.
Chronicler::Anchor CChoice::InputAnchorAtPosition(const QPointF &)
{
    return Chronicler::None;
}

QDataStream & CChoice::Read(QDataStream &ds)
{
    QString choice;
    bool linked;

    ds >> m_UID >> choice >> linked;

    if(linked)
    {
        m_link = dynamic_cast<CGraphicsScene *>(scene())->AddConnection();
        m_link->Read(ds);
    }

    m_choice->setText(choice);

    return ds;
}

QDataStream & CChoice::Write(QDataStream &ds)
{
    ds << m_UID << m_choice->Text() << bool(m_link);

    if(m_link)
        m_link->Write(ds);

    return ds;
}
