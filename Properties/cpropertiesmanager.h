#ifndef CPROPERTIESVIEW_H
#define CPROPERTIESVIEW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QStringListModel;
QT_END_NAMESPACE

class CStoryProperties;
class CConditionProperties;
class CChoiceProperties;
class CActionProperties;
class CCodeProperties;
class CStartHereProperties;
class CBubble;

class CPropertiesManager : public QWidget
{
public:
    explicit CPropertiesManager(QWidget *parent = 0);

    void setBubble(CBubble *bbl);

private:
    CStoryProperties *m_storyProperties;
    CConditionProperties *m_conditionProperties;
    CChoiceProperties *m_choiceProperties;
    CActionProperties *m_actionProperties;
    CCodeProperties *m_codeProperties;
    CStartHereProperties *m_startHereProperties;

};

#endif // CPROPERTIESVIEW_H
