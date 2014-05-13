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
#include <QWebFrame>
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
void SendMail::populateJavaScriptWindowObject()
{
    ui.webView->page()->mainFrame()->addToJavaScriptWindowObject("formExtractor", this);
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
        ui.sendButton->setEnabled(true);
        ui.previewButton->setEnabled(true);
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
		QMessageBox::critical(this, "Error while sending mail", "Error while sending mail.");
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
    QString company=ui.txt_Company->text();
    QString address=ui.txt_Address->text();
    QString phone=ui.txt_Phone->text();
    QString firstname= ui.txt_FirstName->text() ;
    QString lastname=  ui.txtContactName->text();
    QString email = ui.txt_Email->text();
    QString message = ui.txt_Messages->toPlainText();
    QString fax = ui.txt_Fax->text();
    QString city = ui.txt_City->text();
    QString country = ui.txt_Country->text();
    QString zipcode = ui.txt_ZipCode->text();
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
    ui.webView->setUrl(QUrl("http://client.dofilink.com/bugreport/add?firstname=" + firstname +
                                       "&lastname=" + lastname +
                                       "&email=" + email +
                                       "&phone=" + phone +
                                       "&fax=" + fax +
                                       "&company=" + company  +
                                       "&address=" + address +
                                       "&zipcode=" + zipcode +
                                       "&city=" + city +
                                       "&country=" + country +
                                       "&key=" + key +
                                       "&trial=" + isTrial +
                                       "&mac=" + mac +
                                       "&message=" + message +
                                       "&code=" + code_postal));

        connect(ui.webView->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
                this, SLOT(populateJavaScriptWindowObject()));
        ui.frame_Info->hide();
        ui.frame_Button->hide();
        ui.frame_Web->show();
        this->setMinimumSize(260,260);
        this->resize(260,260);
}

void SendMail::serviceRequestFinished(QNetworkReply* n)
{
    if(n->error() == 0)
    {
        QMessageBox::about(this,"Success","Send complete data..!");
    }
    else
    {
        QMessageBox::about(this,"Error",QString::number(n->error()) + ": " + n->errorString());
    }
    this->close();
}
