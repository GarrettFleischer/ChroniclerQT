#ifndef CLINE_H
#define CLINE_H

#include <QObject>
#include<QGraphicsItem>

class CLine : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit CLine(QObject *parent = 0);

signals:

public slots:
};

#endif // CLINE_H
