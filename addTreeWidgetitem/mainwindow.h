#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QString>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void addTreeChild(QTreeWidgetItem *parent, QString type, QString name );
    void addTreeRoot(QString type, QString name);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
