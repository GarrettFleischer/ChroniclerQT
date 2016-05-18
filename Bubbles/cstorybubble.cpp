#include "cstorybubble.h"

#include <QPainterPath>
#include <QtMath>

#include "Connections/cconnection.h"
//#include "Connections/clink.h"


CStoryBubble::CStoryBubble(const QPointF &pos, const CPalette &palette, const QFont &font, QGraphicsItem *parent)
    : CSingleLinkBubble(pos, palette, font, parent)//, m_resize(false)
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

    UpdatePolygon();
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
