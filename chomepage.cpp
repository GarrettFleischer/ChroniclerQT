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


CHomepage::CHomepage(CMainWindow *window, CSettingsView *settings)
    : QWidget((QWidget*)window), m_window(window), m_webView(0), m_recentView(0), m_settings(settings)
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
    QPushButton *btn_new = new QPushButton(QIcon(":/images/icn_new.png"), "");
    btn_new->setIconSize(icon_size);
    btn_new->setToolTip("New Project");
    connect(btn_new, SIGNAL(clicked(bool)),
            this, SLOT(NewProjectClicked()));

    QPushButton *btn_load = new QPushButton(QIcon(":/images/icn_load.png"), "");
    btn_load->setIconSize(icon_size);
    btn_load->setToolTip("Load Project");
    connect(btn_load, SIGNAL(clicked(bool)),
            this, SLOT(LoadProjectCLicked()));

    QPushButton *btn_import = new QPushButton(QIcon(":/images/icn_loadcs.png"), "");
    btn_import->setIconSize(icon_size);
    btn_import->setToolTip("Import Choicescript");
    connect(btn_import, SIGNAL(clicked(bool)),
            this, SLOT(ImportProjectClicked()));

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

void CHomepage::NewProjectClicked()
{
    // TODO popup new project dialog
}

void CHomepage::LoadProjectCLicked()
{
    // TODO popup ".chron" file picker
}

void CHomepage::ImportProjectClicked()
{
    // TODO popup "startup.txt" file picker
}
