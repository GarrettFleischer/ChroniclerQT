#ifndef QACTIONBUTTON_H
#define QACTIONBUTTON_H

#include <QPushButton>

QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE


class QActionButton : public QPushButton
{
    Q_OBJECT

public:
    explicit QActionButton(QWidget *parent = 0, QAction *action = 0, bool showText = false);

    void setAction( QAction* action );

private:
    QAction *m_action;
    bool m_showText;

private slots:
    void updateButtonStatusFromAction();
};

#endif // QACTIONBUTTON_H
