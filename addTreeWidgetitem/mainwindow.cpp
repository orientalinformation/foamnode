#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeWidget->setColumnCount(2);
//    addTreeRoot("FluidRoot ", "airRoot");
//    addTreeRoot("SolidRoot ", "cuRoot");
    //QHash <QString, QTreeWidgetItem> dbType;
    QString type = "fluid";
    //QString name = "air";
    QTreeWidgetItem *itemRoot = new QTreeWidgetItem(ui->treeWidget);
    itemRoot->setText(0,type);
    QTreeWidgetItem *itemChild = new QTreeWidgetItem(itemRoot);
    itemChild->setText(0,"water");
    itemChild->setText(1,"mua");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addTreeChild(QTreeWidgetItem *parent, QString type, QString name)
{
    QTreeWidgetItem * treeItem = new QTreeWidgetItem();
    treeItem->setText(0, type);
    treeItem->setText(1, name);
    parent->addChild(treeItem);
}

void MainWindow::addTreeRoot(QString type, QString name)
{
    QTreeWidgetItem * treeRoot = new QTreeWidgetItem(ui->treeWidget);
    treeRoot->setText(0, type);
    treeRoot->setText(1, name);
    addTreeChild(treeRoot,  "Fluid", "air");
    addTreeChild(treeRoot,  "Solid", "cu");
}
