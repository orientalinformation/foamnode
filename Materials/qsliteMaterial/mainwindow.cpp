#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSqlDatabase *db = new QSqlDatabase();
    *db = QSqlDatabase::addDatabase( "QSQLITE" );
     db->setDatabaseName( "materials.sqlite" );
    service->setDatabase(db);
    service->getRelation(1);
    //service->getById(1);
    //service->getAll();
    //service->getById(1);
    //service->getByName("water");
    //service->countAll();

}

MainWindow::~MainWindow()
{
    delete ui;
}
