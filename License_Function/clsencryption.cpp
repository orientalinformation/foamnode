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
    return true;
    QString strEmail;
    QString strLicence;
    QString strExpiration;
    QString strActive;
    QString strMac;
    QString strActiveKey;
    QString strResult;


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
            if(strExpiration=="7ff10abb653dead4186089acbd2b7891"){
                nday=1;
            }else if(strExpiration=="7fe8e836921493cba9afc767c65c7233"){
                nday=2;
            }else if(strExpiration=="2ca7631b9e4cf31b33d513d03a9bb5b4"){
                nday=3;
            }else if(strExpiration=="74b6aef81ed70658c01a3d76333f79eb"){
                nday=4;
            }else if(strExpiration=="6219026c9be0964064381537e2f0eb97"){
                nday=5;
            }else if(strExpiration=="026ec4bdc15038dd2dd6e8ce7072c322"){
                nday=6;
            }else if(strExpiration=="9f18f7b13efc41a7eebfe7f20295041f"){
                nday=7;
            }else if(strExpiration=="0832fa99e53d4b60d9b348c0af6a63b8"){
                nday=8;
            }else if(strExpiration=="4900d63bb2dc12c85703724f8d7af816"){
                nday=9;
            }else if(strExpiration=="5b5260163423e92708f42159ca373484"){
                nday=10;
            }else if(strExpiration=="7577bb1e8e1252e490f1c1c1d533cdd1"){
                nday=11;
            }else if(strExpiration=="3efd339ffe54f9214e2a9c0bd7f2126e"){
                nday=12;
            }else if(strExpiration=="4ba6436461b2fde84e93eea448547a74"){
                nday=13;
            }else if(strExpiration=="0006ffa48e2629176dd247c164d77d74"){
                nday=14;
            }else if(strExpiration=="7a79846295930e03b404801d73c4ad2c"){
                nday=15;
            }else if(strExpiration=="2816aa5f1d4f09527ecc8a2b7202730e"){
                nday=16;
            }else if(strExpiration=="39c999661811a24ac6eda5464d9033e7"){
                nday=17;
            }else if(strExpiration=="cbc8e1026e948748a68564c9c0d57fd5"){
                nday=18;
            }else if(strExpiration=="fe4efcbb726800c2431ab74b6e228d5b"){
                nday=19;
            }else if(strExpiration=="1752594fe2868affd4f2a51c8ab7354c"){
                nday=20;
            }else if(strExpiration=="3f1dcad1dc4ba0c984cb9f52b8252af3"){
                nday=21;
            }else if(strExpiration=="fe1af588515420f171096a297cc1ed5f"){
                nday=22;
            }else if(strExpiration=="a46a0ac969a27fe0382b16f5dfafdf83"){
                nday=23;
            }else if(strExpiration=="98706e32de22c7d1c14618898915546a"){
                nday=24;
            }else if(strExpiration=="d7f227a486ef0ebea55684289fd12bcb"){
                nday=25;
            }else if(strExpiration=="1538817d51664c0556c3176c694809be"){
                nday=26;
            }else if(strExpiration=="846267063f24c4609079bcb843deb58c"){
                nday=27;
            }else if(strExpiration=="c4878204908a8251ea7ddc35f9880a39"){
                nday=28;
            }else if(strExpiration=="27dc6747f4c6db079facf44bd8ce1528"){
                nday=29;
            }else if(strExpiration=="914c3d96b420b7cc43e6f580331b1343"){
                nday=30;
            }else if(strExpiration=="914c3d96b420b7cc43e6f580331b1343"){
                nday=31;
            }else if(strExpiration=="54ff85e3adef130d97bb4eb3c6db530c"){
                nday=45;
            }else if(strExpiration=="5f785990b92c053f9a37418205c9bd06"){
                nday=60;
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
