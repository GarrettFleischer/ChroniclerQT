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
#include <QFileInfo>
#include <QMessageBox>
#include <QStatusBar>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QProcess>
#include <QDir>

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "cmainwindow.h"

#include "Properties/cprojectview.h"
#include "csettingsview.h"
#include "Misc/qactionbutton.h"

#include "Misc/cfiledownloader.h"

#include "Misc/chronicler.h"
using Chronicler::shared;
using Chronicler::CVersion;

#include <QDebug>

// for QSettings
Q_DECLARE_METATYPE(QStringList)
Q_DECLARE_METATYPE(UpdateFile)


CHomepage::CHomepage(QWidget *parent)
    : QWidget(parent), m_webView(Q_NULLPTR), m_recentView(Q_NULLPTR), m_filesDownloaded(0)
{
    // Set variables relevant to each OS
#ifdef Q_OS_WIN
    m_os = Windows;
    m_executable = "/Chronicler-Next.exe";
#endif
#ifdef Q_OS_OSX
    m_os = Mac;
    m_executable = "/Chronicler-Next.app";
#endif
#ifdef Q_OS_LINUX
    m_os = Linux;
    m_executable = "/Chronicler-Next";
#endif

    // Build the layout
    QHBoxLayout *main_layout = new QHBoxLayout(this);
    SetupSidebar(main_layout);
    SetupMainWindow(main_layout);

    // Setup the downloader
    m_settingsPath = QFileInfo(shared().settingsView->settings()->fileName()).absolutePath();

    m_downloader = new CFileDownloader(this);
    connect(m_downloader, SIGNAL(downloaded(QVariant,QByteArray)), this, SLOT(FileDownloaded(QVariant,QByteArray)));

    shared().statusBar->showMessage("Checking for updates and acquiring the latest news...");
    m_downloader->Download(QUrl("https://www.dropbox.com/s/tru5nkm9m4uukwe/Chronicler_news.html?dl=1"), (int)News);
    m_downloader->Download(QUrl("https://www.dropbox.com/s/vozka09alf1xtnh/Chronicler_Versions.json?dl=1"), (int)Versions);
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
    QString news = QFileInfo(shared().settingsView->settings()->fileName()).absolutePath() + "/Chronicler_news.html";
    m_webView = new QWebView();
    m_webView->load(QUrl::fromLocalFile(news));
    m_webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(m_webView, SIGNAL(linkClicked(QUrl)), this, SLOT(LinkClicked(QUrl)));

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

void CHomepage::FileDownloaded(QVariant userData, QByteArray data)
{
    if(data.length() > 0)
    {
        if(userData.type() == QVariant::Int)
        {
            if(userData.toInt() == News)
                NewsDownloaded(data);
            else
                CheckForUpdates(data);
        }
        else
        {
            UpdateFile uf = userData.value<UpdateFile>();
            QString basePath = ((uf.path == m_executable) ? m_settingsPath : QCoreApplication::applicationDirPath());

            WriteToFile(basePath + uf.path, data, uf.permissions);

            if(++m_filesDownloaded >= m_updateVersion.files.length())
                FinishInstallingUpdates();
        }
    }
    else
        shared().statusBar->showMessage("An error occured while downloading the file...");
}

void CHomepage::NewsDownloaded(const QByteArray &data)
{
    QString news = QFileInfo(shared().settingsView->settings()->fileName()).absolutePath() + "/Chronicler_news.html";
    WriteToFile(news, data, QFileDevice::ReadUser | QFileDevice::WriteUser);

    m_webView->load(QUrl::fromLocalFile(news));
}

void CHomepage::CheckForUpdates(const QByteArray &data)
{
    // Parse the returned JSON data into UpdateVersion's
    QList<UpdateVersion> updateVersions;

    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonArray array = document.object()["versions"].toArray();

    for(const QJsonValue &jsonVersion : array)
    {
        QString osName = ((m_os == Windows) ? "windows" : ((m_os == Linux) ? "linux" : "mac"));

        UpdateVersion uv;
        QJsonObject oVersion = jsonVersion.toObject();

        uv.version = oVersion["version"].toString();
        for(const QJsonValue &jsonFile : oVersion[osName].toArray())
        {
            UpdateFile uf;
            QJsonObject oFile = jsonFile.toObject();

            uf.path = oFile["path"].toString();
            uf.url = oFile["url"].toString();
            for(QChar c : oFile["permissions"].toString())
            {
                if(c == 'r')
                    uf.permissions |= QFileDevice::ReadUser;
                if(c == 'w')
                    uf.permissions |= QFileDevice::WriteUser;
                if(c == 'e')
                    uf.permissions |= QFileDevice::ExeUser;
            }

            uv.files.append(uf);
        }

        updateVersions.append(uv);
    }

    // Use the parsed data
    for(UpdateVersion uv : updateVersions)
    {
        // Find the first available update
        if(shared().ProgramVersion < uv.version)
        {
            // Ask if the user wishes to download the update
            QMessageBox msgBox;
            msgBox.setText(tr("Version ") + uv.version + tr(" available.\n"));
            msgBox.setInformativeText("Download now?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Yes);
            int ret = msgBox.exec();

            if(ret == QMessageBox::Yes)
            {
                m_updateVersion = uv;
                shared().statusBar->showMessage("Downloading update...");
                for(UpdateFile uf : uv.files)
                    m_downloader->Download(QUrl(uf.url), QVariant::fromValue(uf));
            }

            // only install this update. Chronicler must restart to install more updates.
            break;
        }
        else
            shared().statusBar->showMessage("Chronicler is up to date.");
    }
}

void CHomepage::FinishInstallingUpdates()
{
    shared().statusBar->showMessage("Download complete.");

    QMessageBox msgBox;
    msgBox.setText(tr("Update has been downloaded. Chronicler must be restarted in order to apply the update."));
    msgBox.setInformativeText(tr("Restart now?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();

    if(ret == QMessageBox::Yes)
    {
        if(m_os == Windows)
        {
            shared().mainWindow->close();
            QDesktopServices::openUrl(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/Chronicler_Updater.exe"));
        }
        else if(m_os == Linux)
        {
            QProcess *process = new QProcess();
            connect(process, SIGNAL(finished(int)), shared().mainWindow, SLOT(close()));
            process->start(QCoreApplication::applicationDirPath() + "/Chronicler_Updater");
        }
        else if(m_os == Mac)
        {
            QProcess *process = new QProcess();
            connect(process, SIGNAL(finished(int)), shared().mainWindow, SLOT(close()));
            process->start(QCoreApplication::applicationDirPath() + "/Chronicler_Updater.app");
        }
    }
}

bool CHomepage::WriteToFile(QString path, const QByteArray &data, QFlags<QFileDevice::Permission> permissions)
{
    QFile file(path);
    file.open(QFile::WriteOnly);
    if(!file.isOpen())
        return false;

    file.setPermissions(permissions);
    file.write(data);
    file.close();

    return true;
}

void CHomepage::LinkClicked(const QUrl &url)
{
    QDesktopServices::openUrl(url);
}

