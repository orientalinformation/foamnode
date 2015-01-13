#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QApplication>
#include <QThread>
#include <QWidget>
#include <QProcess>
#include <QFile>
#include <QMessageBox>
#include "openfoam.h"

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = 0);
    void SetCommand(QString command);
    void SetSubCommand(QString command,int index);
    void ThreadName(QString name);
    void Terminate();
    QString ThreadName();
    QString logPath;
    bool writeLog;
private:
    QProcess *process;
    QString mainCommand;
    QString sourceOF;
    QString command;
    QString subCommand_1;
    QString subCommand_2;
    QString subCommand_3;
    QString threadName;
    QString PID;
    bool emitappend;
    QString FilterLog(QString value);
    int checkFlag;
protected:
    void run();
signals:
    emit void changed(QString value);

};

#endif // MYTHREAD_H
