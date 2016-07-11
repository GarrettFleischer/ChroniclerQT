#ifndef CVARIABLESVIEW_H
#define CVARIABLESVIEW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QTableView;
QT_END_NAMESPACE

class CVariablesModel;

class CVariablesView : public QWidget
{
    Q_OBJECT
public:
    explicit CVariablesView(QWidget *parent = 0);

private:
    QTableView      *m_view;
    CVariablesModel *m_model;

signals:

public slots:
};

#endif // CVARIABLESVIEW_H
