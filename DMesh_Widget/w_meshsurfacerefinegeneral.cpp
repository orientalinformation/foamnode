#include "w_meshsurfacerefinegeneral.h"
#include "ui_w_meshsurfacerefinegeneral.h"

W_MeshSurfaceRefineGeneral::W_MeshSurfaceRefineGeneral(DMesh *d, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::W_MeshSurfaceRefineGeneral)
{
    ui->setupUi(this);
    mesh = d;
    ui->txt_Expansion_Surface_General->setText(QString::number(mesh->snappyd->add_Layers_Controls.expansionRatio));
    ui->txt_Final_Surface_General->setText(QString::number(mesh->snappyd->add_Layers_Controls.finalLayerThickness));
    ui->txt_Layer_Surface_General->setText(QString::number(mesh->snappyd->add_Layers_Controls.minThickness));
    ui->txt_FeatureAngel_Surface_General->setText(QString::number(mesh->snappyd->add_Layers_Controls.featureAngle));
    ui->txt_SlipFeatureAngel_Surface_General->setText(QString::number(mesh->snappyd->add_Layers_Controls.slipFeatureAngle));
    if(mesh->snappyd->add_Layers_Controls.relativeSizes==true)
    {
        ui->cbx_Relative_Surface_General->setCurrentIndex(0);
    }
    else
        ui->cbx_Relative_Surface_General->setCurrentIndex(1);

}

W_MeshSurfaceRefineGeneral::~W_MeshSurfaceRefineGeneral()
{
    delete ui;
}

void W_MeshSurfaceRefineGeneral::on_buttonBox_accepted()
{
    bool a,b,c,d,e;
    float x = ui->txt_Final_Surface_General->text().toFloat(&a);
    float y = ui->txt_Layer_Surface_General->text().toFloat(&b);
    float z = ui->txt_Expansion_Surface_General->text().toFloat(&c);
    float m = ui->txt_FeatureAngel_Surface_General->text().toFloat(&d);
    float n = ui->txt_SlipFeatureAngel_Surface_General->text().toFloat(&e);

    if(a== false || b==false || c==false || d==false || e==false)
    {
        QMessageBox::critical(this,tr("Error"),tr("Please input all values in this form...!"));
        return;
    }
    if(ui->cbx_Relative_Surface_General->currentIndex()==0)
        mesh->snappyd->add_Layers_Controls.relativeSizes =true;
    else
        mesh->snappyd->add_Layers_Controls.relativeSizes =false;
    mesh->snappyd->add_Layers_Controls.finalLayerThickness = x;
    mesh->snappyd->add_Layers_Controls.minThickness = y;
    mesh->snappyd->add_Layers_Controls.expansionRatio = z;
    mesh->snappyd->add_Layers_Controls.featureAngle = m;
    mesh->snappyd->add_Layers_Controls.slipFeatureAngle = n;
}
