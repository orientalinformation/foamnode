#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <foamcase.h>
#include <foamnode.h>
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QTreeWidgetItem>

using namespace DOFI2::OpenFOAM;

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
    void on_btnBrowse_clicked();

    void on_treeWidget_itemSelectionChanged();

private:
    Ui::MainWindow *ui;
    FoamCase *foamcase;

};

#endif // MAINWINDOW_H
