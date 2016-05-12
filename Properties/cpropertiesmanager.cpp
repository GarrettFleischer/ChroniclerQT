#include "cpropertiesmanager.h"

#include <QVBoxLayout>

#include "Widgets/cstoryproperties.h"
#include "Widgets/cconditionproperties.h"
#include "Widgets/cchoiceproperties.h"
#include "Widgets/cactionproperties.h"
#include "Bubbles/cbubble.h"


CPropertiesManager::CPropertiesManager(QStringListModel *model, QWidget *parent)
    : QWidget(parent)
{
    m_storyProperties = new CStoryProperties(model, parent);
    m_storyProperties->setEnabled(false);

    m_conditionProperties = new CConditionProperties(model, parent);
    m_conditionProperties->setEnabled(false);
    m_conditionProperties->hide();

    m_choiceProperties = new CChoiceProperties(model, parent);
    m_choiceProperties->setEnabled(false);
    m_choiceProperties->hide();

    m_actionProperties = new CActionProperties(model, parent);
    m_actionProperties->setEnabled(false);
    m_actionProperties->hide();

    
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_storyProperties);
    layout->addWidget(m_conditionProperties);
    layout->addWidget(m_choiceProperties);
    layout->addWidget(m_actionProperties);
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

        switch(bbl->getType())
        {
        case Chronicler::Story:
            m_storyProperties->setBubble(bbl);
            m_storyProperties->show();
        break;

        case Chronicler::Condition:
            m_conditionProperties->setBubble(bbl);
            m_conditionProperties->show();
        break;

        case Chronicler::Choice:
            m_choiceProperties->setBubble(bbl);
            m_choiceProperties->show();
        break;

        case Chronicler::Action:
            m_actionProperties->setBubble(bbl);
            m_actionProperties->show();
        break;
        }
    }
    else
    {
        m_storyProperties->setBubble(0);
        m_conditionProperties->setBubble(0);
        m_choiceProperties->setBubble(0);
        m_actionProperties->setBubble(0);
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
