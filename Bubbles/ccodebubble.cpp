#include "ccodebubble.h"

#include "Misc/ctextitem.h"
#include "Misc/Palette/cpaletteaction.h"

CCodeBubble::CCodeBubble(const QPointF &pos, CPaletteAction *palette, const QFont &font, QGraphicsItem *parent)
    : CSingleLinkBubble(pos, palette, font, parent)
{
    m_type = Chronicler::CodeBubble;

    m_code = new CTextItem("", QRectF(), this);

    AdjustMinSize();
    m_bounds = QRectF(-m_minSize.width()/2, -m_minSize.height()/2, m_minSize.width(), m_minSize.height());
    UpdatePolygon();

    setPalette(m_palette);
}

void CCodeBubble::setFont(const QFont &font)
{
    CSingleLinkBubble::setFont(font);
    m_code->setFont(font);
}

void CCodeBubble::setPalette(CPaletteAction *palette)
{
    CSingleLinkBubble::setPalette(palette);
    m_code->setColor(palette->getPalette().font);
}

void CCodeBubble::setCode(const QString &code)
{
    m_code->setText(code);
}

QString CCodeBubble::getCode() const
{
    return m_code->Text();
}

void CCodeBubble::UpdatePolygon()
{
    CSingleLinkBubble::UpdatePolygon();

    QRectF b = boundingRect();
    m_code->Resize(QRectF(b.x() + 10, b.y() + 10, b.width() - 20, b.height() - 12));
}

QDataStream &CCodeBubble::Deserialize(QDataStream &ds, const Chronicler::CVersion &version)
{
    Q_UNUSED(version)

    CSingleLinkBubble::Deserialize(ds, version);

    QString code;
    ds >> code;

    m_code->setText(code);

    return ds;
}

QDataStream &CCodeBubble::Serialize(QDataStream &ds) const
{
    CSingleLinkBubble::Serialize(ds);

    ds << m_code->Text();

    return ds;
}

void CCodeBubble::AdjustMinSize()
{
    QFontMetrics fm(m_font);
    m_minSize.setHeight(fm.height() + 30);
}
