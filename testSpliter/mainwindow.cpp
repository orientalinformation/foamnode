#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnBrowse_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(this,"foamcase");
    ui->ledShow->setText(fileName);

    foamcase = new FoamCase();
    foamcase->open(fileName);

    QTreeWidgetItem * zeroItem = new QTreeWidgetItem(QStringList() << "0");
    ui->treeWidget->addTopLevelItem(zeroItem);
    QTreeWidgetItem * systemItem = new QTreeWidgetItem(QStringList() << "system");
    ui->treeWidget->addTopLevelItem(systemItem);
    QTreeWidgetItem * constantItem = new QTreeWidgetItem(QStringList() << "constant");
    ui->treeWidget->addTopLevelItem(constantItem);

    QHash<QString, FoamNode*>::const_iterator i = foamcase->zero.constBegin();
    while (i != foamcase->zero.constEnd()) {

        QString fileName = i.key();
        QTreeWidgetItem * item = new  QTreeWidgetItem(QStringList() << fileName);
        zeroItem->addChild(item);
        ++i;
    }
    QHash<QString, FoamNode*>::const_iterator j = foamcase->system.constBegin();
    while(j != foamcase->system.constEnd()){

        QString fileName = j.key();
        QTreeWidgetItem * item = new QTreeWidgetItem(QStringList() << fileName);
        systemItem->addChild(item);
        ++j;
    }
    QHash<QString, FoamNode*>::const_iterator k = foamcase->constant.constBegin();
    while(k != foamcase->constant.constEnd()){
        QString fileName = k.key();
        constantItem->addChild( new QTreeWidgetItem(QStringList() << fileName ));
        ++k;
    }


}

void MainWindow::on_treeWidget_itemSelectionChanged()
{
    QTreeWidgetItem * item;
    item = ui->treeWidget->selectedItems().first();
    if(item->parent() != (QTreeWidgetItem*)ui->treeWidget){

        QString parent = ((QTreeWidgetItem*) item->parent())->text(0);
        if(parent == "0"){
            ui->txtShowContent->setPlainText(this->foamcase->zero[item->text(0)]->format().join("\n"));
        }else if(parent == "system"){
            ui->txtShowContent->setPlainText(this->foamcase->system[item->text(0)]->format().join("\n"));
        }else{
            ui->txtShowContent->setPlainText(this->foamcase->constant[item->text(0)]->format().join("\n"));
        }
    }

}
