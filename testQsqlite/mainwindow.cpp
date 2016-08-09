#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/home/dongtp/sqlite/materials.db");
    if(!db.open()){
        qDebug() << "Can't open database!";
        ui->pushButton->setText("Can't open database");
    }else{
        qDebug() << "Connected";
        ui->pushButton->setText("Connected");
    }

    //show table on Qsqlite
    QSqlTableModel * model = new QSqlTableModel(this,db);
    model->setTable("material_types");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
//    model->setHeaderData(0, Qt::Horizontal, tr("name"));

    ui->tableView->setModel(model);
//    QTableView *view = new QTableView;
//    view->setModel(model);
//    view->hideColumn(0);// don't show the ID
//    view->show();


    //Query

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{

    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/home/dongtp/sqlite/materials.db");
    if(!db.open()){
        qDebug() << "Can't open database!";
        ui->pushButton->setText("Can't open database");
    }else{
        qDebug() << "Connected";
        ui->pushButton->setText("Connected");
    }
    //insert,updated,delete
    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS material_types (id INTEGER UNIQUE PRIMARY KEY, name VARCHAR(45)");
      if( !query.exec() )
        qDebug() << query.lastError();
      else
        qDebug() << "Table created!";

      query.prepare( "INSERT INTO material_types (id, name) VALUES (3, 'suild')" );
      if( !query.exec() )
        qDebug() << query.lastError();
      else
        qDebug( "Inserted!" );

    //show table on Qsqlite
    QSqlTableModel * model = new QSqlTableModel(this,db);
    model->setTable("material_types");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
//    model->setHeaderData(0, Qt::Horizontal, tr("name"));

    ui->tableView_2->setModel(model);


}

void MainWindow::on_pushButton_3_clicked()
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/home/dongtp/sqlite/materials.db");
    if(!db.open()){
        qDebug() << "Can't open database!";
        ui->pushButton->setText("Can't open database");
    }else{
        qDebug() << "Connected";
        ui->pushButton->setText("Connected");
    }
    //insert,updated,delete
    QSqlQuery query;
    query.prepare( "UPDATE material_types SET name = 'fluid2' WHERE id = 1" );
      if( !query.exec() )
        qDebug() << query.lastError();
      else
        qDebug( "Updated!" );

    //show table on Qsqlite
    QSqlTableModel * model = new QSqlTableModel(this,db);
    model->setTable("material_types");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
//    model->setHeaderData(0, Qt::Horizontal, tr("name"));

    ui->tableView_2->setModel(model);

}
