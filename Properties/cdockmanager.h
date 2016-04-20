#ifndef CDOCKMANAGER_H
#define CDOCKMANAGER_H

#include <QObject>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QTabWidget;
class QStringListModel;
QT_END_NAMESPACE

class CPropertiesManager;
class CProjectView;

class CBubble;

class CDockManager : public QWidget
{
    Q_OBJECT

public:
    explicit CDockManager(QStringListModel *model, QWidget *parent = 0);

    virtual QSize sizeHint() const;

    void setBubble(CBubble *bbl, bool ignore = false);

private:
    QTabWidget *m_tabView;

    CPropertiesManager *m_properties;
    CProjectView *m_project;

signals:

public slots:

};

#endif // CDOCKMANAGER_H
