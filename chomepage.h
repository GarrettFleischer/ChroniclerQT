#ifndef CHOMEPAGE_H
#define CHOMEPAGE_H

#include <QObject>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QWebView;
class QListWidget;
QT_END_NAMESPACE

class MainWindow;

class CHomepage : public QWidget
{
    Q_OBJECT
public:
    explicit CHomepage(MainWindow *window);

private:
    MainWindow *m_window;
    QWebView * m_webView;
    QListWidget *m_recent;

signals:

private slots:
    void RecentItemSelected(QListWidgetItem *item);

};

#endif // CHOMEPAGE_H
