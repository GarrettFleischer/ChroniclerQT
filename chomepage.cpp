#include "chomepage.h"

#include <QListWidget>
#include <QListWidgetItem>
#include <QWebView>
#include <QUrl>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include "cmainwindow.h"


CHomepage::CHomepage(CMainWindow *window)
    : QWidget((QWidget*)window), m_window(window)
{
    m_webView = new QWebView();
    m_recent = new QListWidget();

    m_webView->load(QUrl("http://www.example.com"));
    m_webView->show();

    m_recent->addItem("C:/Development/Dragon.chron");
    m_recent->horizontalScrollBar()->setValue(m_recent->horizontalScrollBar()->maximum());
    connect(m_recent, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(RecentItemSelected(QListWidgetItem *)));

    QHBoxLayout *main_layout = new QHBoxLayout(this);
    QVBoxLayout *recent_layout = new QVBoxLayout();

    recent_layout->addWidget(new QLabel("Recent files"));
    recent_layout->addWidget(m_recent);

    main_layout->addLayout(recent_layout);
    main_layout->addWidget(m_webView);

    setLayout(main_layout);
}

void CHomepage::RecentItemSelected(QListWidgetItem *item)
{
    m_window->LoadProject(item->text());
}
