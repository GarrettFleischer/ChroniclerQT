#ifndef CSTARTHEREPROPERTIES_H
#define CSTARTHEREPROPERTIES_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QTableView;
class QLineEdit;
QT_END_NAMESPACE

class CBubble;
class CStartHereBubble;
class CCodeEdit;

class CStartHereProperties : public QWidget
{
    Q_OBJECT

public:
    explicit CStartHereProperties(QWidget *parent = 0);

    void setBubble(CBubble *bbl);

private:
    QLineEdit *m_label;
    QTableView *m_variablesView;
    CStartHereBubble *m_bubble;
    CCodeEdit *m_customCodeEdit;

public slots:
    void AddItem();
    void RemoveItem();

    void LabelChanged(QString label);
    void CodeChanged();
};

#endif // CSTARTHEREPROPERTIES_H
