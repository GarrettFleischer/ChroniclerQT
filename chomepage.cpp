#include "chomepage.h"

#include <QListWidget>
#include <QWebView>
#include <QUrl>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>


CHomepage::CHomepage(MainWindow *window)
    : QWidget(window)
{
    m_webView = new QWebView();
    m_recent = new QListWidget();

    m_webView->load(QUrl("http://www.example.com"));
    m_webView->show();

    m_recent->addItem("C:/Development/Dragon.chron");
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

}
