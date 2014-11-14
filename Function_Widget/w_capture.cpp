#include "w_capture.h"
#include "ui_w_capture.h"

W_Capture::W_Capture(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::W_Capture)
{
    ui->setupUi(this);
    ui->txt_FileSave->setText(OpenFoam::OpenFOAMPath());
    GetvalueCurrentTime = new QDateTime();
    int currentDate = GetvalueCurrentTime->daysTo(GetvalueCurrentTime->currentDateTime());
    saveFileName = "DMesh_"+ QString::number(currentDate);
    ui->txt_namePicture->setText(saveFileName);
}

W_Capture::~W_Capture()
{
    delete ui;
}

QString W_Capture::GetFormat()
{
    if(ui->rbn_BMP->isChecked())
        return ".bmp";
    if(ui->rbn_TIFF->isChecked())
        return ".tiff";
    if(ui->rbn_JPEG->isChecked())
        return ".jpg";
    if(ui->rbn_PNG->isChecked())
        return ".png";
    return "";
}

QString W_Capture::GetFilePath()
{
    if(ui->rbn_SaveToOther->isChecked())
    {
        return ui->txt_FileSave->text();
    }
    return OpenFoam::OpenFOAMPath();
}

void W_Capture::on_rbn_SaveToOther_clicked(bool checked)
{
    if(checked)
        ui->frame_Browse->setEnabled(true);
}

void W_Capture::on_rbn_SaveToThis_clicked(bool checked)
{
    if(checked)
        ui->frame_Browse->setEnabled(false);
}

void W_Capture::on_btn_Browse_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this,"Save to",OpenFoam::OpenFOAMPath());
    if(filePath != "")
        ui->txt_FileSave->setText(filePath);
}

void W_Capture::on_txt_namePicture_editingFinished()
{
    saveFileName = ui->txt_namePicture->text();
}
