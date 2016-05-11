#ifndef CSTORYPROPERTIES_H
#define CSTORYPROPERTIES_H

#include "cpropertieswidget.h"

QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE

class CStoryBubble;
class CTextEdit;

class CStoryProperties : public CPropertiesWidget
{
    Q_OBJECT

public:
    CStoryProperties(QStringListModel *model, QWidget *parent = 0);

public:
    virtual void setBubble(CBubble *bbl);

private:
    void TagSelectedText(const QString &tag);

    CStoryBubble *m_storyBubble;

    CTextEdit *m_storyEdit;

    QAction *m_boldAction;
    QAction *m_italicAction;

signals:

protected slots:
    void StoryChanged();

    void BoldTriggered();
    void ItalicTriggered();
};

#endif // CSTORYPROPERTIES_H
