#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QFileDialog>
#include "DMesh_Widget/w_meshsurfacerefinegeneral.h"
#include "DMesh_Widget/w_meshboundrefinegeneral.h"
#include "DMesh_Widget/w_meshstlrefineadvance.h"
#include "Core_Fuctions/dmesh.h"
#include "Core_Fuctions/mythread.h"
#include "Core_Fuctions/patchdict.h"
#include "License_Function/sendmail.h"
#include "License_Function/registerdialog.h"
#include "License_Function/clsencryption.h"
#include "Function_Widget/w_capture.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool loaded;
private:
    Ui::MainWindow *ui;
    DMesh *mesh;
    MyThread *createrThread;
    QStringList listFaces;
    QStringList listSurfaces;
    QStringList listSurfacesDefault;
    //declare file name import *STL
    QString file_name_STL;
    //declare path Open
    QString path_Open;
    //declare flag
    //flag refine surface
    bool flag_btnSurface_Click;
    //flag refine bounding
    bool flag_btnBounding_Click;
    //flag Item List Face click
    bool flag_Item_Face_Click;
    //flag add bounding fist
    bool flag_Add_Bounding;
    //flag button Volume click
    bool flag_btnVolume_Click;
    //flag button Surface Geometry
    bool flag_btnSurGeometry_Click;
    //flag button Specific click
    bool flag_btnSpecific_Click;
    //flag button btn_MeshLayer click
    bool flag_btn_MeshLayer_Click;
    //check DofiCore
    bool CheckDFC;
    //check License
    bool licenseOK;
    //enable Close trigger
    bool isOpen;

    void LoadLocationInMesh();
    void LoadControlItems();
    void LoadMeshControlItems();
    void LoadGeometryControlItems();
    void LoadBoundaryControlItems();

    void LoadControlsVisible();
    void LoadGeometryControlsVisible();
    void LoadMeshControlsVisible();
    void LoadBoundaryControlsVisible();
    void LoadGenerateControlsVisible();

    bool CheckAndSaveBoundingBoxInput();
    bool CheckAndSaveBoundingDistance();
    bool AddFaceToList(QString name);
    bool SetLocation();
    void Remove_All_Face();
    bool AddSurfaceRegionBox();
    bool AddUserDefine();
    bool CheckNameValid(QString value);
    void CheckLicense();
    void SetButtonDefault();
    void SetButtonEnable(bool value);
    void SetButtonDisplayEnable(bool value);
    bool SetBoundingDistance(float dMinX = 0, float dMaxX = 0, float dMinY = 0, float dMaxY = 0, float dMinZ = 0, float dMaxZ = 0);

    //reset draw boxs
    void ResetDrawBoxs();
    void LoadRefineDistanceSurface(QString currentSurface, int type);
    void ReloadRefineDistanceSurface(RefinementRegion *refi_Reg);
    void loadData();
    void boundaryDefault();
    void checkDFC();

    void ImportSTLSurface();
    void ImportSTLCellzone();
    void DefineSimpleSurface();
    void DefineSimpleVolume();
    void DefineSimpleCellZone();

    bool AddRefineVolume(RefinementRegions *refi_Reg, QString currentSurface, QString mode, float lv);
    bool RemoveRefineRegion(RefinementRegions *refi_Reg,QString currentSurface,float lv);
    bool AddRefineRegion(RefinementRegions *refi_Reg, QString currentSurface, RefinementDistance r);
private slots:
    void on_actionOpen_triggered();
    void on_txt_Level_Volume_editingFinished();
    void on_txt_Level_Max_Surface_Refine_editingFinished();
    void on_txt_Level_Min_Surface_Refine_editingFinished();
    void on_txt_Z_textChanged(QString );
    void on_txt_Y_textChanged(QString );
    void on_txt_X_textChanged(QString );
    void on_tb_MeshRefineAroundSurface_cellClicked(int row, int column);
    void on_btn_MeshRefineAdvance_clicked();
    void on_btn_DeleteBoundary_clicked();
    void on_btn_CreateBoundary_clicked();
    void on_actionShow_Hide_Note_Coordinate_triggered();
    void on_actionShow_Hide_Center_Coordinate_triggered();
    void on_actionXYZ_triggered();
    void on_actionZ2_triggered();
    void on_actionY2_triggered();
    void on_actionX2_triggered();
    void on_actionZ_triggered();
    void on_actionY_triggered();
    void on_actionX_triggered();
    void on_btn_ViewMesh_clicked();    void on_btn_DeleteSurface_clicked();
    void on_actionSave_triggered();
    void on_btn_RefineLayer_clicked();
    void on_btn_GeoDefineBouding_clicked();
    void on_btn_MeshRefineVolume_clicked();
    void on_btn_GenerateLocation_clicked();
    void on_btn_MeshRefineSurface_clicked();
    void on_btn_RefineBase_clicked();
    void on_btn_MeshLayer_clicked();
    void on_btn_MeshRefinement_clicked();
    void on_btn_Boundary_clicked();
    void on_tb_boundary_clicked(QModelIndex index);
    void on_txt_Max_Z_Bounding_editingFinished();
    void on_txt_Max_Y_Bounding_editingFinished();
    void on_txt_Max_X_Bounding_editingFinished();
    void on_txt_Min_Z_Bounding_editingFinished();
    void on_txt_Min_Y_Bounding_editingFinished();
    void on_txt_Min_X_Bounding_editingFinished();
    void on_btn_Browse_clicked();
    void on_btn_MeshSurfaceGeneral_clicked();
    void on_btn_MeshVolume_clicked();
    void on_btn_MeshSurface_clicked();
    void on_btn_MeshBounding_clicked();
    void on_btn_CreateMesh_clicked();
    void on_cb_BoundingGeometryType_currentIndexChanged(QString );
    void on_cb_BoundingType_currentIndexChanged(QString );
    void on_cb_SurfaceType_currentIndexChanged(QString );
    void on_btn_Surface_clicked();
    void on_btn_Bounding_clicked();
    void on_btn_Generate_clicked();
    void on_btn_Mesh_clicked();
    void on_btn_Geometry_clicked();
    void Thread_Changed(QString value);
    void on_actionQuit_triggered();
    void on_tb_boundary_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous);
    void on_tb_MeshSurface_itemSelectionChanged();
    //void on_tb_boundary_itemSelectionChanged();
    void on_actionClose_triggered();
    void on_actionCapture_triggered();
    void on_cb_GeometryNewMesh_currentIndexChanged(const QString &arg1);
    void on_btn_GeoDefineNew_clicked();
    void on_rbn_Volume_clicked(bool checked);
    void on_rbn_Cellzone_clicked(bool checked);
    void on_rbn_Surface_clicked(bool checked);
};

#endif // MAINWINDOW_H
