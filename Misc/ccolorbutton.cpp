#include "ccolorbutton.h"

#include <QIcon>
#include <QPainter>
#include <QPixmap>
#include <QColorDialog>


CColorButton::CColorButton(const QSize &size, const QColor &color, QWidget *parent)
    : QPushButton(parent), m_color(color)
{
    resize(size);
    setIcon(ColorIcon());

    connect(this, SIGNAL(clicked(bool)), this, SLOT(ShowDialog()));

    m_dialog = new QColorDialog(m_color);
    connect(m_dialog, SIGNAL(currentColorChanged(QColor)), this, SLOT(ColorChanged(QColor)));
    connect(m_dialog, SIGNAL(colorSelected(QColor)), this, SLOT(ColorChanged(QColor)));
}

void CColorButton::setColor(const QColor &color)
{
    m_dialog->setCurrentColor(color); // connected to ColorChanged()
}

QColor CColorButton::getColor()
{
    return m_color;
}


QIcon CColorButton::ColorIcon()
{
    QPixmap pixmap(iconSize());
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(pixmap.rect(), m_color);

    return QIcon(pixmap);
}

void CColorButton::ShowDialog()
{
    m_dialog->show();
}

void CColorButton::ColorChanged(const QColor &color)
{
    m_color = color;
    setIcon(ColorIcon());
    emit colorChanged(color);
}
