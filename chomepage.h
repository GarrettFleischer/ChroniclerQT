#ifndef CHOMEPAGE_H
#define CHOMEPAGE_H

#include <QWidget>

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

class CHomepage : public QWidget
{
    Q_OBJECT

public:
    explicit CHomepage(QWidget *parent = 0);

private:
    enum OS { Windows, Mac, Linux };

    void SetupSidebar(QHBoxLayout *main_layout);
    void SetupMainWindow(QHBoxLayout *main_layout);

    QWebView * m_webView;
    QListWidget *m_recentView;

    CFileDownloader *m_downloader;

    QString m_download;
    QString m_executable;
    OS m_os;

signals:

private slots:
    void RecentItemSelected(QListWidgetItem *item);
    void NewsDownloaded();
    void CheckForUpdates();
    void UpdateDownloaded();
};

#endif // CHOMEPAGE_H
