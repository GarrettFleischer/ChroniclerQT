#include "cfiledownloader.h"

CFileDownloader::CFileDownloader(QObject *parent)
    : QObject(parent)
{
    connect(&m_WebCtrl, SIGNAL(finished(QNetworkReply*)), this, SLOT(fileDownloaded(QNetworkReply*)));
}

CFileDownloader::~CFileDownloader()
{}

void CFileDownloader::Download(QUrl url, QVariant userData)
{
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::User, userData);

    m_WebCtrl.get(request);
}

void CFileDownloader::fileDownloaded(QNetworkReply* pReply)
{
    QVariant userData = pReply->request().attribute(QNetworkRequest::User);
    QUrl redirect = pReply->attribute(QNetworkRequest::RedirectionTargetAttribute).value<QUrl>();

    if(!redirect.isEmpty())
        Download(redirect, userData);
    else
        emit downloaded(userData, pReply->readAll());

    pReply->deleteLater();
}
