#ifndef CSTATICON_H
#define CSTATICON_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

#include "Misc/chronicler.h"
using Chronicler::Stat;

class CStatIcon : public QWidget
{
    Q_OBJECT
public:
    explicit CStatIcon(const Chronicler::Stat &type, const QPixmap &pixmap, const QString &text, QWidget *parent = Q_NULLPTR);

    Stat type() const;
    QPixmap pixmap() const;
    QString text() const;

private:
    QLabel *m_icon;
    QLabel *m_text;
    Stat m_type;

};

#endif // CSTATICON_H
