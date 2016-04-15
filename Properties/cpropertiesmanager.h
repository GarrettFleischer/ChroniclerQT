#ifndef CPROPERTIESVIEW_H
#define CPROPERTIESVIEW_H

#include <QObject>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QStringListModel;
QT_END_NAMESPACE

class CStoryProperties;
class CConditionProperties;
class CChoiceProperties;
class CBubble;

class CPropertiesManager : public QWidget
{
    Q_OBJECT

public:
    explicit CPropertiesManager(QStringListModel *model, QWidget *parent = 0);

public:
    void setBubble(CBubble *bbl);

    virtual void setFont(const QFont &font);

private:

    CStoryProperties *m_storyProperties;
    CConditionProperties *m_conditionProperties;
    CChoiceProperties *m_choiceProperties;

signals:

public slots:
};

#endif // CPROPERTIESVIEW_H
