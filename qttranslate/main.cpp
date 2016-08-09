#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QInputDialog>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    QStringList language;
    language << "English" << "France";
    const QString lang = QInputDialog::getItem(NULL,"Language","Select a language",language);
    if(lang == "France"){
        translator.load(":/qttranslate.qm");
        a.installTranslator(&translator);
    }else {

    }

    MainWindow w;
    w.show();
    return a.exec();
}
