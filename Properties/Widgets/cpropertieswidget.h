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

class CPropertiesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CPropertiesWidget(QStringListModel *model, QWidget *parent = 0);

    virtual void setBubble(CBubble *bbl);
    virtual void setFont(const QFont &font);

protected:
    CBubble *m_bubble;

    QStringListModel *m_model;
    QLineEdit *m_labelEdit;

    QCheckBox *m_lockEdit;
    QLineEdit *m_orderEdit;

    QVBoxLayout *m_layout;

signals:

protected slots:
    virtual void LabelChanged(QString label);
    virtual void OrderChanged(QString order);
    virtual void LockedChanged(bool locked);

};

#endif // CPROPERTIES_H
