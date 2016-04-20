#include "cstorybubble.h"

#include <QPainterPath>


CStoryBubble::CStoryBubble(QMenu *contextMenu, const QPointF &pos, const CPalette &palette, const QFont &font, QGraphicsItem *parent)
    : CBubble(contextMenu, pos, palette, font, parent), m_resize(false)
{
    m_type = Chronicler::Story;
    
    setPolygon(QPolygonF(QRectF(-100,-100,200,200)));

    
    m_palette.fill = QColor(124, 140, 230); //(106,116,163);//(106,136,213);//(150,150,255);

    m_title = new CTextItem("", QRectF(), this);
    m_title->SetStyle(Qt::AlignHCenter);
    setLabel("Title");

    //QString s("Allow miles wound place the leave had.\nTo sitting subject no improve studied limited.\nYe indulgence unreserved connection alteration appearance my an astonished.\nUp as seen sent make he they of.\nHer raising and himself pasture believe females.\nFancy she stuff after aware merit small his.\nCharmed esteems luckily age out.\nBy an outlived insisted procured improved am.\nPaid hill fine ten now love even leaf.\nSupplied feelings mr of dissuade recurred no it offering honoured.\nAm of of in collecting devonshire favourable excellence.\nHer sixteen end ashamed cottage yet reached get hearing invited.\nResources ourselves sweetness ye do no perfectly.\nWarmly warmth six one any wisdom.\nFamily giving is pulled beauty chatty highly no.\nBlessing appetite domestic did mrs judgment rendered entirely.\nHighly indeed had garden not.\nPost no so what deal evil rent by real in.\nBut her ready least set lived spite solid.\nSeptember how men saw tolerably two behaviour arranging.\nShe offices for highest and replied one venture pasture.\nApplauded no discovery in newspaper allowance am northward.\nFrequently partiality possession resolution at or appearance unaffected he me.\nEngaged its was evident pleased husband.\nYe goodness felicity do disposal dwelling no.\nFirst am plate jokes to began of cause an scale.\nSubjects he prospect elegance followed no overcame possible it on.\nLooking started he up perhaps against.\nHow remainder all additions get elsewhere resources.\nOne missed shy wishes supply design answer formed.\nPrevent on present hastily passage an subject in be.\nBe happiness arranging so newspaper defective affection ye.\nFamilies blessing he in to no daughter.\nDo so written as raising parlors spirits mr elderly.\nMade late in of high left hold.\nCarried females of up highest calling.\nLimits marked led silent dining her she far.\nSir but elegance marriage dwelling likewise position old pleasure men.\nDissimilar themselves simplicity no of contrasted as.\nDelay great day hours men.\nStuff front to do allow to asked he.\nOf recommend residence education be on difficult repulsive offending.\nJudge views had mirth table seems great him for her.\nAlone all happy asked begin fully stand own get.\nExcuse ye seeing result of we.\nSee scale dried songs old may not.\nPromotion did disposing you household any instantly.\nHills we do under times at first short an.");

    m_story = new CTextItem("Story", QRectF(), this);
    m_story->SetStyle(Qt::AlignLeft);// | Qt::TextWordWrap);
    
    setCursor(Qt::PointingHandCursor);
    setAcceptHoverEvents(true);

    UpdateShape();
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
        UpdateShape();
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


void CStoryBubble::UpdateShape()
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
        UpdateShape();
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
