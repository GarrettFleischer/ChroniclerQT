#ifndef CSTORYPROPERTIES_H
#define CSTORYPROPERTIES_H

#include "cpropertieswidget.h"

class CStoryBubble;
class CTextEdit;

class CStoryProperties : public CPropertiesWidget
{
    Q_OBJECT

public:
    CStoryProperties(QStringListModel *model, QWidget *parent = 0);

public:
    virtual void setBubble(CBubble *bbl);

    virtual void setFont(const QFont &font);

private:
    CStoryBubble *m_bbl;

    CTextEdit *m_storyEdit;

signals:

protected slots:
    virtual void LabelChanged(QString title);
    virtual void OrderChanged(QString order);
    virtual void LockedChanged(bool locked);

    void StoryChanged();
};

#endif // CSTORYPROPERTIES_H
