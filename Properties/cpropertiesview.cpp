#include "cpropertiesview.h"

#include "Bubbles/cstorybubble.h"

CPropertiesView::CPropertiesView(QStringListModel *model, QWidget *parent) : QWidget(parent)
{
    m_storyProperties = new CStoryProperties(NULL, model, parent);
    m_storyProperties->setEnabled(false);

    m_conditionProperties = new CConditionProperties(NULL, model, parent);
    m_conditionProperties->setEnabled(false);
    m_conditionProperties->hide();
    
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    
    
    m_layout = new QVBoxLayout(this);
    m_layout->addWidget(m_storyProperties);
    m_layout->addWidget(m_conditionProperties);
    setLayout(m_layout);

//    QPalette Pal(palette());
//    Pal.setColor(QPalette::Background, Qt::gray);//QColor(86,96,123));
//    setAutoFillBackground(true);
//    setPalette(Pal);

//    QColor bc(Qt::gray);
//    QString style = QString("background-color: rgb(%1, %2, %3)").arg(bc.red()).arg(bc.green()).arg(bc.blue());
//    setAutoFillBackground(true);
//    setStyleSheet(style);

}



void CPropertiesView::SetBubble(CBubble *bbl)
{
    if(bbl)
    {
        m_storyProperties->hide();
        m_conditionProperties->hide();

        switch(bbl->GetType())
        {
        case CBubble::Story:
            m_storyProperties->SetBubble(bbl);
            m_storyProperties->show();
        break;

        case CBubble::Condition:
            m_conditionProperties->SetBubble(bbl);
            m_conditionProperties->show();
        break;

        case CBubble::Choice:
            m_storyProperties->SetBubble(0);
            m_conditionProperties->SetBubble(0);
        break;
        }
    }
    else
    {
        m_storyProperties->SetBubble(0);
        m_conditionProperties->SetBubble(0);
    }
}


void CPropertiesView::setFont(const QFont &font)
{
    QWidget::setFont(font);
    m_storyProperties->setFont(font);
}

QSize CPropertiesView::sizeHint() const
{
    return QSize(400, 720);
}
