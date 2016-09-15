#include "cstaticon.h"

#include <QLabel>
#include <QVBoxLayout>


CStatIcon::CStatIcon(const Stat &type, const QPixmap &icon, const QString &label, QWidget *parent)
    : QWidget(parent), m_type(type)
{
    m_icon = new QLabel(this);
    m_icon->setPixmap(icon);

    m_text = new QLabel(this);
    m_text->setText(label);
    m_text->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_icon);
    layout->addWidget(m_text);
}

Chronicler::Stat CStatIcon::type() const
{
    return m_type;
}

QPixmap CStatIcon::pixmap() const
{
    return *m_icon->pixmap();
}

QString CStatIcon::text() const
{
    return m_text->text();
}
