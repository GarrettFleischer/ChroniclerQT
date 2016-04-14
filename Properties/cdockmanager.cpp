#include "cdockmanager.h"

#include <QTabWidget>
#include <QVBoxLayout>


#include "cpropertiesview.h"
#include "cprojectview.h"

CDockManager::CDockManager(QStringListModel *model, QWidget *parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);

    m_tabView = new QTabWidget();
    m_tabView->setTabPosition(QTabWidget::South);

    m_properties = new CPropertiesView(model);
    m_project = new CProjectView();

    m_tabView->addTab(m_project, "Project");
    m_tabView->addTab(m_properties, "Properties");

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_tabView);

    setLayout(layout);
}

void CDockManager::setBubble(CBubble *bbl)
{
    m_properties->setBubble(bbl);

    if(bbl)
        m_tabView->setCurrentWidget(m_properties);
    else
        m_tabView->setCurrentWidget(m_project);
}

CPropertiesView &CDockManager::properties()
{
    return *m_properties;
}


QSize CDockManager::sizeHint() const
{
    return QSize(300, 0);
}
