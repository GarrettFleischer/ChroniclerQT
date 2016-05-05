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
#include <QAction>

#include "cmainwindow.h"
#include "csettingsview.h"
#include "Misc/qactionbutton.h"


// for QSettings
Q_DECLARE_METATYPE(QStringList)


CHomepage::CHomepage(CMainWindow *window, CSettingsView *settings, QAction *newProjectAction, QAction *openProjectAction, QAction *importProjectAction)
    : QWidget((QWidget*)window), m_window(window), m_webView(0), m_recentView(0), m_settings(settings),
      m_newProjectAction(newProjectAction), m_openProjectAction(openProjectAction), m_importProjectAction(importProjectAction)
{
    QHBoxLayout *main_layout = new QHBoxLayout(this);

    SetupSidebar(main_layout);
    SetupMainWindow(main_layout);

    setLayout(main_layout);
}

void CHomepage::SetupSidebar(QHBoxLayout *main_layout)
{
    m_recentView = new QListWidget();
    QVBoxLayout *layout_recent = new QVBoxLayout();
    QHBoxLayout *layout_buttons = new QHBoxLayout();

    if(m_settings->maxRecentFiles() > 0)
    {
        QStringList def({"C:/Chronicler/Dragon.chron", "C:/Chronicler/Test.chron"}); // fake default projects

        // load the recently opened projects from the settings
        m_recentView->addItems(m_settings->settings()->value("Homepage/RecentFiles", QVariant::fromValue(def)).value<QStringList>());
        connect(m_recentView, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(RecentItemSelected(QListWidgetItem*)));

        layout_recent->addWidget(new QLabel("Recent files"));
        layout_recent->addWidget(m_recentView);
    }

    // Buttons
    const QSize icon_size(32, 32);
    QActionButton *btn_new = new QActionButton(0, m_newProjectAction);
    btn_new->setIconSize(icon_size);

    QActionButton *btn_load = new QActionButton(0, m_openProjectAction);
    btn_load->setIconSize(icon_size);

    QActionButton *btn_import = new QActionButton(0, m_importProjectAction);
    btn_import->setIconSize(icon_size);

    layout_buttons->addWidget(btn_new);
    layout_buttons->addWidget(btn_load);
    layout_buttons->addWidget(btn_import);
    layout_buttons->addStretch(1);
    layout_recent->addLayout(layout_buttons);

    // Add to main layout
    main_layout->addLayout(layout_recent, 1);
}

void CHomepage::SetupMainWindow(QHBoxLayout *main_layout)
{
    m_webView = new QWebView();
    m_webView->load(QUrl("http://www.example.com"));

    // Add to main layout
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

    m_settings->settings()->setValue("Homepage/RecentFiles", QVariant::fromValue(labels));

    // load the selected project
    m_window->LoadProject(item->text());
}
