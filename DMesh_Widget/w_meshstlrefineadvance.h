#ifndef W_MESHSTLREFINEADVANCE_H
#define W_MESHSTLREFINEADVANCE_H

#include <QDialog>
#include <Core_Fuctions/snappy_dmesh.h>
#include <QMessageBox>

namespace Ui {
    class W_MeshSTLRefineAdvance;
}

class W_MeshSTLRefineAdvance : public QDialog
{
    Q_OBJECT

public:
    explicit W_MeshSTLRefineAdvance(RefinementSurfaceSTL *rSurface,RefinementFeaturesSTL *rFeature,int resolveFeatureAngle,QWidget *parent = 0);
    ~W_MeshSTLRefineAdvance();

private:
    Ui::W_MeshSTLRefineAdvance *ui;
    RefinementSurfaceSTL *rSurface;
    RefinementFeaturesSTL *rFeature;
    int resolveFeatureAngle;
    bool SetDefine();

private slots:
    void on_btn_Ok_clicked();
    void on_btn_Apply_clicked();
    void on_cb_Regions_currentIndexChanged(QString );
    void on_ckb_MeshSurfaceFeatures_clicked(bool checked);
};

#endif // W_MESHSTLREFINEADVANCE_H
