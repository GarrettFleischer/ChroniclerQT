#include "chomepage.h"

#include <QListWidget>
#include <QListWidgetItem>
#include <QWebView>
#include <QUrl>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollBar>
#include <QStringList>
#include <QSettings>

#include "cmainwindow.h"

Q_DECLARE_METATYPE(QStringList)


CHomepage::CHomepage(CMainWindow *window, QSettings *settings)
    : QWidget((QWidget*)window), m_window(window), m_settings(settings)
{
    m_webView = new QWebView();
    m_recentView = new QListWidget();

    m_webView->load(QUrl("http://www.example.com"));

    // load the recently opened projects from the settings
    m_recentView->addItems(m_settings->value("Homepage/RecentFiles").value<QStringList>());
    connect(m_recentView, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(RecentItemSelected(QListWidgetItem*)));

    QHBoxLayout *main_layout = new QHBoxLayout(this);
    QVBoxLayout *recent_layout = new QVBoxLayout();

    recent_layout->addWidget(new QLabel("Recent files"));
    recent_layout->addWidget(m_recentView);

    main_layout->addLayout(recent_layout, 1);
    main_layout->addWidget(m_webView, 4);

    setLayout(main_layout);
}

void CHomepage::RecentItemSelected(QListWidgetItem *item)
{
    // set the selected project to the top of the list
    m_recentView->takeItem(m_recentView->row(item));
    m_recentView->insertItem(0, item);
    item->setSelected(true);

    // save the updated list to settings
    QStringList labels;
    for(int i = 0; i < m_recentView->count(); ++i)
        labels << m_recentView->item(i)->text();

    m_settings->setValue("Homepage/RecentFiles", QVariant::fromValue(labels));

    // load the selected project
    m_window->LoadProject(item->text());
}
