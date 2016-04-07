#ifndef CPROPERTIESVIEW_H
#define CPROPERTIESVIEW_H

#include <QWidget>
#include <QStringListModel>

#include "Properties/cstoryproperties.h"
#include "Properties/cconditionproperties.h"

class CPropertiesView : public QWidget
{
    Q_OBJECT

public:
    explicit CPropertiesView(QStringListModel *model, QWidget *parent = 0);

public:
    void SetBubble(CBubble *bbl);

    virtual void setFont(const QFont &font);
    
    virtual QSize sizeHint() const;

private:
    //CBubble::BType ItemType(QGraphicsItem *item);

private:
    QVBoxLayout * m_layout;

    CStoryProperties *m_storyProperties;
    CConditionProperties *m_conditionProperties;



signals:

public slots:
//    void SelectedItemChanged(QGraphicsItem *item);
};

#endif // CPROPERTIESVIEW_H
