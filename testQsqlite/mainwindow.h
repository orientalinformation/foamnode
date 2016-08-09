#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QDebug>
#include <QTableView>
#include <QSqlError>
//#include "QxDao/QxDao.h"
//#include "QxCommon/QxStringCvt.h"
//#include "QxCommon/QxStringCvt_Impl.h"
//#include "QxCommon/QxStringCvt_Export.h"
//#include "QxSerialize/boost/QxExportDllMacroHpp.h"
//#include "QxCommon/QxConfig.h"
//#include "QxCommon/QxMacro.h"
//#include "Materials.gen.h"
#include <QxOrm.h>




namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
