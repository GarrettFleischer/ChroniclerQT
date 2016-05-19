#include "filedownloader.h"

FileDownloader::FileDownloader(QUrl file, const char *slot, QObject *parent)
    : QObject(parent)
{
    connect(&m_WebCtrl, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(fileDownloaded(QNetworkReply*)));

    Download(file, slot);
}

FileDownloader::~FileDownloader()
{}

void FileDownloader::Download(QUrl file, const char *slot)
{
    connect(this, SIGNAL(downloaded()), parent(), slot);

    QNetworkRequest request(file);
    m_WebCtrl.get(request);
}

void FileDownloader::fileDownloaded(QNetworkReply* pReply)
{
    m_DownloadedData = pReply->readAll();
    pReply->deleteLater();
    emit downloaded();
}

QByteArray FileDownloader::downloadedData() const
{
    return m_DownloadedData;
}


