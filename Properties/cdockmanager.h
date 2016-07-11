#ifndef CDOCKMANAGER_H
#define CDOCKMANAGER_H

#include <QObject>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QTabWidget;
class QStringListModel;
class QMenu;
QT_END_NAMESPACE

class CPropertiesManager;

class CBubble;

class CDockManager : public QWidget
{
    Q_OBJECT

public:
    explicit CDockManager(QWidget *parent = 0);

    virtual QSize sizeHint() const;

    void setBubble(CBubble *bbl, bool ignore = false);

private:
    QTabWidget *m_tabView;

    CPropertiesManager *m_properties;



signals:

public slots:

};

#endif // CDOCKMANAGER_H
