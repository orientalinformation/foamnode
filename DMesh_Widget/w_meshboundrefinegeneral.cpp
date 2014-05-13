#include "W_MeshBoundRefineGeneral.h"
#include "ui_W_MeshBoundRefineGeneral.h"

W_MeshBoundRefineGeneral::W_MeshBoundRefineGeneral(DMesh *d, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::W_MeshBoundRefineGeneral)
{
    ui->setupUi(this);
    mesh = d;
    ui->txt_Layer_Bounding->setText(QString::number(mesh->blockd->gen_Bounding.Layers));
    ui->txt_X->setText(QString::number(mesh->blockd->gen_Bounding.element.x));
    ui->txt_Y->setText(QString::number(mesh->blockd->gen_Bounding.element.y));
    ui->txt_Z->setText(QString::number(mesh->blockd->gen_Bounding.element.z));
}

W_MeshBoundRefineGeneral::~W_MeshBoundRefineGeneral()
{
    delete ui;
}

void W_MeshBoundRefineGeneral::on_buttonBox_accepted()
{
    bool a,b,c,d;
    int v = ui->txt_Layer_Bounding->text().toInt(&d);
    float x = ui->txt_X->text().toFloat(&a);
    float y = ui->txt_Y->text().toFloat(&b);
    float z = ui->txt_Z->text().toFloat(&c);


    if(a== false || b==false || c==false || d==false)
    {
        QMessageBox::critical(this,"Error","Please input all values in this form...!");
        return;
    }
    mesh->blockd->gen_Bounding.Layers = v;
    mesh->blockd->gen_Bounding.element.x = x;
    mesh->blockd->gen_Bounding.element.y = y;
    mesh->blockd->gen_Bounding.element.z = z;
}
