#ifndef CLISTBUTTONS_H
#define CLISTBUTTONS_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
QT_END_NAMESPACE

class CListButtons : public QWidget
{
    Q_OBJECT
public:
    enum ActiveButtons { Up = 1, Down = 2, Add = 4, Remove = 8 };

    explicit CListButtons(QWidget *parent = 0, int activeButtons = Up | Down | Add | Remove);

signals:
    void addItem();
    void removeItem();
    void moveDown();
    void moveUp();

public slots:
};

#endif // CLISTBUTTONS_H
