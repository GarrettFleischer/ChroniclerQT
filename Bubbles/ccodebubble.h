#ifndef CCODEBUBBLE_H
#define CCODEBUBBLE_H

#include "csinglelinkbubble.h"

class CTextItem;

class CCodeBubble : public CSingleLinkBubble
{
public:
    CCodeBubble(const QPointF &pos, CPaletteAction *palette, const QFont &font = QFont(), QGraphicsItem *parent = Q_NULLPTR);

    virtual void setFont(const QFont &font) Q_DECL_OVERRIDE;
    virtual void setPalette(CPaletteAction *palette) Q_DECL_OVERRIDE;

    void setCode(const QString &code);
    QString getCode() const;

protected:
    virtual void UpdatePolygon() Q_DECL_OVERRIDE;

    virtual QDataStream &Deserialize(QDataStream &ds, const Chronicler::CVersion &version) Q_DECL_OVERRIDE;
    virtual QDataStream &Serialize(QDataStream &ds) const Q_DECL_OVERRIDE;

private:
    void AdjustMinSize();

    CTextItem * m_code;

};

#endif // CCODEBUBBLE_H
