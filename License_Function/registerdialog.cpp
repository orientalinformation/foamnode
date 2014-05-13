#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "QMessageBox"
#include "clsencryption.h"
#include <QNetworkInterface>
#include <QFile>
#include <QtCore>
#include <qtextstream.h>
#include <QCryptographicHash>
#include <QFileDialog>

#define IO_ReadOnly QIODevice::ReadOnly
RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_txt_KeyFile_textChanged(QString a)
{
    clsEncryption cls;
    bool temp;
    temp=cls.checkKey(ui->txt_KeyFile->text());
    if(temp==true)
    {
        ui->checkBox->setCheckState(Qt::Checked);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
    else
    {
        ui->lineEdit->setText("");
        ui->checkBox->setCheckState(Qt::Unchecked);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}

void RegisterDialog::on_btn_Browse_clicked()
{
    QString fileName ;
    fileName= QFileDialog::getOpenFileName(this, tr("Open File"),"/home",tr("File (*.dat)"));
    ui->txt_KeyFile->setText(fileName);
    this->readFile(fileName);
}
void RegisterDialog::readFile(QString fileName)
{    
    QString strEmail;
    QFile file(fileName);
    QString line;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&file);
    while (!in.atEnd()) {
        line = in.readLine();
        if(line.contains("Email=",Qt::CaseInsensitive)==true){
            strEmail=line.replace("Email=","");
            ui->lineEdit->setText(strEmail);
        }
    }
}
void RegisterDialog::on_buttonBox_accepted()
{
    if(QFile("Data/.Licence.dat").exists())
        QFile::remove("Data/.Licence.dat");
    QFile::copy(ui->txt_KeyFile->text(),"Data/.Licence.dat");
//    QString fileName;
//    QString temp;
//    fileName=QApplication::applicationDirPath()+ "/Register.dat";
//    clsEncryption cls;
//    if(cls.checkDay(fileName)==true){
//        temp="";
//    }else{
//        temp="";
//    }
}

void RegisterDialog::writeFile(QString fileName)
{
    QString strDay;
    QString strResult;
    QString strTemp;
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    //Encryption MD5
    int intDay;
    int intMonth;
    int intYear;
    intDay=QDate::currentDate().day();
    intMonth=QDate::currentDate().month();
    intYear=QDate::currentDate().year();
    strDay=QString::number(intDay) + "." + QString::number(intMonth) + "." + QString::number(intYear);
    strTemp=strDay;
    strDay=strDay+"day";
    QByteArray hashMD5=QCryptographicHash::hash(strDay.toUtf8(),QCryptographicHash::Md5);
    strResult=hashMD5.toHex();

    out << "[CLIENT]\n";
    out << "RegisterKey=" + strResult+"\n";
    out << "Register=" + strTemp+"\n";
    file.close();
}

