#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QVariant>

class CFileDownloader : public QObject
{
    Q_OBJECT

public:
    explicit CFileDownloader(QObject *parent = Q_NULLPTR);
    virtual ~CFileDownloader();

    void Download(QUrl url, QVariant userData = QVariant());

private:
    QNetworkAccessManager m_WebCtrl;

signals:
    void downloaded(QVariant id, QByteArray data);

private slots:
    void fileDownloaded(QNetworkReply* pReply);

};

#endif // FILEDOWNLOADER_H
