#include "cpropertiesmanager.h"

#include <QVBoxLayout>

#include "Widgets/cstoryproperties.h"
#include "Widgets/cconditionproperties.h"
#include "Widgets/cchoiceproperties.h"
#include "Widgets/cactionproperties.h"
#include "Widgets/ccodeproperties.h"
#include "Bubbles/cbubble.h"


CPropertiesManager::CPropertiesManager(QWidget *parent)
    : QWidget(parent)
{
    m_storyProperties = new CStoryProperties(parent);
    m_storyProperties->setEnabled(false);

    m_conditionProperties = new CConditionProperties(parent);
    m_conditionProperties->setEnabled(false);
    m_conditionProperties->hide();

    m_choiceProperties = new CChoiceProperties(parent);
    m_choiceProperties->setEnabled(false);
    m_choiceProperties->hide();

    m_actionProperties = new CActionProperties(parent);
    m_actionProperties->setEnabled(false);
    m_actionProperties->hide();

    m_codeProperties = new CCodeProperties(parent);
    m_codeProperties->setEnabled(false);
    m_codeProperties->hide();

    
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_storyProperties);
    layout->addWidget(m_conditionProperties);
    layout->addWidget(m_choiceProperties);
    layout->addWidget(m_actionProperties);
    layout->addWidget(m_codeProperties);
    setLayout(layout);

//    QPalette Pal(palette());
//    Pal.setColor(QPalette::Background, Qt::gray);//QColor(86,96,123));
//    setAutoFillBackground(true);
//    setPalette(Pal);

//    QColor bc(Qt::gray);
//    QString style = QString("background-color: rgb(%1, %2, %3)").arg(bc.red()).arg(bc.green()).arg(bc.blue());
//    setAutoFillBackground(true);
//    setStyleSheet(style);

}



void CPropertiesManager::setBubble(CBubble *bbl)
{
    if(bbl)
    {
        m_storyProperties->hide();
        m_conditionProperties->hide();
        m_choiceProperties->hide();
        m_actionProperties->hide();
        m_codeProperties->hide();

        switch(bbl->getType())
        {
        case Chronicler::StartBubble:
            break;

        case Chronicler::StoryBubble:
            m_storyProperties->setBubble(bbl);
            m_storyProperties->show();
        break;

        case Chronicler::ChoiceBubble:
            m_choiceProperties->setBubble(bbl);
            m_choiceProperties->show();
        break;

        case Chronicler::ActionBubble:
            m_actionProperties->setBubble(bbl);
            m_actionProperties->show();
        break;

        case Chronicler::ConditionBubble:
            m_conditionProperties->setBubble(bbl);
            m_conditionProperties->show();
        break;

        case Chronicler::CodeBubble:
            m_codeProperties->setBubble(bbl);
            m_codeProperties->show();
            break;

        default:
            break;
        }
    }
    else
    {
        m_storyProperties->setBubble(Q_NULLPTR);
        m_conditionProperties->setBubble(Q_NULLPTR);
        m_choiceProperties->setBubble(Q_NULLPTR);
        m_actionProperties->setBubble(Q_NULLPTR);
        m_codeProperties->setBubble(Q_NULLPTR);
    }
}


//void CPropertiesManager::setFont(const QFont &font)
//{
//    QWidget::setFont(font);
//    m_storyProperties->setFont(font);
//    m_conditionProperties->setFont(font);
//    m_choiceProperties->setFont(font);
//    m_actionProperties->setFont(font);
//}
