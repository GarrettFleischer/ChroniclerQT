#ifndef CSTATSMODEL_H
#define CSTATSMODEL_H

#include <QAbstractListModel>


class CStatsModel : public QAbstractListModel
{
public:
    CStatsModel(QObject *parent = Q_NULLPTR);
};

#endif // CSTATSMODEL_H
