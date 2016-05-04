#include "chomepage.h"

#include <QListWidget>
#include <QListWidgetItem>
#include <QWebView>
#include <QUrl>
#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollBar>
#include <QStringList>
#include <QSettings>
#include <QPushButton>

#include "cmainwindow.h"
#include "csettingsview.h"

Q_DECLARE_METATYPE(QStringList)


CHomepage::CHomepage(CMainWindow *window, QSettings *settings)
    : QWidget((QWidget*)window), m_window(window), m_settings(settings), m_recentView(0), m_webView(0)
{
    QHBoxLayout *main_layout = new QHBoxLayout(this);

    SetupMainWindow(main_layout);
    SetupSidebar(main_layout);

    setLayout(main_layout);
}

void CHomepage::SetupSidebar(QHBoxLayout *main_layout)
{
    m_recentView = new QListWidget();
    QVBoxLayout *recent_layout = new QVBoxLayout();

    if(m_settings->maxRecentFiles() > 0)
    {
        QStringList def({"C:/Chronicler/Dragon.chron", "C:/Chronicler/Test.chron"}); // fake default projects

        // load the recently opened projects from the settings
        m_recentView->addItems(m_settings->settings()->value("Homepage/RecentFiles", QVariant::fromValue(def)).value<QStringList>());
        connect(m_recentView, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(RecentItemSelected(QListWidgetItem*)));

        recent_layout->addWidget(new QLabel("Recent files"));
        recent_layout->addWidget(m_recentView);
    }

    QPushButton *btn_load = new QPushButton();

    main_layout->addLayout(recent_layout, 1);
}

void CHomepage::SetupMainWindow(QHBoxLayout *main_layout)
{
    m_webView = new QWebView();
    m_webView->load(QUrl("http://www.example.com"));

    main_layout->addWidget(m_webView, 4);
}

void CHomepage::RecentItemSelected(QListWidgetItem *item)
{
    // move the selected project to the top of the list
    m_recentView->takeItem(m_recentView->row(item));
    m_recentView->insertItem(0, item);
    item->setSelected(true);

    // save the updated list to settings
    QStringList labels;
    for(int i = 0; i < m_recentView->count() && i < m_settings->maxRecentFiles(); ++i)
        labels << m_recentView->item(i)->text();

    m_settings->setValue("Homepage/RecentFiles", QVariant::fromValue(labels));

    // load the selected project
    m_window->LoadProject(item->text());
}
