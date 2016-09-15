#ifndef CSTATSEDITOR_H
#define CSTATSEDITOR_H

#include <QWidget>

class CStatsEditor : public QWidget
{
    Q_OBJECT
public:
    explicit CStatsEditor(QWidget *parent = 0);

protected:
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

signals:

public slots:


};

#endif // CSTATSEDITOR_H
