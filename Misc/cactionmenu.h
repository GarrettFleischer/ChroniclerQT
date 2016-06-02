#ifndef CACTIONMENU_H
#define CACTIONMENU_H

#include <QMenu>

class CActionMenu : public QMenu
{
    Q_OBJECT

public:
    CActionMenu(QWidget *parent = 0);

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;

signals:
    void leftTriggered(QAction *);
    void rightTriggered(QAction *);
};

#endif // CACTIONMENU_H
