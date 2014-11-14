#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QHash>
#include <QtNetwork/QtNetwork>
//#include <QNetworkAccessManager>
//#include <QNetworkProxy>
//#include <QNetworkReply>
//#include <QNetworkRequest>
//#include <QAuthenticator>
#include "authdialog.h"

class Downloader : public QObject
{
    Q_OBJECT
public:
    static const int READY = 0;
    static const int DOWNLOADING = 1;
    static const int FAILED = 3;
    static const int SUCCESS = 4;
    static const int CANCELLED = 5;

    static const int GET_REQUEST = 0;
    static const int POST_REQUEST = 1;

    explicit Downloader(QObject *parent = 0);
    Downloader(QString url, int method = Downloader::GET_REQUEST, QObject *parent = 0);

    void setUrl(QString url);
    QString *url();
    void start();
    void cancel();
    void reset();
    void addPostParameter(QString name,QString value);
    int status();

    QNetworkReply *getNetworkReply();
    QNetworkRequest *getNetworkRequest();

    void setProxyAuthentication(QString username, QString password);

signals:
    void saveProxyAuthentication(QString, QString);

private slots:
    void proxyAuthenticate(QNetworkProxy,QAuthenticator * authenticator);
    void updateProgress(qint64 bytesReceived,qint64 bytesTotal);
    void slotFinished();
    void slotErrors(QNetworkReply::NetworkError);

private:
    int _status;
    int _method;
    QString *_url;
    QHash<QString,QString> _postData;
    QNetworkAccessManager *networkManager;
    QNetworkReply *_reply;
    QNetworkRequest *_request;
    QString proxyUsername;
    QString proxyPassword;

signals:
    void progressUpdate(qint64 bytesReceived,qint64 bytesTotal);
    void downloadSuccess();
    void downloadFailed();
    void downloadStopped();
};

#endif // DOWNLOADER_H
