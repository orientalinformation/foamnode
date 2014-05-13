/*
-----------------------------------------------------------------------------
This source file is part of the SendMail program

Copyright (c) 2007 Didier Collard <didiercollard@gmail.com>

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.

This file is based on the Qt Mail example, which can be found at:
http://doc.trolltech.com/3.3/mail-example.html
-----------------------------------------------------------------------------
*/

#ifndef SMTP_H
#define SMTP_H

#include <QApplication>
#include <QMessageBox>
#include <QtDebug>
#include <QString>
#include <QObject>
#include <QTcpSocket>
#include <QTextStream>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QLocale>
#include <QList>


class Smtp : public QObject
{
    Q_OBJECT
    
public:
    Smtp( const QString &server, const QString &from, const QString &to, const QList<QString> &bcc,
    const QString &subject,  const QString &body, const bool html = false);
    ~Smtp();
    
    void send();
    
signals:
    void status( const int);

private slots:

 	void stateChanged(QAbstractSocket::SocketState socketState);
    void errorReceived(QAbstractSocket::SocketError socketError);
    void disconnected();
    void connected();
	void readyRead();
private:
	QString message;
	QTextStream *t;
	QTcpSocket *socket;
	QString server;
	QString from;
	QList<QString> rcpt;
	int rcptIndex;
	QString response;
	enum states{Rcpt,Mail,Data,Init,Body,Quit,Close};
	int state;
};
#endif /* SMTP_H */
