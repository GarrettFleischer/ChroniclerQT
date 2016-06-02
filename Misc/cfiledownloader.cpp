#include "cfiledownloader.h"

CFileDownloader::CFileDownloader(QUrl file, const char *slot, QObject *parent)
    : QObject(parent)
{
    connect(&m_WebCtrl, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(fileDownloaded(QNetworkReply*)));

    Download(file, slot);
}

CFileDownloader::~CFileDownloader()
{}

void CFileDownloader::Download(QUrl file, const char *slot)
{
    if(slot)
        connect(this, SIGNAL(downloaded()), parent(), slot);

    QNetworkRequest request(file);
    m_WebCtrl.get(request);
}

void CFileDownloader::fileDownloaded(QNetworkReply* pReply)
{
    QUrl redirect = pReply->attribute(QNetworkRequest::RedirectionTargetAttribute).value<QUrl>();
    if(redirect.toString().length())
        Download(redirect, 0);
    else
    {
        m_DownloadedData = pReply->readAll();
        emit downloaded();
    }

    pReply->deleteLater();
}

QByteArray CFileDownloader::downloadedData() const
{
    return m_DownloadedData;
}


