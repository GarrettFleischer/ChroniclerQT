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
class QAction;
QT_END_NAMESPACE

class CMainWindow;
class CSettingsView;

class CHomepage : public QWidget
{
    Q_OBJECT
public:
    explicit CHomepage(CMainWindow *window, CSettingsView *settings,
                       QAction *newProjectAction, QAction *openProjectAction, QAction *importProjectAction);

private:
    void SetupSidebar(QHBoxLayout *main_layout);
    void SetupMainWindow(QHBoxLayout *main_layout);

    CMainWindow *m_window;
    QWebView * m_webView;
    QListWidget *m_recentView;

    CSettingsView *m_settings;

    QAction *m_newProjectAction;
    QAction *m_openProjectAction;
    QAction *m_importProjectAction;

signals:

private slots:
    void RecentItemSelected(QListWidgetItem *item);
};

#endif // CHOMEPAGE_H
