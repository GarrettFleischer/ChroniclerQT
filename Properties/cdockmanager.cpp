#include "cdockmanager.h"

#include <QTabWidget>
#include <QVBoxLayout>


#include "cpropertiesmanager.h"
#include "cprojectview.h"

CDockManager::CDockManager(QStringListModel *model, QMenu *editMenu, QWidget *parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);

    m_tabView = new QTabWidget();
    m_tabView->setTabPosition(QTabWidget::South);

    m_properties = new CPropertiesManager(model);
    m_project = new CProjectView(editMenu);

    m_tabView->addTab(m_project, "Project");
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
            m_tabView->setCurrentWidget(m_project);
    }
}

QSize CDockManager::sizeHint() const
{
    return QSize(350, 0);
}
