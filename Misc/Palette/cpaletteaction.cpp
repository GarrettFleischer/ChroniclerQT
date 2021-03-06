#include "cpaletteaction.h"

#include <QIcon>
#include <QPainter>
#include <QPixmap>

QList<t_uid> CPaletteAction::m_UIDs = QList<t_uid>();

CPaletteAction::CPaletteAction(QObject *parent, const Chronicler::CPalette &palette, const QString &name, Chronicler::t_uid uid)
    : QAction(parent), m_uid(uid), m_palette(palette)
{
    setText(name);
    setIcon(PaletteIcon());

    m_UIDs.append(m_uid);
}

Chronicler::CPalette CPaletteAction::getPalette() const
{
    return m_palette;
}

void CPaletteAction::setPalette(const CPalette &palette)
{
    m_palette = palette;
    setIcon(PaletteIcon());
    emit changed();
}

Chronicler::t_uid CPaletteAction::getUID() const
{
    return m_uid;
}

QIcon CPaletteAction::PaletteIcon()
{
    QPixmap pixmap(32, 32);
    QPainter painter(&pixmap);

    painter.fillRect(QRect(QPoint(), pixmap.size()), m_palette.fill);

    // half of rect outlined in line color
    painter.setPen(QPen(QBrush(m_palette.line), 3));
    painter.drawLine(0, 0, pixmap.width()/2, 0);
    painter.drawLine(0, 0, 0, pixmap.height());
    painter.drawLine(0, pixmap.height(), pixmap.width()/2, pixmap.height());

    // other half in select color
    painter.setPen(QPen(QBrush(m_palette.select), 3));
    painter.drawLine(pixmap.width()/2, 0, pixmap.width(), 0);
    painter.drawLine(pixmap.width(), 0, pixmap.width(), pixmap.height());
    painter.drawLine(pixmap.width()/2, pixmap.height(), pixmap.width(), pixmap.height());

    // draw demo font as well
    painter.setPen(QPen(m_palette.font));
    painter.drawText(pixmap.rect(), Qt::AlignCenter, "P");

    return QIcon(pixmap);
}

Chronicler::t_uid CPaletteAction::generateUID()
{
    t_uid lowest = 1;
    while(m_UIDs.contains(lowest))
        ++lowest;

    return lowest;
}
