#ifndef CPROPERTIES_H
#define CPROPERTIES_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QCheckBox>

QT_BEGIN_NAMESPACE
class QStringListModel;
class QFont;
QT_END_NAMESPACE

class CBubble;
class CLineEdit;

#include "Misc/chronicler.h"
using Chronicler::shared;

class CPropertiesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CPropertiesWidget(QWidget *parent = Q_NULLPTR);

    virtual void setBubble(CBubble *bbl);

protected:
    CBubble *m_bubble;

    QLineEdit *m_labelEdit;

    QCheckBox *m_lockEdit;
    QLineEdit *m_orderEdit;

    QVBoxLayout *m_layout;

signals:

public slots:
    virtual void LabelChanged(QString label);
    virtual void OrderChanged(QString order);
    virtual void LockedChanged(bool locked);

};

#endif // CPROPERTIES_H
