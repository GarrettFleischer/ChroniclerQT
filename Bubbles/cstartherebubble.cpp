#include "cstartherebubble.h"

#include "Misc/Bubbles/cstartheremodel.h"
#include "Misc/ctextitem.h"
#include "Misc/Palette/cpaletteaction.h"

CStartHereBubble::CStartHereBubble(const QPointF &pos, CPaletteAction *palette, const QFont &font, QGraphicsItem *parent)
    : CSingleLinkBubble(pos, palette, font, parent)
{
    m_type = Chronicler::StartHereBubble;

    m_model = new CStartHereModel(this);

    m_title = new CTextItem("", m_bounds, this);
    m_title->SetStyle(Qt::AlignCenter);

    m_minSize = QSizeF(100, 50);

    m_bounds = QRectF(QPointF(0, 0), m_minSize);
    UpdatePolygon();
    setPalette(m_palette);
}


QDataStream &CStartHereBubble::Deserialize(QDataStream &ds, const Chronicler::CVersion &version)
{
    CSingleLinkBubble::Deserialize(ds, version);

    QList<CVariable> modelData;
    ds >> modelData;

    m_model->setVariables(modelData);

    return ds;
}

QDataStream &CStartHereBubble::Serialize(QDataStream &ds) const
{
    CSingleLinkBubble::Serialize(ds);

    ds << m_model->variables();

    return ds;
}

CStartHereModel *CStartHereBubble::model() const
{
    return m_model;
}


void CStartHereBubble::UpdatePolygon()
{
    CSingleLinkBubble::UpdatePolygon();

    m_title->Resize(boundingRect());
}


void CStartHereBubble::setLabel(QString label)
{
    CSingleLinkBubble::setLabel(label);

    m_title->setText(label);
}


void CStartHereBubble::setFont(const QFont &font)
{
    if(font != m_font)
    {
        m_font = font;
        m_title->setFont(m_font);
    }
}


void CStartHereBubble::setPalette(CPaletteAction *palette)
{
    m_title->setColor(palette->getPalette().font);

    CSingleLinkBubble::setPalette(palette);
}
