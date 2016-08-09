#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTranslator>
#include <QMessageBox>

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

void MainWindow::on_pushButton_clicked()
{
    const double number1 = ui->spxNumber1->value();
    const double number2 = ui->spxNumber2->value();
    double res = 0.0;
    switch (ui->cbxTranslate->currentIndex()) {
    case 0: res = number1 + number2;
        break;
    case 1: res = number1 - number2;
        break;
    case 2: res = number1 * number2;
        break;
    case 3: res = number1 / number2;
        break;
    default:
        break;
    }
    QMessageBox::information(this,tr("Result to do"),tr("Result : %1").arg(res));
}

void MainWindow::on_actionExit_triggered()
{
    close();
}
