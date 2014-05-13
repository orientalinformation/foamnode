#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

namespace Ui {
    class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = 0);
    ~RegisterDialog();
    void readFile(QString fileName);
    void writeFile(QString fileName);

private:
    Ui::RegisterDialog *ui;

private slots:

    void on_buttonBox_accepted();
    void on_btn_Browse_clicked();
    void on_txt_KeyFile_textChanged(QString );
};

#endif // REGISTERDIALOG_H
