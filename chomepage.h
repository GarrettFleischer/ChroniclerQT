#ifndef CHOMEPAGE_H
#define CHOMEPAGE_H

#include <QObject>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QWebView;
class QListWidget;
class QStringList;
class QListWidgetItem;
class QSettings;
class QHBoxLayout;
QT_END_NAMESPACE

class CMainWindow;
class CSettingsView;

class CHomepage : public QWidget
{
    Q_OBJECT
public:
    explicit CHomepage(CMainWindow *window, QSettings *settings);

private:
    void SetupSidebar(QHBoxLayout *main_layout);
    void SetupMainWindow(QHBoxLayout *main_layout);

    CMainWindow *m_window;
    QWebView * m_webView;
    QListWidget *m_recentView;

    CSettingsView *m_settings;

signals:

private slots:
    void RecentItemSelected(QListWidgetItem *item);

};

#endif // CHOMEPAGE_H
