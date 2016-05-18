#include "cdockmanager.h"

#include <QTabWidget>
#include <QVBoxLayout>

#include "cpropertiesmanager.h"
#include "cprojectview.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

CDockManager::CDockManager(QWidget *parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);

    m_tabView = new QTabWidget();
    m_tabView->setTabPosition(QTabWidget::South);

    m_properties = new CPropertiesManager();
    shared().projectView = new CProjectView();

    m_tabView->addTab(shared().projectView, "Project");
    m_tabView->addTab(m_properties, "Bubble");

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_tabView);

    setLayout(layout);
}

void CDockManager::setBubble(CBubble *bbl, bool ignore)
{
    m_properties->setBubble(bbl);

    if(!ignore)
    {
        if(bbl)
            m_tabView->setCurrentWidget(m_properties);
        else
            m_tabView->setCurrentWidget(shared().projectView);
    }
}

QSize CDockManager::sizeHint() const
{
    return QSize(350, 0);
}
