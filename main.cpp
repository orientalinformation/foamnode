#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug("Application starting up");

    #if defined(Q_OS_WIN)
    //check one instance of DMesh may be run
    QSharedMemory memInstanceLock("8Wb5631F");
    if ( memInstanceLock.attach() ) {
        QMessageBox::information(0,"DMesh","Only one DMesh can run on this computer...!");
        return 1;
    } else {
        if (!memInstanceLock.create(512, QSharedMemory::ReadWrite)) {
            qDebug("Unable to create single instance.");
            return 1;
        }
    }
    #endif
    MainWindow w;
    w.setWindowState(Qt::WindowMaximized);
    w.show();

    return a.exec();
}
