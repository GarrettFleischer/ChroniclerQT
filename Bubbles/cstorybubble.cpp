#include "cstorybubble.h"

#include <QPainterPath>
#include <QtMath>

#include "Connections/cconnection.h"
//#include "Connections/clink.h"


CStoryBubble::CStoryBubble(QMenu *contextMenu, const QPointF &pos, const CPalette &palette, const QFont &font, QGraphicsItem *parent)
    : CSingleLinkBubble(contextMenu, pos, palette, font, parent), m_resize(false)
{
    m_type = Chronicler::Story;
    
    setPolygon(QPolygonF(QRectF(-100,-100,200,200)));

    m_palette.fill = QColor(124, 140, 230);

    m_title = new CTextItem("", QRectF(), this);
    m_title->SetStyle(Qt::AlignHCenter);
    setLabel("Title");

    m_story = new CTextItem("Story", QRectF(), this);
    m_story->SetStyle(Qt::AlignLeft);
    
    setCursor(Qt::PointingHandCursor);
    setAcceptHoverEvents(true);

    UpdatePolygon();
}

void CStoryBubble::mousePressEvent(QGraphicsSceneMouseEvent *evt)
{
    CBubble::mousePressEvent(evt);
    

    QRectF b = sceneBoundingRect();
    QRectF resizeRect(QPointF(b.x() + b.width() - 20, b.y() + b.height() - 20), QSizeF(20,20));

    if(resizeRect.contains(evt->scenePos()))
    {
        m_resize = true;
        m_offset = evt->scenePos();
        m_lastBounds = boundingRect();
    }
}

void CStoryBubble::mouseReleaseEvent(QGraphicsSceneMouseEvent *evt)
{
    CBubble::mouseReleaseEvent(evt);
    setCursor(Qt::PointingHandCursor);
    m_resize = false;
}

void CStoryBubble::mouseMoveEvent(QGraphicsSceneMouseEvent *evt)
{
    if(m_resize)
    {
        QPointF delta(evt->scenePos() - m_offset);
        // to update boundingRect....
        setPolygon(QRectF(m_lastBounds.x(), m_lastBounds.y(),
                          qMax<float>(m_lastBounds.width() + delta.x(), m_minSize.width()),
                          qMax<float>(m_lastBounds.height() + delta.y(), m_minSize.height())));
        UpdatePolygon();
        emit PositionChanged();
    }
    else
        CBubble::mouseMoveEvent(evt);
}

void CStoryBubble::hoverMoveEvent(QGraphicsSceneHoverEvent *evt)
{
    QRectF b = sceneBoundingRect();
    QRectF resizeRect(QPointF(b.x() + b.width() - 20, b.y() + b.height() - 20), QSizeF(20,20));

    if(resizeRect.contains(evt->scenePos()))
        setCursor(Qt::SizeFDiagCursor);
    else
        setCursor(Qt::PointingHandCursor);
}


void CStoryBubble::UpdatePolygon()
{
    QRectF b = boundingRect();
    qreal th = m_title->textBounds().height() + 10.0f;
    qreal tm = b.width()*2/3;

    m_title->Resize(QRectF(b.x() + 10, b.y() + 2, tm - 20, th));
    m_story->Resize(QRectF(b.x() + 10, b.y() + th + 10, b.width() - 20, b.height() - th - 12));
    if(m_title->textBounds().width() > m_title->boundingRect().width())
        m_title->SetStyle(Qt::AlignLeft);
    else
        m_title->SetStyle(Qt::AlignHCenter);

    QPainterPath path;
    path.setFillRule( Qt::WindingFill );
    path.addRoundedRect( QRectF(b.x(),b.y() + th, b.width(), b.height() - th ), 10, 10 );
    path.addRect( QRectF( b.x(), b.y() + th, b.width()*3/4, b.height()/2 ) ); // Top left corner not rounded
    path.addRect( QRectF( b.x() + b.width()/2, b.y() + b.height()/2, b.width()/2, b.height()/2 ) ); // Bottom right corner not rounded
    path.moveTo(b.x(), b.y() + th);
    path.lineTo(b.x() + 10, b.y());
    path.lineTo(b.x() + tm - 10, b.y());
    path.lineTo(b.x() + tm, b.y() + th);

    setPolygon(path.simplified().toFillPolygon());
}

void CStoryBubble::setFont(const QFont &font)
{
    if(font != m_font)
    {
        m_font = font;
        m_title->setFont(m_font);
        m_story->setFont(m_font);
        UpdatePolygon();
    }
}


void CStoryBubble::setLabel(QString label)
{
    CBubble::setLabel(label);
    m_title->setText(label);
}

void CStoryBubble::setPalette(const Chronicler::CPalette &palette)
{
    m_title->setColor(palette.font);
    m_story->setColor(palette.font);
    CBubble::setPalette(palette);
}
