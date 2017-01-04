#ifndef CHOMEPAGE_H
#define CHOMEPAGE_H

#include <QWidget>
#include <QFile>
#include <QVariant>

QT_BEGIN_NAMESPACE
class QWebView;
class QListWidget;
class QStringList;
class QListWidgetItem;
class QSettings;
class QHBoxLayout;
class QAction;
QT_END_NAMESPACE

class CMainWindow;
class CSettingsView;
class CFileDownloader;

// for handling updates
struct UpdateFile {
    QString path;
    QString url;
    QFlags<QFileDevice::Permission> permissions = 0;
};

struct UpdateVersion {
    QString version;
    QList<UpdateFile> files;
};

class CHomepage : public QWidget
{
    Q_OBJECT

public:
    explicit CHomepage(QWidget *parent = 0);

private:
    enum OS { Windows, Mac, Linux };
    enum DownloadTypes { News, Versions };

    void SetupSidebar(QHBoxLayout *main_layout);
    void SetupMainWindow(QHBoxLayout *main_layout);

    void NewsDownloaded(const QByteArray &data);
    void CheckForUpdates(const QByteArray &data);
    void FinishInstallingUpdates();

    bool WriteToFile(QString path, const QByteArray &data, QFlags<QFileDevice::Permission> permissions);

    QWebView * m_webView;
    QListWidget *m_recentView;

    CFileDownloader *m_downloader;

    QString m_executable;
    OS m_os;

    QString m_settingsPath;

    UpdateVersion m_updateVersion;
    int m_filesDownloaded;

signals:

private slots:
    void RecentItemSelected(QListWidgetItem *item);

    void FileDownloaded(QVariant userData, QByteArray data);

    void LinkClicked(const QUrl &url);
};

#endif // CHOMEPAGE_H
