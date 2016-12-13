#ifndef CSTARTHEREPROPERTIES_H
#define CSTARTHEREPROPERTIES_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QTableView;
QT_END_NAMESPACE

class CBubble;

class CStartHereProperties : public QWidget
{
    Q_OBJECT

public:
    explicit CStartHereProperties(QWidget *parent = 0);

    void setBubble(CBubble *bbl);

private:
    QTableView *m_view;

public slots:
    void AddItem();
    void RemoveItem();
};

#endif // CSTARTHEREPROPERTIES_H
