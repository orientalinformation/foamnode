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
-----------------------------------------------------------------------------
*/
#ifndef SENDMAIL_H
#define SENDMAIL_H

#include <QtGui/QDialog>
#include <QProgressDialog>
#include <QMessageBox>
#include <QDebug>
#include <QList>
#include <QChar>
#include <QRegExp>
#include <QRegExpValidator>
#include <QInputDialog>
#include "previewdialog.h"
#include "ui_sendmail.h"
#include "downloader.h"

class SendMail : public QDialog
{
    Q_OBJECT

public:
    SendMail(QWidget *parent = 0);
    void SetMode(bool isBuy);
    ~SendMail();

private:
    Ui::SendMailClass ui;
    QProgressDialog* progressDialog;
    Downloader *sendPost;
    bool isBuy;
    QList<QString> createList(QString str);
    bool checkMail;
    bool checkValidForMail();
    bool checkValidForName();
    bool checkValidForFirstName();
    bool checkValidForPhone();
    bool checkValidForFax();

    void AddPostData(QString,QString,QString,QString);
    
private slots:
    void on_pushButton_clicked();
    void on_cancelButton_clicked();
    void on_sendButton_clicked();
    void on_previewButton_clicked();
    void enableButtons(QString value);
    void updateProgress(int state);
    void serviceRequestFinished(QNetworkReply* n);
    void sendData();
    void on_txt_Email_editingFinished();
    void on_txt_Email_textEdited(const QString &arg1);
    void on_txtContactName_textEdited(const QString &arg1);
    void on_txtContactName_editingFinished();
    void on_txt_FirstName_editingFinished();
    void on_txt_FirstName_textEdited(const QString &arg1);
    void on_txt_Phone_editingFinished();
    void on_txt_Phone_textEdited(const QString &arg1);
    void on_txt_Fax_textEdited(const QString &arg1);
    void on_txt_Fax_editingFinished();
    void on_txt_Company_editingFinished();
    void on_txt_Address_editingFinished();
    void on_txt_City_editingFinished();
    void on_txt_Company_textEdited(const QString &arg1);
    void on_txt_Address_textEdited(const QString &arg1);
    void on_txt_City_textEdited(const QString &arg1);
};

#endif // SENDMAIL_H
