#ifndef CACTIONBUTTON_H
#define CACTIONBUTTON_H

#include <QPushButton>

QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE


class QActionButton : public QPushButton
{
    Q_OBJECT

public:
    explicit QActionButton(QWidget *parent = 0, QAction *action = 0);

    void setAction( QAction* action );

private:
    QAction *m_action;

private slots:
    void updateButtonStatusFromAction();
};

#endif // CACTIONBUTTON_H
