#include "cdockmanager.h"

#include <QTabWidget>
#include <QVBoxLayout>

#include "cpropertiesmanager.h"
#include "cprojectview.h"
#include "cvariablesview.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

CDockManager::CDockManager(QWidget *parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);

    m_tabView = new QTabWidget();
    m_tabView->setTabPosition(QTabWidget::South);
    connect(m_tabView, SIGNAL(currentChanged(int)), this, SLOT(TabChanged()));

    m_properties = new CPropertiesManager();
    shared().projectView = new CProjectView();
    shared().variablesView = new CVariablesView();

    m_lastView = shared().projectView;

    m_tabView->addTab(shared().projectView, "Project");
    m_tabView->addTab(shared().variablesView, "Variables");
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
            m_tabView->setCurrentWidget(m_lastView);
    }
}

void CDockManager::TabChanged()
{
    if(m_tabView->currentWidget() != m_properties)
        m_lastView = m_tabView->currentWidget();
}

QSize CDockManager::sizeHint() const
{
    return QSize(350, 0);
}
