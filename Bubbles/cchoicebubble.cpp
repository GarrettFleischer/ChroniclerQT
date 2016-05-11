#include "cchoicebubble.h"

#include "Misc/cstringlistmodel.h"
#include "Misc/ctextitem.h"

CChoiceBubble::CChoiceBubble(QMenu *contextMenu, const QPointF &pos, const Chronicler::CPalette &palette, const QFont &font, QGraphicsItem *parent)
    : CListBubble(contextMenu, pos, palette, font, parent)
{
    m_type = Chronicler::Choice;

    m_palette.fill = QColor(151,118,166);

    QRectF min_bounds(-m_minSize.width()/2, -m_minSize.height()/2, m_minSize.width()*2, m_minSize.height()*2);
    m_modelView = new CTextItem("", min_bounds, this);

    QStringList test;
    test.append("Code");
    test.append("Eat");
    test.append("Sleep");
    test.append("In that order...");

    m_model->setStringList(test);

}

void CChoiceBubble::setPalette(const Chronicler::CPalette &palette)
{
    CBubble::setPalette(palette);
    m_modelView->setColor(palette.font);
}


void CChoiceBubble::AddLink(CConnection *link)
{

}

void CChoiceBubble::RemoveLink(CConnection *link)
{

}

QList<CConnection *> CChoiceBubble::links()
{
    return {};
}

void CChoiceBubble::UpdatePolygon()
{
    qreal padding = 10;
    QRectF tb = m_modelView->textBounds(m_minSize);
    m_modelView->Resize(tb);

    QRectF b(tb.x() - padding, tb.y() - padding,
             tb.width() + padding*2, tb.height() + padding*2);

    QPainterPath path;
    path.addRoundedRect(b, 10, 10);

    setPolygon(path.toFillPolygon());
}

void CChoiceBubble::ModelUpdated()
{
    QString txt;
    for(QString action : m_model->stringList())
        txt += action + '\n';

    m_modelView->setText(txt);
    UpdatePolygon();
}
