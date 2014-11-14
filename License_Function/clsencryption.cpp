#include "clsencryption.h"
#include <QCryptographicHash>
#include <QtCore>
#include <QFile>
#include <QNetworkInterface>
#include <QApplication>
clsEncryption::clsEncryption(QObject *parent) :
    QObject(parent)
{    
}

QString clsEncryption::mixString(QString strMac, QString strEmail)
{
    QString strTemp="";
     //expirationDay="01/01/2012";
    if(strMac=="" || strEmail=="")
    {
        return strTemp;
    }else{
        //check SHA1 on Qt
        strTemp=strMac+strEmail;
        QByteArray arrGlobal=strTemp.toLocal8Bit();        
        arrGlobal.reserve(arrGlobal.size());
        int j=arrGlobal.size()-1;
        for(int i=0;i<arrGlobal.size();i++)
        {            
            strTemp+=arrGlobal[j];
            j--;
        }
        return strTemp;
    }

}

QString clsEncryption::hashMD5(QString strMac, QString strEmail)
{
//    clsEncryption cls;
//    QString str;
//    if(strMac!="" and strEmail!="" )
//    {
//        str=cls.mixString(strMac,strEmail);
//        str=str.toLower();
//        QByteArray hashMD5=QCryptographicHash::hash(str.toUtf8(),QCryptographicHash::Md5);
//        str=hashMD5.toHex();
//    }else{
//        str="";
//    }
//    return str;
	return "";
}

void clsEncryption::reserveArray(QByteArray arr){
    QByteArray temp;
    int lenghtArr=arr.size();
    for(int i=0;i<(lenghtArr/2);i++)
    {
        temp[i]=arr[i];
        arr[i]=arr[lenghtArr-i-1];
        arr[lenghtArr-i-1]=temp[lenghtArr-i-1];
    }

}

bool clsEncryption::checkKey(QString fileName)
{
    QString strEmail;
    QString strLicence;
    QString strExpiration;
    QString strActive;
    QString strMac;
    QString strActiveKey;
    QString strResult;

//    return true;
    //get MAC address
    foreach (const QNetworkInterface &ni, QNetworkInterface::allInterfaces())
    {
        QString temp = ni.hardwareAddress();
        temp=temp.replace(":","");
        if(!temp.contains("0000") && temp != "")
        {
            strMac = temp;
            break;
        }
    }

    //read file
    int nday;
    QFile file(fileName);
    QString line;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream in(&file);
    while (!in.atEnd())
    {
        line = in.readLine();
        if(line.contains("Email=",Qt::CaseInsensitive)==true){
            strEmail=line.replace("Email=","");
        }else if(line.contains("Licence=",Qt::CaseInsensitive)==true){
            strLicence=line.replace("Licence=","");
        }else if(line.contains("Expiration=",Qt::CaseInsensitive)==true){
            strExpiration = line.replace("Expiration=","");
            if(strExpiration=="914c3d96b420b7cc43e6f580331b1343"){
                nday=30;
            }else if(strExpiration=="5f785990b92c053f9a37418205c9bd06"){
                nday=60;
            }else if(strExpiration=="260261a5bdd468d9b1e8a9c4f474d97d"){
                nday=90;
            }else if(strExpiration=="6149b7e6e4cc56e3cbf592bdf8a99eb7"){
                nday=120;
            }else if(strExpiration=="4346fda2a08651636023da9bb3713512"){
                nday=150;
            }else if(strExpiration=="b7bd0f07ffbe9b8092d66cb507f97a37"){
                nday=180;
            }else if(strExpiration=="513673e86d3dc6f49ac011f451b0a697"){
                nday=365;
            }else{
                nday=0;
            }
        }else if(line.contains("ActiveKey=",Qt::CaseInsensitive)==true){
            strActiveKey=line.replace("ActiveKey=","");
        }else if(line.contains("Active=",Qt::CaseInsensitive)==true)
        {
            strActive = line.replace("Active=","");           
        }
    }

    //get date
    QDate dayBefore;
    QDate dayAfter;

    dayBefore=QDate::fromString(strActive,"dd.MM.yyyy");
    dayAfter=QDate::currentDate();
    dayBefore=dayBefore.addDays(nday);
    QDate d1(dayAfter.year(), dayAfter.month(), dayAfter.day());
    QDate d2(dayBefore.year(), dayBefore.month(), dayBefore.day());

    //check Licence   
    strResult=this->mixString(strMac,strEmail);
    strResult=strResult.toLower();
    QByteArray hashMD5=QCryptographicHash::hash(strResult.toUtf8(),QCryptographicHash::Md5);
    strResult=hashMD5.toHex();

    strActive=strActive + "day";
    QByteArray hashMD5_Active=QCryptographicHash::hash(strActive.toUtf8(),QCryptographicHash::Md5);
    strActive=hashMD5_Active.toHex();

    QString fileNameRegister;
    fileNameRegister=QApplication::applicationDirPath()+ "/.Register.dat";
//    //check date
//    if(strResult != "")
//    {
//        return true;
//    }else{
//        return false;
//    }

    if(strResult==strLicence && strActive==strActiveKey)
    {
        if(d2>=d1)
        {
            if(QFile::exists(fileNameRegister))
            {
                if(this->checkDay(fileNameRegister)==true)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return true;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

}

bool  clsEncryption::checkDay(QString fileName)
{
    //Check file
    QString strActive;
    QFile file(fileName);
    QString line;
    QString RegisterKey;
    QString strRegister;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream in(&file);
    while (!in.atEnd())
    {
        line = in.readLine();
        if(line.contains("RegisterKey=",Qt::CaseInsensitive)==true){
            RegisterKey=line.replace("RegisterKey=","");
        }else if(line.contains("Register=",Qt::CaseInsensitive)==true){
            strRegister=line.replace("Register=","");
        }
    }
    strRegister=strRegister + "day";
    QByteArray hashMD5_Register=QCryptographicHash::hash(strRegister.toUtf8(),QCryptographicHash::Md5);
    strActive=hashMD5_Register.toHex();

    //Check current day with file
    QDate dBefore;
    QDate dAfter;
    strRegister=strRegister.replace("day","");
    dAfter=QDate::currentDate();
    dBefore=QDate::fromString(strRegister,"dd.MM.yyyy");

    QDate d1(dAfter.year(), dAfter.month(), dAfter.day());
    QDate d2(dBefore.year(), dBefore.month(), dBefore.day());
    if(strActive==RegisterKey){
        if(d1>=d2){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}
