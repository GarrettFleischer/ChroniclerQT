#ifndef CPROJECTVIEW_H
#define CPROJECTVIEW_H

#include <QObject>
#include <QWidget>

class CPropertiesManager;

class CProjectView : public QWidget
{
    Q_OBJECT
public:
    explicit CProjectView(QWidget *parent = 0);

signals:

private slots:
};

#endif // CPROJECTVIEW_H
