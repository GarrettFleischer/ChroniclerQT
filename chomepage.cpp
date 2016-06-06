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
#include <QStandardPaths>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>

#include "Properties/cprojectview.h"
#include "csettingsview.h"
#include "Misc/qactionbutton.h"

#include "Misc/cfiledownloader.h"

#include "Misc/chronicler.h"
using Chronicler::shared;

// for QSettings
Q_DECLARE_METATYPE(QStringList)


CHomepage::CHomepage(QWidget *parent)
    : QWidget(parent), m_webView(0), m_recentView(0)
{
    QHBoxLayout *main_layout = new QHBoxLayout(this);

    SetupSidebar(main_layout);
    SetupMainWindow(main_layout);

    setLayout(main_layout);

    QString news = "https://www.dropbox.com/s/tru5nkm9m4uukwe/Chronicler_news.html?dl=1";
    m_downloader = new CFileDownloader(QUrl(news), SLOT(Downloaded()), this);
}

void CHomepage::SetupSidebar(QHBoxLayout *main_layout)
{
    m_recentView = new QListWidget();
    QVBoxLayout *layout_recent = new QVBoxLayout();
    QHBoxLayout *layout_buttons = new QHBoxLayout();

    if(shared().settingsView->maxRecentFiles() > 0)
    {
        // load the recently opened projects from the settings
        m_recentView->addItems(shared().settingsView->settings()->value("Homepage/RecentFiles").value<QStringList>());
        connect(m_recentView, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(RecentItemSelected(QListWidgetItem*)));

        layout_recent->addWidget(new QLabel("Recent files"));
        layout_recent->addWidget(m_recentView);
    }

    // Buttons
    const QSize icon_size(32, 32);
    QActionButton *btn_new = new QActionButton(0, shared().newProjectAction);
    btn_new->setIconSize(icon_size);

    QActionButton *btn_load = new QActionButton(0, shared().openProjectAction);
    btn_load->setIconSize(icon_size);

    QActionButton *btn_import = new QActionButton(0, shared().importProjectAction);
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

    // Add to main layout
    main_layout->addWidget(m_webView, 4);
}

void CHomepage::RecentItemSelected(QListWidgetItem *item)
{
    if(QFileInfo(item->text()).exists())
    {
        // move the selected project to the top of the list
        m_recentView->takeItem(m_recentView->row(item));
        m_recentView->insertItem(0, item);
        item->setSelected(true);

        // save the updated list to settings
        QStringList labels;
        for(int i = 0; i < m_recentView->count() && i < shared().settingsView->maxRecentFiles(); ++i)
            labels << m_recentView->item(i)->text();

        shared().settingsView->settings()->setValue("Homepage/RecentFiles", QVariant::fromValue(labels));

        // load the selected project
        shared().projectView->OpenProject(item->text());
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Selected project file not found.");
        msgBox.setInformativeText("Do you wish to remove this file from recent items?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);

        if(msgBox.exec() == QMessageBox::Yes)
        {
            // remove the non-existant project
            m_recentView->takeItem(m_recentView->row(item));

            // save the updated list to settings
            QStringList labels;
            for(int i = 0; i < m_recentView->count() && i < shared().settingsView->maxRecentFiles(); ++i)
                labels << m_recentView->item(i)->text();

            shared().settingsView->settings()->setValue("Homepage/RecentFiles", QVariant::fromValue(labels));
        }
    }
}

void CHomepage::Downloaded()
{
    QString news = QFileInfo(shared().settingsView->settings()->fileName()).absolutePath() + "/Chronicler_news.html";
    QByteArray ba(m_downloader->downloadedData());

    if(ba.length() > 0)
    {
        QFile file(news);
        file.open(QFile::WriteOnly);
        file.write(ba);
        file.close();
    }

    m_webView->load(QUrl::fromLocalFile(news));
}

