#include "authdialog.h"
#include "ui_authdialog.h"

AuthDialog::AuthDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthDialog)
{
    ui->setupUi(this);
}

AuthDialog::~AuthDialog()
{
    delete ui;
}

QString AuthDialog::username()
{
    return ui->userEdit->text();
}

QString AuthDialog::password()
{
    return ui->passwordEdit->text();
}

bool AuthDialog::saveChecked()
{
    return ui->ckbSaveAuthentication->isChecked();
}
