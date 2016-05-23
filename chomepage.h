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
class FileDownloader;
class QDropboxFile;

class CHomepage : public QWidget
{
    Q_OBJECT

public:
    explicit CHomepage(QWidget *parent = 0);

private:
    void SetupSidebar(QHBoxLayout *main_layout);
    void SetupMainWindow(QHBoxLayout *main_layout);

    QWebView * m_webView;
    QListWidget *m_recentView;

    FileDownloader *m_downloader;

    QDropboxFile *m_dbfile;

signals:

private slots:
    void RecentItemSelected(QListWidgetItem *item);
    void Downloaded();
};

#endif // CHOMEPAGE_H
