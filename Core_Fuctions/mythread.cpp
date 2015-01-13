#include "mythread.h"

MyThread::MyThread(QObject *parent) :
    QThread(parent)
{
    #if defined(Q_OS_WIN)
        this->sourceOF = QApplication::applicationDirPath() + "/DOFICore/setvars.bat";
//        this->sourceOF ="C:/Users/Napyidaw/Desktop/DMesh_Working/DMesh-build-desktop/DOFICore\\setvars.bat";
    #else
//    QString dofidir = "/home/longlp/Desktop/DOFI_Working/OpenFoamGUI-build-desktop";
    this->sourceOF = "DOFIDIR=" + QApplication::applicationDirPath() + " && source "  + QApplication::applicationDirPath() +  "/DOFICore/openfoam211/etc/bashrc";
//    this->sourceOF = "DOFIDIR=" + dofidir + " && source "  + dofidir +  "/DOFICore/openfoam211/etc/bashrc";
    #endif
    this->command = "";
    this->subCommand_1 = "";
    this->subCommand_2 = "";
    this->threadName = "";
    this->emitappend = true;
    logPath = "";
    writeLog = false;
    checkFlag = 0;
}
QString MyThread::ThreadName()
{
    return this->threadName;
}

QString MyThread::FilterLog(QString value)
{
//    if(flag == true){
        if(value.contains("Read mesh in")){
            checkFlag = 1;
            return value;
        }
        if(value.trimmed() == ""){
            return "NULL-NONE";
        }
        if(value.contains("Marked for refinement due to") || value.contains("Keeping all cells in region")
                || value.contains("markFacesOnProblemCells") || value.contains("markFacesOnProblemCells")
                || value.contains("Introducing baffles for")){
            checkFlag = 0;
            return "NULL-NONE";
        }
        if(value.contains("Selected for refinement : 0 cells") || value.contains("Number of intersected edges"))
        {
            checkFlag = 1;
            return "NULL-NONE";
        }
        if(value.contains("Checking faces in error :") || value.contains("Moving mesh using diplacement scaling")){
            checkFlag = 0;
            return "NULL-NONE";
        }
        if(value.contains("faces on cells with determinant"))
        {
            checkFlag = 1;
            return "NULL-NONE";
        }
        if(checkFlag == 0){
            return "NULL-NONE";
        }
        if(checkFlag == 1){
            return value;
        }
//    }
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
//        if(temp_Command.indexOf("-case") == -1)
//            this->command = sourceOF + " && " + subCommand_1 + " " + command + " -case \"" + OpenFoam::OpenFOAMPath() + "\" " + subCommand_2 + " 2>&1";
//        else
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
//    emit changed(command);
    if(writeLog && this->ThreadName() != "checkMesh" && this->ThreadName() != "paraFoam"){
        QFile file(logPath);
        file.open(QIODevice::Append);
        file.write(command.toAscii().data());
        file.close();
    }
    #if defined(Q_OS_WIN)
    {
        process = new QProcess();
        process->setProcessChannelMode(QProcess::MergedChannels);
        process->setWorkingDirectory(OpenFoam::OpenFOAMPath());
        process->start(command,QIODevice::ReadOnly);
        while(process->waitForReadyRead(-1))
            while(process->canReadLine()){
//                emit changed(process->readLine());
                QString temp = process->readLine();
                if(writeLog && this->ThreadName() != "checkMesh" && this->ThreadName() != "paraFoam"){
                    QFile file(logPath);
                    file.open(QIODevice::Append);
                    file.write(temp.toAscii().data());
                    file.close();
                }
//                if(this->ThreadName() != "blocMesh"){
                    if(temp.contains("/*-----")){
                        emitappend = false;
                    }
                    if(temp.contains("// * *")){
                        emitappend = true;
                    }
                    if(temp.left(3) == "PID"){
                        QStringList temps = temp.trimmed().split(":",QString::SkipEmptyParts);
                        if(temps.size() == 2){
                            PID= temps[1];
                        }
                    }
                    temp = FilterLog(temp);
                    if(emitappend && temp != "NULL-NONE"){
                        emit changed(temp);
                    }
//                }
            }
    }
    #else
    {
        QStringList commands;
        commands.append("-c");
        commands.append(command);
        process = new QProcess();
        process->setProcessChannelMode(QProcess::MergedChannels);
        process->setWorkingDirectory(OpenFoam::OpenFOAMPath());
        process->start("/bin/bash",commands,QIODevice::ReadOnly);
        while(process->waitForReadyRead(-1))
            while(process->canReadLine()){
                QString temp = process->readLine();
                if(writeLog && this->ThreadName() != "checkMesh" && this->ThreadName() != "paraFoam"){
                    QFile file(logPath);
                    file.open(QIODevice::Append);
                    file.write(temp.toAscii().data());
                    file.close();
                }
//                if(this->ThreadName() != "blocMesh"){
                    if(temp.contains("/*-----")){
                        emitappend = false;
                    }
                    if(temp.contains("// * *")){
                        emitappend = true;
                    }
                    if(temp.left(3) == "PID"){
                        QStringList temps = temp.trimmed().split(":",QString::SkipEmptyParts);
                        if(temps.size() == 2){
                            PID= temps[1];
                        }
                    }
                    QString temp1 = temp;
                    temp = FilterLog(temp1);
                    if(emitappend && temp != "NULL-NONE"){
                        emit changed(temp);
                    }
//                }
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
