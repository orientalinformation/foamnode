#ifndef CLSENCRYPTION_H
#define CLSENCRYPTION_H

#include <QObject>

class clsEncryption : public QObject
{
    Q_OBJECT
public:
    explicit clsEncryption(QObject *parent = 0);
    QString mixString(QString strMac,QString strEmail);
    QString hashMD5(QString strMac, QString strEmail);
    void reserveArray(QByteArray arr);
    bool checkKey(QString fileName);
    bool checkDay(QString fileName);
    //QString strMixString;  
signals:

public slots:

};

#endif // CLSENCRYPTION_H
