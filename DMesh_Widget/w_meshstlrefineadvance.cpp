#include "w_meshstlrefineadvance.h"
#include "ui_w_meshstlrefineadvance.h"

W_MeshSTLRefineAdvance::W_MeshSTLRefineAdvance(RefinementSurfaceSTL *rSurface, RefinementFeaturesSTL *rFeature, float *resolveFeatureAngle, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::W_MeshSTLRefineAdvance)
{
    ui->setupUi(this);
    this->rSurface = rSurface;
    this->rFeature = rFeature;
    this->resolveFeatureAngle = resolveFeatureAngle;
    ui->frame_MeshSurfaceFeatures->hide();

    for(int j = 0; j< rSurface->n; j++)
    {
        ui->cb_Regions->addItem(rSurface->regionSTLs[j].name);
    }
    ui->txt_Level_Min_Surface_Refine_STL->setText(QString::number(rSurface->regionSTLs[0].lv1));
    ui->txt_Level_Max_Surface_Refine_STL->setText(QString::number(rSurface->regionSTLs[0].lv2));
    //Resolve angle
    ui->txt_ResolveFeatureAngle->setText(QString::number(*this->resolveFeatureAngle));

    //Feature
    ui->ckb_MeshSurfaceFeatures->setChecked(false);
    if(rFeature->n > 0)
    {
        foreach(FeatureSLT f, rFeature->feature)
        {
            if(f.name == rSurface->name)
            {
                ui->ckb_MeshSurfaceFeatures->setChecked(true);
                ui->txt_Angle_Features_STL->setText(QString::number(f.angle));
                ui->txt_Level_Features_STL->setText(QString::number(f.lv));
                ui->frame_MeshSurfaceFeatures->show();
                break;
            }
        }
    }
}

W_MeshSTLRefineAdvance::~W_MeshSTLRefineAdvance()
{
    delete ui;
}
bool W_MeshSTLRefineAdvance::SetDefine()
{
    bool g,h,t;
    float angle = ui->txt_Angle_Features_STL->text().toFloat(&g);
    float feaLv = ui->txt_Level_Features_STL->text().toFloat(&h);
    float resoleAngle = ui->txt_ResolveFeatureAngle->text().toFloat(&t);
    if(ui->ckb_MeshSurfaceFeatures->isChecked() && (!g || !h || !t))
    {
        QMessageBox::critical(this,tr("Error"),tr("Please input all values in this form...!"));
        return false;
    }    
    *this->resolveFeatureAngle = resoleAngle;
    QString currentSurface = rSurface->name;
    //Set Feature Settings
    bool isFeatureExisted = false;
    if(ui->ckb_MeshSurfaceFeatures->isChecked())
    {
        for(int k = 0; k < rFeature->n; k++)
        {
            if(rFeature->feature[k].name == currentSurface)
            {
                rFeature->feature[k].angle = angle;
                rFeature->feature[k].lv = feaLv;
                isFeatureExisted = true;
                break;
            }
        }
        if(!isFeatureExisted)
        {
            rFeature->n = rFeature->n + 1;
            rFeature->feature.resize(rFeature->n);
            int i = rFeature->n - 1;
            rFeature->feature[i].name = currentSurface;
            rFeature->feature[i].angle = angle;
            rFeature->feature[i].lv = feaLv;
        }
    }
    else
    {
        for(int k = 0; k < rFeature->n; k++)
        {
            if(rFeature->feature[k].name == currentSurface)
            {
                rFeature->feature.remove(k);
                rFeature->n = rFeature->n -1;
                break;
            }
        }
    }
    return true;
}

void W_MeshSTLRefineAdvance::on_ckb_MeshSurfaceFeatures_clicked(bool checked)
{
    if(checked)
    {
        if(ui->txt_Angle_Features_STL->text() == "")
            ui->txt_Angle_Features_STL->setText("150");
        if(ui->txt_Level_Features_STL->text() == "")
            ui->txt_Level_Features_STL->setText("0");
        ui->frame_MeshSurfaceFeatures->show();
    }
    else
        ui->frame_MeshSurfaceFeatures->hide();
}

void W_MeshSTLRefineAdvance::on_cb_Regions_currentIndexChanged(QString str)
{
    for(int j = 0; j< rSurface->n; j++)
    {
        if(str == rSurface->regionSTLs[j].name)
        {
            ui->txt_Level_Min_Surface_Refine_STL->setText(QString::number(rSurface->regionSTLs[j].lv1));
            ui->txt_Level_Max_Surface_Refine_STL->setText(QString::number(rSurface->regionSTLs[j].lv2));
            return;
        }
    }
}

void W_MeshSTLRefineAdvance::on_btn_Apply_clicked()
{
    QString str = ui->cb_Regions->currentText();
    bool a,b;
    float c = ui->txt_Level_Min_Surface_Refine_STL->text().toFloat(&a);
    float d = ui->txt_Level_Max_Surface_Refine_STL->text().toFloat(&b);
    if(!c || !d)
    {
        QMessageBox::critical(this,tr("Error"),tr("Please input the valid values in this form...!"));
        return;
    }
    for(int j = 0; j< rSurface->n; j++)
    {
        if(str == rSurface->regionSTLs[j].name)
        {
            rSurface->regionSTLs[j].lv1 = c;
            rSurface->regionSTLs[j].lv2 = d;
            return;
        }
    }
}

void W_MeshSTLRefineAdvance::on_btn_Ok_clicked()
{
    if(SetDefine())
    {
        this->setResult(1);
        this->close();
    }
}
