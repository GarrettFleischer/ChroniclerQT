#ifndef CPROJECTMODEL_H
#define CPROJECTMODEL_H

#include <QObject>
#include <QList>
#include <QString>

class CGraphicsScene;


class CProjectModel : public QObject
{
    Q_OBJECT
public:
    explicit CProjectModel(QObject *parent = Q_NULLPTR);

    QList<QString> getLabels();
    QList<QString> getLabels(CGraphicsScene *scene);

signals:

public slots:
};

#endif // CPROJECTMODEL_H
