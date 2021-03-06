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
    CStoryProperties(QWidget *parent = Q_NULLPTR);

    virtual void setBubble(CBubble *bbl);

private:
    void TagSelectedText(const QString &tag);

    CStoryBubble *m_storyBubble;

    CTextEdit *m_storyEdit;

    QAction *m_boldAction;
    QAction *m_italicAction;

    QStringListModel *m_completionModel;

signals:

protected slots:
    void StoryChanged();

    void BoldTriggered();
    void ItalicTriggered();

private slots:
    void UpdateModel();
};

#endif // CSTORYPROPERTIES_H
