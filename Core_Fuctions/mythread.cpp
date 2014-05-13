#include "mythread.h"

MyThread::MyThread(QObject *parent) :
    QThread(parent)
{
    #if defined(Q_OS_WIN)
        this->sourceOF = "DOFICore\\setvars.bat";
    #else
        this->sourceOF = ". ./DOFICore/openfoam211/etc/bashrc";
    #endif
    this->command = "";
    this->subCommand_1 = "";
    this->subCommand_2 = "";
    this->threadName = "";
}
QString MyThread::ThreadName()
{
    return this->threadName;
}
void MyThread::ThreadName(QString name)
{
    this->threadName = name;
}

void MyThread::SetCommand(QString command)
{
    QStringList temp_Command = command.split(" ");
    this->mainCommand = temp_Command[0];

    #if defined(Q_OS_WIN)
        this->command = sourceOF + " \"" + OpenFoam::OpenFOAMPath() + "\" && " + subCommand_1 + " " + command + " " + subCommand_2 + " 2>&1";
    #else
        if(temp_Command.indexOf("-case") == -1)
            this->command = sourceOF + " && " + subCommand_1 + " " + command + " -case \"" + OpenFoam::OpenFOAMPath() + "\" " + subCommand_2 + " 2>&1";
        else
            this->command = sourceOF + " && " + subCommand_1 + " " + command + " " + subCommand_2 + " 2>&1";
    #endif
}
void MyThread::SetSubCommand(QString command, int index)
{
    switch(index)
    {
        case 1:
        {
            this->subCommand_1 = command;
            break;
        }
        case 2:
        {
            this->subCommand_2 = command;
            break;
        }
    }
}
void MyThread::run()
{
    #if defined(Q_OS_WIN)
    {
        process = new QProcess();
        process->setProcessChannelMode(QProcess::MergedChannels);
        process->start(command,QIODevice::ReadOnly);
        while(process->waitForReadyRead(-1))
            while(process->canReadLine())
                emit changed(process->readLine());
    }
    #else
    {
        FILE *pipe;
        pipe = popen(this->command.toAscii().data(),"r");
        char buffer[128];
        while(!feof(pipe))
        {
            if(fgets(buffer, 128, pipe) != NULL)
            {
                emit changed(buffer);
            }
    	}
    }
    #endif
    this->exit();
}
void MyThread::Terminate()
{
    if(OpenFoam::PID() == "-1")
        return;
    QProcess *taskKill;
    taskKill = new QProcess();
    QString command;
    if(this->threadName == "calculator")
        mainCommand = OpenFoam::Solver();
    #if defined(Q_OS_WIN)
    {
        command = "taskkill /PID " + OpenFoam::PID() + " /F /T";
        taskKill->start(command,QIODevice::ReadOnly);
        process->kill();
    }
    #else
    {
        command = "kill " + OpenFoam::PID();
        system(command.toAscii().data());
    }
    #endif
    OpenFoam::PID("-1");
}
