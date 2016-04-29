#include "cchoicebubble.h"

CChoiceBubble::CChoiceBubble(QMenu *contextMenu, const QPointF &pos, const Chronicler::CPalette &palette, const QFont &font, QGraphicsItem *parent)
    : CBubble(contextMenu, pos, palette, font, parent)
{
    m_type = Chronicler::Choice;

    m_palette.fill = QColor(151,118,166);

    m_choices.append(new CTextItem("", QRectF(), this));
    m_choices[0]->SetStyle(Qt::AlignHCenter);

    m_choiceStrings.append("Code");
    m_choiceStrings.append("Eat");
    m_choiceStrings.append("Sleep");
    m_choiceStrings.append("In that order...");

    AdjustMinSize();

    setPolygon(QPolygonF(QRectF(-m_minSize.width()/2, -m_minSize.height()/2, m_minSize.width()*2, m_minSize.height()*2)));
    UpdatePolygon();
}



void CChoiceBubble::UpdatePolygon()
{
    qreal padding = 10;
    QRectF tb = m_choices[0]->textBounds(m_minSize);
    m_choices[0]->Resize(tb);

    QRectF b(tb.x() - padding, tb.y() - padding,
             tb.width() + padding*2, tb.height() + padding*2);

    QPainterPath path;
    path.addRoundedRect(b, 10, 10);

    setPolygon(path.toFillPolygon());
}

void CChoiceBubble::AdjustMinSize()
{
    QFontMetrics fm(m_font);

    m_minSize.setWidth(100);
    m_minSize.setHeight(fm.height());
}

void CChoiceBubble::setFont(const QFont &font)
{
    CBubble::setFont(font);
    for(CTextItem *itm : m_choices)
        itm->setFont(m_font);
    UpdatePolygon();
}


void CChoiceBubble::AddChoice(const QString &choice)
{
    m_choices.append(new CTextItem(choice, QRectF(), this));
    UpdatePolygon();
}

void CChoiceBubble::MoveChoice(int old_index, int new_index)
{

}



void CChoiceBubble::setPalette(const Chronicler::CPalette &palette)
{
    CBubble::setPalette(palette);
    for(CTextItem *itm : m_choices)
        itm->setColor(m_palette.font);
}


void CChoiceBubble::RemoveLink(CConnection *link)
{
}

void CChoiceBubble::AddLink(CConnection *link, int position)
{
}

QList<CConnection *> CChoiceBubble::links()
{
    return {};
}
