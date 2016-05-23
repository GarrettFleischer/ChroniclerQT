#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class FileDownloader : public QObject
{
    Q_OBJECT

public:
    explicit FileDownloader(QUrl file, const char *slot = 0, QObject *parent = 0);
    virtual ~FileDownloader();
    QByteArray downloadedData() const;

    void Download(QUrl file, const char *slot);

private:
    QNetworkAccessManager m_WebCtrl;
    QByteArray m_DownloadedData;

signals:
    void downloaded();

private slots:
    void fileDownloaded(QNetworkReply* pReply);


};

#endif // FILEDOWNLOADER_H
