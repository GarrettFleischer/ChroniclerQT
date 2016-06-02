#include "cpaletteaction.h"

#include <QIcon>
#include <QPainter>
#include <QPixmap>

CPaletteAction::CPaletteAction(QObject *parent, const Chronicler::CPalette &palette, const QString &name)
    : QAction(parent), m_palette(palette)
{
    setText(name);
    setIcon(PaletteIcon());
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
