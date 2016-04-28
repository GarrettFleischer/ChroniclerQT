#ifndef ACTIONBUTTON_H
#define ACTIONBUTTON_H

#include <QPushButton>

QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE


class ActionButton : public QPushButton
{
    Q_OBJECT

public:
    explicit ActionButton(QWidget *parent = 0, QAction *action = 0);

    void setAction( QAction* action );

private:
    QAction *m_action;

private slots:
    void updateButtonStatusFromAction();
};

#endif // ACTIONBUTTON_H
