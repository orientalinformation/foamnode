#include <QNetworkInterface>
#include <QFile>
#include <QtCore>
#include <qtextstream.h>
#include <QCryptographicHash>
#include "clsencryption.h"
#include "sendmail.h"
#include "smtp.h"
#include "registerdialog.h"
#include <QDesktopServices>
#include <QNetworkProxyFactory>
#include <QtNetwork>
SendMail::SendMail(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);       
        progressDialog = new QProgressDialog ("Sending mail...", "Abort", 0, 7, this);
        connect (progressDialog, SIGNAL(canceled()), this, SLOT(close()));
        progressDialog->hide();
        connect (ui.txtContactName, SIGNAL(textChanged(QString)), this, SLOT(enableButtons(QString)));
        connect (ui.txt_FirstName, SIGNAL(textChanged(QString)), this, SLOT(enableButtons(QString)));
        connect (ui.txt_Address, SIGNAL(textChanged(QString)), this, SLOT(enableButtons(QString)));
        connect (ui.txt_Company, SIGNAL(textChanged(QString)), this, SLOT(enableButtons(QString)));
        connect (ui.txt_Phone, SIGNAL(textChanged(QString)), this, SLOT(enableButtons(QString)));
        connect (ui.txt_Email, SIGNAL(textChanged(QString)), this, SLOT(enableButtons(QString)));
        connect (ui.txt_City, SIGNAL(textChanged(QString)), this, SLOT(enableButtons(QString)));
        connect(ui.txt_Fax,SIGNAL(textChanged(QString)),this,SLOT(enableButtons(QString)));
        ui.frame_Web->hide();
        this->setMaximumSize(260,260);
}

SendMail::~SendMail()
{

}
void SendMail::SetMode(bool isBuy)
{
    this->isBuy = isBuy;
}

void SendMail::on_sendButton_clicked()
{
        this->sendData();
}

void SendMail::on_previewButton_clicked()
{
    QString company=ui.txt_Company->text();
    QString address=ui.txt_Address->text();
    QString phone=ui.txt_Phone->text();
    QString name= ui.txt_FirstName->text() + " " + ui.txtContactName->text();
    QString message = ui.txt_Messages->toPlainText();

    QString fax = ui.txt_Fax->text();
    QString country = ui.txt_Country->text();
    QString zipcode = ui.txt_ZipCode->text();
    QString temp,mac;
    foreach (const QNetworkInterface &ni, QNetworkInterface::allInterfaces())
    {
        temp = ni.hardwareAddress();
        temp=temp.replace(":","");
        if(!temp.contains("0000"))
        {
            mac = temp;
            break;
        }
    }    
    QString contain="Name: " + name +
                    ";<br />Company: " + company +
                    ";<br />Address: " + address +
                    ";<br />Phone: " + phone;
    if(fax != "")
        contain = contain + ";<br />Fax: " + fax;
    if(zipcode != "")
        contain = contain + ";<br />Zip code: " + zipcode;
    if(country != "")
        contain = contain + ";<br />Country: " + country;
    if(ui.checkBox_AskLicence->isChecked())
        contain = contain + ";<br />Mac Address: " + mac;
    if(message != "")
        contain = contain + ";<br />Messages: " + message;
    previewDialog dialog(contain);
    dialog.exec();
}
void SendMail::enableButtons(QString value)
{
    if ((ui.txtContactName->text() != QString("")) &&
        (ui.txt_Address->text() != QString("")) &&
        (ui.txt_Company->text() != QString("")) &&
        (ui.txt_Email->text() != QString("")) &&
        (ui.txt_FirstName->text() != QString("")) &&
        (ui.txt_City->text() != QString("")) &&
        (ui.txt_Phone->text() != QString("")))
    {
        checkValidForMail();
        checkValidForName();
        checkValidForFirstName();
        checkValidForPhone();
        if(checkValidForMail() == true &&
           checkValidForName() == true &&
           checkValidForFirstName() == true &&
           checkValidForPhone() == true){
            if(ui.txt_Fax->text() != ""){
                checkValidForFax();
                if(checkValidForFax() == true) {
                    ui.sendButton->setEnabled(true);
                    ui.previewButton->setEnabled(true);
                }else {
                    ui.sendButton->setEnabled(false);
                    ui.previewButton->setEnabled(false);
                }
            }else {
                ui.sendButton->setEnabled(true);
                ui.previewButton->setEnabled(true);
            }
        }
    }
    else
    {
        ui.sendButton->setEnabled(false);
        ui.previewButton->setEnabled(false);
    }
}
void SendMail::updateProgress(int state)
{

	qDebug() << state;
	if (state == -1)
	{
        QMessageBox::critical(this,tr("Error while sending mail"),tr("Error while sending mail."));
		progressDialog->hide();
	}
	progressDialog->setValue(state);
}

QList<QString> SendMail::createList(QString str)
{
	QList<QString> list;
	enum states {seek, read, store,done};
	int state;
	int i = 0;
	QString item = "";
	QChar c;
	
	//Add an extra ; so it will always include the last email adresst oo
	str.append(';');
	QRegExp regExp("^[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}$", Qt::CaseInsensitive);
	QRegExpValidator validator(regExp, this);
	if(str == "")
		return list;
	
	state = seek;
	while(state!=done)
	{
		c = str[i];
		if (state==seek)
		{
			if(c == ' ' || c == ';')
			{
				//Keep seeking
				state=seek;
			}
			else
			{
				//Found item
				state=read;
			}
		}
		if (state==read)
		{
			//Check for end of this item
			if(c == ';')
			{
				//End so store it
				state=store;
			}
			else
			{
				//Read
				item += c;
			}
		}
		if  (state==store)
		{
			//Store item
			//Strip /n
			item.replace( QString::fromLatin1( "\n" ), QString::fromLatin1( "" ) );
			//Validate
			int pos = 0;
			if(validator.validate(item, pos) != QValidator::Acceptable)
			{
				bool ok;
				item = QInputDialog::getText(this, tr("Wrong email-adress"),
						tr("The email adress you provided is wrong. Please enter a new one:"),
						QLineEdit::Normal,
						item,
						&ok);
				if (ok)
				{
					//Store again
					state=store;
					continue;
				}
			}
			else
			{
				list.append(item);
			}
			item.clear();
			state=seek;
		}
		//Increase I, check if we're done
		if (i >= str.length())
			state=done;
		else
			i++;
		
	}
	
    return list;
}

bool SendMail::checkValidForMail()
{
    QRegExp mailREX("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b");
    mailREX.setCaseSensitivity(Qt::CaseInsensitive);
    mailREX.setPatternSyntax(QRegExp::RegExp);
    if(mailREX.exactMatch(ui.txt_Email->text()) == false) {
        //ui.txt_Email->undo();
        //ui.txt_Email->setPlaceholderText("Format email false!");
        return false;
    }else{
        //ui.txt_Email->setStyleSheet("");
        //ui.txt_Email->setPlaceholderText("");
        //checkMail=true;
        return true;
    }
}

bool SendMail::checkValidForName()
{
    bool icheck;
    QString temp = ui.txtContactName->text();
    for(int i = 0; i< temp.length();i++) {
        QString check = temp.at(i);
        check.toInt(&icheck);
        if(icheck){
            //ui.txtContactName->undo();
            //ui.txtContactName->setPlaceholderText("No number.");
            return false;
            break;
        }
    }
    return true;
}

bool SendMail::checkValidForFirstName()
{
    bool icheck;
    QString temp = ui.txt_FirstName->text();
    for(int i = 0; i< temp.length();i++) {
        QString check = temp.at(i);
        check.toInt(&icheck);
        if(icheck){
            //ui.txt_FirstName->undo();
            //ui.txt_FirstName->setPlaceholderText("No number.");
            return false;
            break;
        }
    }
    return true;
}

bool SendMail::checkValidForPhone()
{
    bool icheck;
    QString temp = ui.txt_Phone->text();
    for(int i = 0; i< temp.length();i++) {
        QString check = temp.at(i);
        check.toInt(&icheck);
        if(!icheck){
            //ui.txt_Phone->undo();
            //ui.txt_Phone->setPlaceholderText("(0-9)");
            return false;
            break;
        }
    }
    return true;
}

bool SendMail::checkValidForFax()
{
    bool icheck;
    QString temp = ui.txt_Fax->text();
    for(int i = 0; i< temp.length();i++) {
        QString check = temp.at(i);
        check.toInt(&icheck);
        if(!icheck){
            //ui.txt_FirstName->undo();
            //ui.txt_FirstName->setPlaceholderText("(0-9)");
            return false;
            break;
        }
    }
    return true;
}

void SendMail::AddPostData(QString mac_vl,QString key_vl,QString code_postal_vl, QString isTrial_vl)
{
    sendPost->addPostParameter("company",ui.txt_Company->text());
    sendPost->addPostParameter("address",ui.txt_Address->text());
    sendPost->addPostParameter("phone",ui.txt_Phone->text());
    sendPost->addPostParameter("firstname",ui.txt_FirstName->text());
    sendPost->addPostParameter("lastname",ui.txtContactName->text());
    sendPost->addPostParameter("email",ui.txt_Email->text());
    sendPost->addPostParameter("message",ui.txt_Messages->toPlainText());
    sendPost->addPostParameter("fax",ui.txt_Fax->text());
    sendPost->addPostParameter("city",ui.txt_City->text());
    sendPost->addPostParameter("country",ui.txt_Country->text());
    sendPost->addPostParameter("zipcode",ui.txt_ZipCode->text());
    sendPost->addPostParameter("mac",mac_vl);
    sendPost->addPostParameter("key",key_vl);
    sendPost->addPostParameter("code_postal",code_postal_vl);
    sendPost->addPostParameter("isTrial",isTrial_vl);
}

void SendMail::on_cancelButton_clicked()
{
    this->close();
}

void SendMail::on_pushButton_clicked()
{
    this->close();
}
void SendMail::sendData()
{
    QString mac;
    QString key="key";
    QString code_postal="code";
    QString isTrial = "1";
    QNetworkProxyFactory::setUseSystemConfiguration(true);
    foreach (const QNetworkInterface &ni, QNetworkInterface::allInterfaces())
    {
        QString temp = ni.hardwareAddress();
        temp=temp.replace(":","");
        if(!temp.contains("0000") && temp != "")
        {
            mac = temp;
            break;
        }
    }
    if(isBuy)
        isTrial = "0";
    sendPost = new Downloader("http://client.dofilink.com/bugreport/add",Downloader::POST_REQUEST);
    AddPostData(mac,key,code_postal,isTrial);
    sendPost->start();
    while (sendPost->status()==Downloader::DOWNLOADING) {
        QApplication::processEvents();
    }
    if (sendPost->status() == Downloader::SUCCESS) {
        ui.frame_Info->hide();
        ui.frame_Button->hide();
        ui.frame_Web->show();
        QString Reply(sendPost->getNetworkReply()->readAll());
        ui.ReplyBrowser->setStyleSheet("text-align:center");
        ui.ReplyBrowser->setText("<p align=\"center\" style=\"top:100px \"><b>Register successfully!</b></p>");
        //ui.ReplyBrowser->setText(Reply);
        this->setMinimumSize(260,260);
        this->resize(260,260);
        qDebug()<<"Successful!";
    } else {
        ui.frame_Info->hide();
        ui.frame_Button->hide();
        ui.frame_Web->show();
        ui.ReplyBrowser->setText("Please check internet connection!");
        this->setMinimumSize(260,260);
        this->resize(260,260);
        qDebug()<<"Error!. Check internet connection !";
    }
}

void SendMail::serviceRequestFinished(QNetworkReply* n)
{
    if(n->error() == 0)
    {
        QMessageBox::about(this,tr("Success"),tr("Send data completed..!"));
    }
    else
    {
        QMessageBox::about(this,tr("Error"),QString::number(n->error()) + ": " + n->errorString());
    }
    this->close();
}

void SendMail::on_txt_Email_editingFinished()
{
    if(checkValidForMail() == false) {
        ui.txt_Email->setStyleSheet("border:2px solid red;border-radius:5px;");
        ui.sendButton->setEnabled(false);
        ui.previewButton->setEnabled(false);
    }else {
        ui.txt_Email->setStyleSheet("");
    }

}

void SendMail::on_txt_Email_textEdited(const QString &arg1)
{
    ui.txt_Email->setStyleSheet("");
    ui.sendButton->setEnabled(false);
    ui.previewButton->setEnabled(false);
}

void SendMail::on_txtContactName_textEdited(const QString &arg1)
{
    ui.txtContactName->setStyleSheet("");
    ui.sendButton->setEnabled(false);
    ui.previewButton->setEnabled(false);
}

void SendMail::on_txtContactName_editingFinished()
{
    if(checkValidForName() == false || ui.txtContactName->text() == "") {
        ui.txtContactName->setStyleSheet("border:2px solid red;border-radius:5px;");
        ui.sendButton->setEnabled(false);
        ui.previewButton->setEnabled(false);
    }
    else {
       ui.txtContactName->setStyleSheet("");
    }
}

void SendMail::on_txt_FirstName_editingFinished()
{
    if(checkValidForFirstName() == false || ui.txt_FirstName->text() == "") {
        ui.txt_FirstName->setStyleSheet("border:2px solid red;border-radius:5px;");
        ui.sendButton->setEnabled(false);
        ui.previewButton->setEnabled(false);
    }
    else {
       ui.txt_FirstName->setStyleSheet("");
    }
}

void SendMail::on_txt_FirstName_textEdited(const QString &arg1)
{
    ui.txt_FirstName->setStyleSheet("");
    ui.sendButton->setEnabled(false);
    ui.previewButton->setEnabled(false);
}

void SendMail::on_txt_Phone_editingFinished()
{
    if(checkValidForPhone() == false || ui.txt_Phone->text() == "") {
        ui.txt_Phone->setStyleSheet("border:2px solid red;border-radius:5px;");
        ui.sendButton->setEnabled(false);
        ui.previewButton->setEnabled(false);
    }
    else {
       ui.txt_Phone->setStyleSheet("");
    }
}

void SendMail::on_txt_Phone_textEdited(const QString &arg1)
{
    ui.txt_Phone->setStyleSheet("");
    ui.sendButton->setEnabled(false);
    ui.previewButton->setEnabled(false);
}

void SendMail::on_txt_Fax_textEdited(const QString &arg1)
{
    if(ui.txt_Fax->text() != "") {
        ui.txt_Fax->setStyleSheet("");
        ui.sendButton->setEnabled(false);
        ui.previewButton->setEnabled(false);
    }else {
        ui.txt_Fax->setStyleSheet("");
    }
}

void SendMail::on_txt_Fax_editingFinished()
{
    if(ui.txt_Fax->text() != "") {
        if(checkValidForFax() == false) {
            ui.txt_Fax->setStyleSheet("border:2px solid red;border-radius:5px;");
            ui.sendButton->setEnabled(false);
            ui.previewButton->setEnabled(false);
        }
        else {
           ui.txt_Fax->setStyleSheet("");
        }
    }else {
        ui.txt_Fax->setStyleSheet("");
    }
}

void SendMail::on_txt_Company_editingFinished()
{
    if(ui.txt_Company->text() == ""){
        ui.txt_Company->setStyleSheet("border:2px solid red;border-radius:5px;");
    }
}

void SendMail::on_txt_Address_editingFinished()
{
    if(ui.txt_Address->text() == ""){
        ui.txt_Address->setStyleSheet("border:2px solid red;border-radius:5px;");
    }
}

void SendMail::on_txt_City_editingFinished()
{
    if(ui.txt_City->text() == ""){
        ui.txt_City->setStyleSheet("border:2px solid red;border-radius:5px;");
    }
}

void SendMail::on_txt_Company_textEdited(const QString &arg1)
{
    ui.txt_Company->setStyleSheet("");
}

void SendMail::on_txt_Address_textEdited(const QString &arg1)
{
    ui.txt_Address->setStyleSheet("");
}

void SendMail::on_txt_City_textEdited(const QString &arg1)
{
    ui.txt_City->setStyleSheet("");
}
