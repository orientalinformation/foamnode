#include "downloader.h"

Downloader::Downloader(QObject *parent) :
    QObject(parent)
{

}

Downloader::Downloader(QString url, int method, QObject *parent):
    QObject(parent)
{
    this->_status = Downloader::READY;

    this->_url = &url;
    this->_method = method;
    this->_request = new QNetworkRequest( *(this->_url) );
    this->_reply = 0;

    this->networkManager = new QNetworkAccessManager(this);
    this->networkManager->proxyFactory()->setUseSystemConfiguration(true);

    connect(this->networkManager,SIGNAL(proxyAuthenticationRequired(QNetworkProxy,QAuthenticator*)),this,SLOT(proxyAuthenticate(QNetworkProxy,QAuthenticator*)));
}

void Downloader::proxyAuthenticate(QNetworkProxy, QAuthenticator *authenticator)
{
    if (this->proxyUsername.length() > 0) {
        authenticator->setUser(this->proxyUsername);
        authenticator->setPassword(this->proxyPassword);
        this->proxyUsername = "";
    } else {
        AuthDialog dlg;
        if (dlg.exec() == QDialog::Accepted) {
            authenticator->setUser(dlg.username());
            authenticator->setPassword(dlg.password());
            if (dlg.saveChecked()) {
                emit saveProxyAuthentication(dlg.username(), dlg.password());
            }
        } else {
            this->cancel();
        }
    }

}

void Downloader::cancel()
{
    this->_status = Downloader::CANCELLED;
    this->_reply->abort();
}

void Downloader::start()
{
    this->_status = Downloader::DOWNLOADING;
    if (this->_method == Downloader::POST_REQUEST) {
        int dataLength = this->_postData.count ();
        QString strPostData;
        QList<QString> keys = this->_postData.keys();
        for (int i = 0; i < dataLength; ++i) {
            strPostData.append(keys[i]);
            strPostData.append('=');
            strPostData.append(this->_postData[keys[i]]);
            strPostData.append('&');
        }
        strPostData.chop(1);
        //qDebug () << strPostData;
        this->_request->setHeader (QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        this->_reply = this->networkManager->post( *(this->_request), strPostData.toUtf8() );
    } else {
        this->_reply = this->networkManager->get( *(this->_request) );
    }

    connect(this->_reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateProgress(qint64,qint64)));
    connect(this->_reply, SIGNAL(finished()), this, SLOT(slotFinished()));
    connect(this->_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotErrors(QNetworkReply::NetworkError)));
}

void Downloader::reset()
{
    this->_status = Downloader::READY;
}

void Downloader::addPostParameter(QString name, QString value)
{
    this->_postData[name]=value;
}

void Downloader::setUrl(QString url)
{
    this->_url = new QString(url);
}

void Downloader::updateProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    emit this->progressUpdate(bytesReceived, bytesTotal);
}

void Downloader::slotFinished()
{
    qDebug() << "bytesAvailable: " << this->_reply->bytesAvailable();
    qDebug() << "error: " << this->_reply->error();
    if (this->_reply->error()==0 && this->_reply->bytesAvailable() == 0) {
        QVariant redirectionTarget = this->_reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
        //qDebug() << redirectionTarget.toString();
        this->_url = new QString(redirectionTarget.toString());
        this->_request->setUrl( redirectionTarget.toUrl() );
        this->start();
    } else {
        emit this->downloadStopped();
        if (this->_status != Downloader::FAILED) {
            this->_status = Downloader::SUCCESS;
            emit this->downloadSuccess();
        } else {
            if (this->_reply->error()!=5) {
                this->_status = Downloader::FAILED;
                emit this->downloadFailed();
            } else {
                this->_status = Downloader::CANCELLED;
            }
        }
    }
}

void Downloader::slotErrors(QNetworkReply::NetworkError)
{
    this->_status = Downloader::FAILED;
}

QString *Downloader::url()
{
    return new QString( *(this->_url) );
}

QNetworkReply * Downloader::getNetworkReply()
{
    return this->_reply;
}

int Downloader::status()
{
    return this->_status;
}

QNetworkRequest * Downloader::getNetworkRequest()
{
    return this->_request;
}

void Downloader::setProxyAuthentication(QString username, QString password)
{
    this->proxyUsername = username;
    this->proxyPassword = password;
}

