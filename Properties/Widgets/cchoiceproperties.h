#ifndef CCHOICEPROPERTIES_H
#define CCHOICEPROPERTIES_H

#include "cpropertieswidget.h"

QT_BEGIN_NAMESPACE
class QListView;
QT_END_NAMESPACE

class CChoiceBubble;

class CChoiceProperties : public CPropertiesWidget
{
    Q_OBJECT
public:
    explicit CChoiceProperties(QStringListModel *model, QWidget *parent = 0);

    virtual void setBubble(CBubble *bbl);

private:
    CChoiceBubble *m_choiceBubble;

    QListView *m_choices;

signals:

};

#endif // CCHOICEPROPERTIES_H
