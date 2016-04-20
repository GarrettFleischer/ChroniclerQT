#ifndef CCHOICEPROPERTIES_H
#define CCHOICEPROPERTIES_H

#include "cpropertieswidget.h"

QT_BEGIN_NAMESPACE
class QListWidget;
QT_END_NAMESPACE

class CChoiceBubble;

class CChoiceProperties : public CPropertiesWidget
{
    Q_OBJECT
public:
    explicit CChoiceProperties(QStringListModel *model, QWidget *parent = 0);

    virtual void setBubble(CBubble *bbl);
    virtual void setFont(const QFont &font);

private:
    CChoiceBubble *m_choiceBubble;

    QListWidget *m_choices;

signals:

protected slots:
    virtual void ChoicesChanged(const QStringList &list);

};

#endif // CCHOICEPROPERTIES_H
