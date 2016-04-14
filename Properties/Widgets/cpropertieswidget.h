#ifndef CPROPERTIES_H
#define CPROPERTIES_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QStringListModel;
class QFont;
class QLineEdit;
class QCheckBox;
class QVBoxLayout;
QT_END_NAMESPACE

class CBubble;
class CLineEdit;

class CPropertiesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CPropertiesWidget(QStringListModel *model, QWidget *parent = 0);

    virtual void setBubble(CBubble *bbl) = 0;
    virtual void setFont(const QFont &font) = 0;

protected:
    QStringListModel *m_model;
    QLineEdit *m_labelEdit;

    QCheckBox *m_lockEdit;
    QLineEdit *m_orderEdit;

    QVBoxLayout *m_layout;

signals:

protected slots:
    virtual void LabelChanged(QString label) = 0;
    virtual void OrderChanged(QString order) = 0;
    virtual void LockedChanged(bool locked) = 0;

};

#endif // CPROPERTIES_H
