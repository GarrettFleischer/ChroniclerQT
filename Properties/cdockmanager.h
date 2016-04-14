#ifndef CDOCKMANAGER_H
#define CDOCKMANAGER_H

#include <QObject>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QTabWidget;
class QStringListModel;
QT_END_NAMESPACE

class CPropertiesView;
class CProjectView;

class CBubble;

class CDockManager : public QWidget
{
    Q_OBJECT
public:
    explicit CDockManager(QStringListModel *model, QWidget *parent = 0);

    void setBubble(CBubble *bbl);

    CPropertiesView & properties();

private:
    QTabWidget *m_tabView;

    CPropertiesView *m_properties;
    CProjectView *m_project;

signals:

public slots:

    // QWidget interface
public:
    virtual QSize sizeHint() const;
};

#endif // CDOCKMANAGER_H
