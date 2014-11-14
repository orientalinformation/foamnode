#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mesh = new DMesh();
    ui->layout_Mesh->addWidget(mesh);
    ui->txt_Log->setMaximumBlockCount(300);
    LoadControlItems();
    LoadControlsVisible();
    LoadLocationInMesh();

    ui->tb_MeshRefineAroundSurface->horizontalHeader()->setStretchLastSection(true);

    SetButtonDefault();
    CheckLicense();
}
void MainWindow::CheckLicense()
{
    if(QFile::exists("Data/.Licence.dat"))//QFile::exists("Data/.Licence.dat")
    {
        clsEncryption license;
        if(!license.checkKey("Data/.Licence.dat"))
        {
            SetButtonEnable(false);
            SetButtonDisplayEnable(false);
            QMessageBox a("License","License has expired...!",QMessageBox::Warning,QMessageBox::Yes,QMessageBox::Ok,QMessageBox::Abort);
            a.setButtonText(QMessageBox::Ok,"Buy License");
            a.setButtonText(QMessageBox::Abort,"Register");
            a.setButtonText(QMessageBox::Yes,"Trial");
            a.addButton(QMessageBox::Cancel);
            a.exec();
            if(a.result() == QMessageBox::Ok)
            {
                SendMail *sendMail;
                sendMail = new SendMail();
                sendMail->SetMode(true);
                sendMail->exec();
            }
            if(a.result() == QMessageBox::Abort)
            {
                RegisterDialog *rg;
                rg = new RegisterDialog();
                rg->exec();
                if(rg->result())
                {
                    ui->actionOpen->setEnabled(true);
                }
            }
            if(a.result() == QMessageBox::Yes)
            {
                SendMail *sendMail;
                sendMail = new SendMail();
                sendMail->SetMode(false);
                sendMail->exec();
            }
        }
        else
        {
            SetButtonEnable(true);
            SetButtonDisplayEnable(true);
        }
    }
    else
    {
        SetButtonEnable(false);
        SetButtonDisplayEnable(false);
        QMessageBox a("License","Please contact DFM-Engineering for license key...!",QMessageBox::Warning,QMessageBox::Yes,QMessageBox::Ok,QMessageBox::Abort);
        a.setButtonText(QMessageBox::Ok,"Buy License");
        a.setButtonText(QMessageBox::Yes,"Trial");
        a.setButtonText(QMessageBox::Abort,"Register");
        a.addButton(QMessageBox::Cancel);
        a.exec();
        if(a.result() == QMessageBox::Ok)
        {
            SendMail *sendMail;
            sendMail = new SendMail();
            sendMail->SetMode(true);
            sendMail->exec();
        }
        if(a.result() == QMessageBox::Abort)
        {
            RegisterDialog *rg;
            rg = new RegisterDialog();
            rg->exec();
            if(rg->result())
            {
                ui->actionOpen->setEnabled(true);
            }
        }
        if(a.result() == QMessageBox::Yes)
        {
            SendMail *sendMail;
            sendMail = new SendMail();
            sendMail->SetMode(false);
            sendMail->exec();
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////  Default Value  ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::LoadLocationInMesh()
{
    ui->txt_X_Location->setText(QString::number(mesh->snappyd->locationInMesh.x));
    ui->txt_Y_Location->setText(QString::number(mesh->snappyd->locationInMesh.y));
    ui->txt_Z_Location->setText(QString::number(mesh->snappyd->locationInMesh.z));
}
void MainWindow::LoadControlItems()
{
    LoadGeometryControlItems();
    LoadMeshControlItems();
}
void MainWindow::LoadGeometryControlItems()
{
    ui->cb_BoundingType->addItem("Automatic");
    ui->cb_BoundingType->addItem("Distance");
    ui->cb_BoundingType->addItem("Define");
    ui->cb_SurfaceType->addItem("Box");
    ui->cb_SurfaceType->addItem("Cylinder");
    ui->cb_SurfaceType->addItem("Sphere");
}
void MainWindow::LoadBoundaryControlItems()
{
    ui->cb_BoundaryType->clear();
    ui->cb_BoundaryType->addItem("---Type---");
    ui->cb_BoundaryType->addItem("patch");
    ui->cb_BoundaryType->addItem("empty");
    ui->cb_BoundaryType->addItem("symmetryPlane");
    ui->cb_BoundaryType->addItem("wall");

    ui->tb_MeshSurface->clear();
    ui->tb_MeshSurface->setColumnCount(1);
    ui->tb_MeshSurface->setColumnWidth(0,300);
    int n = listSurfaces.size();
    ui->tb_MeshSurface->setRowCount(n);
    ui->tb_MeshSurface->setEditTriggers(QTableWidget::NoEditTriggers);
    for(int i = 0; i < listSurfaces.size(); i++)
    {
        QTableWidgetItem *temp = new QTableWidgetItem(listSurfaces[i]);
        ui->tb_MeshSurface->setItem(i,0,temp);
    }
    ui->tb_MeshBoundary->clear();
    ui->tb_MeshBoundary->setColumnCount(1);
    ui->tb_MeshBoundary->setColumnWidth(0,300);
    ui->tb_MeshBoundary->setRowCount(mesh->patchDict->boundaries.size());
    ui->tb_MeshBoundary->setEditTriggers(QTableWidget::NoEditTriggers);
    for(int i = 0; i < mesh->patchDict->boundaries.size(); i++)
    {
        QTableWidgetItem *temp = new QTableWidgetItem(mesh->patchDict->boundaries[i].name);
        ui->tb_MeshBoundary->setItem(i,0,temp);
    }
}

void MainWindow::LoadMeshControlItems()
{
    ui->cb_MeshVolumeMode->addItem("---Mode---");
    ui->cb_MeshVolumeMode->addItem("inside");
    ui->cb_MeshVolumeMode->addItem("outside");
    ui->cb_VolumeType->addItem("Box");
    ui->cb_VolumeType->addItem("Cylinder");
    ui->cb_VolumeType->addItem("Sphere");
}

void MainWindow::LoadControlsVisible()
{
    LoadGeometryControlsVisible();
    LoadMeshControlsVisible();
    LoadBoundaryControlsVisible();
    LoadGenerateControlsVisible();

    ui->btn_Geometry->show();
    ui->btn_Mesh->show();
    ui->btn_Boundary->show();
    ui->btn_Generate->show();
}
void MainWindow::LoadGeometryControlsVisible()
{
    ui->btn_Geometry->hide();
    ui->frame_Geometry->hide();
    ui->frame_GeometrySurface->hide();
//    ui->frame_Import->hide();
//    ui->frame_Define->hide();
//    ui->frame_DefineBox->hide();
    ui->frame_DefineSurfaceCylinder->hide();

    ui->frame_GeometryBoundingCylinder->hide();

    ui->frame_Mesh->hide();

    ui->frame_GeometryBounding->hide();
    ui->frame_Generate->hide();
}
void MainWindow::LoadMeshControlsVisible()
{
    ui->btn_Mesh->hide();
    ui->frame_MeshBounding->hide();
    ui->frame_MeshSurface->hide();
    ui->frame_MeshVolume->hide();
    ui->groupBox_MeshVolumeCellZone->hide();
    ui->frame_MeshRefinement->hide();
    ui->frame_MeshLayer->hide();
    ui->frame_DefineVolumeCylinder->hide();
    ui->frame_DefineVolumeSphere->hide();
}
void MainWindow::LoadBoundaryControlsVisible()
{
    ui->btn_Boundary->hide();
    ui->frame_Boundary->hide();
}
void MainWindow::LoadGenerateControlsVisible()
{
    ui->btn_Generate->hide();
    ui->frame_GeometryBounding->hide();
}

bool MainWindow::AddFaceToList(QString name)
{
    int i = listFaces.indexOf(name);
    if(i > -1)
    {
        ui->tb_boundary->showRow(i);
        loadData();
        return false;
    }
    listFaces.append(name);
    ui->tb_boundary->setColumnCount(1);
    ui->tb_boundary->setColumnWidth(0,300);
    ui->tb_boundary->setRowCount(listFaces.size());
    QTableWidgetItem *temp = new QTableWidgetItem(name);
    ui->tb_boundary->setItem(listFaces.size()-1,0,temp);
    ui->tb_boundary->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->tb_boundary->setCurrentCell(0,0);
    //loadData();
    return true;
}
void MainWindow::Thread_Changed(QString value)
{
    value = value.remove("\n");
    ui->txt_Log->appendPlainText(value);
}
void MainWindow::Remove_All_Face()
{
    for(int i= ui->tb_boundary->rowCount()-1;i>=0;i--)
    {
        ui->tb_boundary->hideRow(i);
    }
    //listface.clear();
}

void MainWindow::on_btn_Geometry_clicked()
{
    if(ui->frame_Geometry->isVisible())
    {
        LoadControlsVisible();
    }
    else
    {
        ui->btn_Mesh->hide();
        ui->btn_Boundary->hide();
        ui->btn_Generate->hide();
        ui->frame_Geometry->show();
        ui->btn_DeleteSurface->setEnabled(true);
        Remove_All_Face();
    }
}
void MainWindow::on_btn_Mesh_clicked()
{
    if(ui->frame_Mesh->isVisible())
    {
        LoadControlsVisible();
    }
    else
    {
        ui->btn_Geometry->hide();
        ui->btn_Boundary->hide();
        ui->btn_Generate->hide();
        ui->frame_Mesh->show();
        ui->btn_DeleteSurface->setEnabled(true);
        if(mesh->blockd->vertice.n==0)
        {
            ui->btn_MeshBounding->setEnabled(false);
            ui->btn_MeshLayer->setEnabled(false);
        }
        else
        {
            ui->btn_MeshBounding->setEnabled(true);
            ui->btn_MeshLayer->setEnabled(true);
        }
        if(mesh->snappyd->gBox.n !=0 || mesh->snappyd->gCylin.n !=0
           || mesh->snappyd->gSphere.n !=0 || mesh->snappyd->gUserDefine.n !=0)
        {
            ui->btn_MeshSurface->setEnabled(true);
            ui->btn_MeshVolume->setEnabled(true);
        }
        else if(mesh->snappyd->gBoxRegion.n != 0)
        {
            ui->btn_MeshSurface->setEnabled(false);
            ui->btn_MeshVolume->setEnabled(true);
        }
        else
        {
            ui->btn_MeshSurface->setEnabled(false);
            ui->btn_MeshVolume->setEnabled(false);
        }
        Remove_All_Face();
    }
}
void MainWindow::on_btn_Generate_clicked()
{
    if(ui->frame_Generate->isVisible())
    {
        LoadControlsVisible();
    }
    else
    {
        ui->btn_Geometry->hide();
        ui->btn_Mesh->hide();
        ui->btn_Boundary->hide();
        ui->frame_Generate->show();
        ui->btn_DeleteSurface->setEnabled(true);
        Remove_All_Face();
    }
}
void MainWindow::on_btn_Boundary_clicked()
{
    if(ui->frame_Boundary->isVisible())
    {
        LoadControlsVisible();
    }
    else
    {
        ui->btn_Geometry->hide();
        ui->btn_Mesh->hide();
        ui->btn_Generate->hide();
        ui->frame_Boundary->show();
        LoadBoundaryControlItems();
        ui->btn_DeleteSurface->setEnabled(true);
        Remove_All_Face();
        for(int i=0; i< mesh->snappyd->gBox.n; i++)
        {
            AddFaceToList(mesh->snappyd->gBox.boxes[i].name);
        }
        for(int i=0; i< mesh->snappyd->gCylin.n; i++)
        {
            AddFaceToList(mesh->snappyd->gCylin.cylins[i].name);
        }
        for(int i=0; i< mesh->snappyd->gSphere.n; i++)
        {
            AddFaceToList(mesh->snappyd->gSphere.sphere[i].name);
        }
        for(int i=0; i< mesh->snappyd->gUserDefine.n; i++)
        {
            AddFaceToList(mesh->snappyd->gUserDefine.user_Defines[i].name);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////   Utility   ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

bool MainWindow::CheckNameValid(QString value)
{
    if(value =="" || value.contains(" "))
    {
        QMessageBox::critical(this,"Error","Please input a valid name!");
        return false;
    }
    //check name valid
    for(int i=0; i< mesh->snappyd->gBox.n; i++)
    {
        if(value==  mesh->snappyd->gBox.boxes[i].name)
        {
            QMessageBox::critical(this,"Error","This name is already existed, this will be replaced by new values (Coming soon)...!");
            return false;
        }
    }
    for(int i=0; i< mesh->snappyd->gBoxRegion.n; i++)
    {
        if(value==  mesh->snappyd->gBoxRegion.boxes[i].name)
        {
            QMessageBox::critical(this,"Error","This name is already existed, this will be replaced by new values (Coming soon)...!");
            return false;
        }
    }
    for(int i=0; i< mesh->snappyd->gCylin.n; i++)
    {
        if(value==  mesh->snappyd->gCylin.cylins[i].name)
        {
            QMessageBox::critical(this,"Error","This name is already existed, this will be replaced by new values (Coming soon)...!");
            return false;
        }
    }
    for(int i=0; i< mesh->snappyd->gCylinRegion.n; i++)
    {
        if(value==  mesh->snappyd->gCylinRegion.cylins[i].name)
        {
            QMessageBox::critical(this,"Error","This name is already existed, this will be replaced by new values (Coming soon)...!");
            return false;
        }
    }
    for(int i=0; i< mesh->snappyd->gSphere.n; i++)
    {
        if(value==  mesh->snappyd->gSphere.sphere[i].name)
        {
            QMessageBox::critical(this,"Error","This name is already existed, this will be replaced by new values (Coming soon)...!");
            return false;
        }
    }
    for(int i=0; i< mesh->snappyd->gSphereRegion.n; i++)
    {
        if(value==  mesh->snappyd->gSphereRegion.sphere[i].name)
        {
            QMessageBox::critical(this,"Error","This name is already existed, this will be replaced by new values (Coming soon)...!");
            return false;
        }
    }
    for(int i=0; i< mesh->snappyd->gUserDefine.n; i++)
    {
        if(value==  mesh->snappyd->gUserDefine.user_Defines[i].name)
        {
            QMessageBox::critical(this,"Error","This name is already existed, this will be replaced by new values (Coming soon)...!");
            return false;
        }
    }
    foreach(Boundary b,mesh->patchDict->boundaries)
    {
        if(b.name == value)
        {
            QMessageBox::critical(this,"Error","This name is already existed in boundaries...!");
            return false;
        }
    }
    return true;
}

bool MainWindow::SetLocation()
{
    bool a, b, c;
    float x, y, z;
    x = ui->txt_X_Location->text().toFloat(&a);
    y = ui->txt_Y_Location->text().toFloat(&b);
    z = ui->txt_Z_Location->text().toFloat(&c);
    if(a && b && c)
    {
        mesh->snappyd->locationInMesh.x=x;
        mesh->snappyd->locationInMesh.y=y;
        mesh->snappyd->locationInMesh.z=z;
        mesh->updateGL();
    }
    else
    {
        QMessageBox::critical(this,"Error","Please input all values in this form...!");        
        return false;
    }

    return true;
}
bool MainWindow::CheckAndSaveBoundingBoxInput()
{
    bool a,b,c,d,e,f;
    float x1 = ui->txt_Min_X_Bounding->text().toFloat(&a);
    float x2 = ui->txt_Max_X_Bounding->text().toFloat(&b);
    float y1 = ui->txt_Min_Y_Bounding->text().toFloat(&c);
    float y2 = ui->txt_Max_Y_Bounding->text().toFloat(&d);
    float z1 = ui->txt_Min_Z_Bounding->text().toFloat(&e);
    float z2 = ui->txt_Max_Z_Bounding->text().toFloat(&f);
    if(a&b&c&d&e&f)
    {
            mesh->blockd->vertice.v.resize(8);
            mesh->blockd->vertice.n =8;
            mesh->blockd->vertice.v[0].x=x1;
            mesh->blockd->vertice.v[0].y=y1;
            mesh->blockd->vertice.v[0].z=z1;

            mesh->blockd->vertice.v[1].x=x2;
            mesh->blockd->vertice.v[1].y=y1;
            mesh->blockd->vertice.v[1].z=z1;

            mesh->blockd->vertice.v[2].x=x2;
            mesh->blockd->vertice.v[2].y=y2;
            mesh->blockd->vertice.v[2].z=z1;

            mesh->blockd->vertice.v[3].x=x1;
            mesh->blockd->vertice.v[3].y=y2;
            mesh->blockd->vertice.v[3].z=z1;

            mesh->blockd->vertice.v[4].x=x1;
            mesh->blockd->vertice.v[4].y=y1;
            mesh->blockd->vertice.v[4].z=z2;

            mesh->blockd->vertice.v[5].x=x2;
            mesh->blockd->vertice.v[5].y=y1;
            mesh->blockd->vertice.v[5].z=z2;

            mesh->blockd->vertice.v[6].x=x2;
            mesh->blockd->vertice.v[6].y=y2;
            mesh->blockd->vertice.v[6].z=z2;

            mesh->blockd->vertice.v[7].x=x1;
            mesh->blockd->vertice.v[7].y=y2;
            mesh->blockd->vertice.v[7].z=z2;

            mesh->blockd->boundMesh.n = 6;
            mesh->blockd->boundMesh.bounsType.resize(6);

                AddFaceToList("MaxY");
                mesh->blockd->boundMesh.bounsType[0].name = "MaxY";
                mesh->blockd->boundMesh.bounsType[0].type = "wall";
                mesh->blockd->boundMesh.bounsType[0].n = 1;
                mesh->blockd->boundMesh.bounsType[0].faces.resize(1);
                mesh->blockd->boundMesh.bounsType[0].faces[0].points.resize(4);
                mesh->blockd->boundMesh.bounsType[0].faces[0].n = 4;
                mesh->blockd->boundMesh.bounsType[0].faces[0].points[0] = 3;
                mesh->blockd->boundMesh.bounsType[0].faces[0].points[1] = 7;
                mesh->blockd->boundMesh.bounsType[0].faces[0].points[2] = 6;
                mesh->blockd->boundMesh.bounsType[0].faces[0].points[3] = 2;

            AddFaceToList("MinY");
            mesh->blockd->boundMesh.bounsType[1].name = "MinY";
            mesh->blockd->boundMesh.bounsType[1].type = "wall";
            mesh->blockd->boundMesh.bounsType[1].n = 1;
            mesh->blockd->boundMesh.bounsType[1].faces.resize(1);
            mesh->blockd->boundMesh.bounsType[1].faces[0].points.resize(4);
            mesh->blockd->boundMesh.bounsType[1].faces[0].n = 4;
            mesh->blockd->boundMesh.bounsType[1].faces[0].points[0] = 1;
            mesh->blockd->boundMesh.bounsType[1].faces[0].points[1] = 5;
            mesh->blockd->boundMesh.bounsType[1].faces[0].points[2] = 4;
            mesh->blockd->boundMesh.bounsType[1].faces[0].points[3] = 0;

                AddFaceToList("MinZ");
                mesh->blockd->boundMesh.bounsType[2].name = "MinZ";
                mesh->blockd->boundMesh.bounsType[2].type = "wall";
                mesh->blockd->boundMesh.bounsType[2].n = 1;
                mesh->blockd->boundMesh.bounsType[2].faces.resize(1);
                mesh->blockd->boundMesh.bounsType[2].faces[0].points.resize(4);
                mesh->blockd->boundMesh.bounsType[2].faces[0].n = 4;
                mesh->blockd->boundMesh.bounsType[2].faces[0].points[0] = 4;
                mesh->blockd->boundMesh.bounsType[2].faces[0].points[1] = 5;
                mesh->blockd->boundMesh.bounsType[2].faces[0].points[2] = 6;
                mesh->blockd->boundMesh.bounsType[2].faces[0].points[3] = 7;

            AddFaceToList("MaxZ");
            mesh->blockd->boundMesh.bounsType[3].name = "MaxZ";
            mesh->blockd->boundMesh.bounsType[3].type = "wall";
            mesh->blockd->boundMesh.bounsType[3].n = 1;
            mesh->blockd->boundMesh.bounsType[3].faces.resize(1);;
            mesh->blockd->boundMesh.bounsType[3].faces[0].n = 4;
            mesh->blockd->boundMesh.bounsType[3].faces[0].points.resize(4);
            mesh->blockd->boundMesh.bounsType[3].faces[0].points[0] = 0;
            mesh->blockd->boundMesh.bounsType[3].faces[0].points[1] = 3;
            mesh->blockd->boundMesh.bounsType[3].faces[0].points[2] = 2;
            mesh->blockd->boundMesh.bounsType[3].faces[0].points[3] = 1;

                AddFaceToList("MinX");
                mesh->blockd->boundMesh.bounsType[4].name = "MinX";
                mesh->blockd->boundMesh.bounsType[4].type = "patch";
                mesh->blockd->boundMesh.bounsType[4].n = 1;
                mesh->blockd->boundMesh.bounsType[4].faces.resize(1);;
                mesh->blockd->boundMesh.bounsType[4].faces[0].n = 4;
                mesh->blockd->boundMesh.bounsType[4].faces[0].points.resize(4);
                mesh->blockd->boundMesh.bounsType[4].faces[0].points[0] = 0;
                mesh->blockd->boundMesh.bounsType[4].faces[0].points[1] = 4;
                mesh->blockd->boundMesh.bounsType[4].faces[0].points[2] = 7;
                mesh->blockd->boundMesh.bounsType[4].faces[0].points[3] = 3;

            AddFaceToList("MaxX");
            mesh->blockd->boundMesh.bounsType[5].name = "MaxX";
            mesh->blockd->boundMesh.bounsType[5].type = "patch";
            mesh->blockd->boundMesh.bounsType[5].n = 1;
            mesh->blockd->boundMesh.bounsType[5].faces.resize(1);
            mesh->blockd->boundMesh.bounsType[5].faces[0].n = 4;
            mesh->blockd->boundMesh.bounsType[5].faces[0].points.resize(4);
            mesh->blockd->boundMesh.bounsType[5].faces[0].points[0] = 2;
            mesh->blockd->boundMesh.bounsType[5].faces[0].points[1] = 6;
            mesh->blockd->boundMesh.bounsType[5].faces[0].points[2] = 5;
            mesh->blockd->boundMesh.bounsType[5].faces[0].points[3] = 1;
            mesh->blockd->isAutomatic = false;
            mesh->updateGL();

            //Save list surface
            listSurfaces.append("MaxY");
            listSurfaces.append("MinY");
            listSurfaces.append("MinZ");
            listSurfaces.append("MaxZ");
            listSurfaces.append("MinX");
            listSurfaces.append("MaxX");
            listSurfaces.removeDuplicates();
    }
    else
    {
        QMessageBox::critical(this,"Error","Please input all values in this form...!");
        return false;
    }
    return true;
}
bool MainWindow::CheckAndSaveBoundingDistance()
{
    bool a,b,c,d,e,f;
    float x1 = ui->txt_DistanceMinX->text().toFloat(&a);
    float x2 = ui->txt_DistanceMaxX->text().toFloat(&b);
    float y1 = ui->txt_DistanceMinY->text().toFloat(&c);
    float y2 = ui->txt_DistanceMaxY->text().toFloat(&d);
    float z1 = ui->txt_DistanceMinZ->text().toFloat(&e);
    float z2 = ui->txt_DistanceMaxZ->text().toFloat(&f);
    if(a&b&c&d&e&f)
    {
         return SetBoundingDistance(x1,x2,y1,y2,z1,z2);
    }
    else
    {
        QMessageBox::critical(this,"Error","Please input all values in this form...!");
        return false;
    }    
    return true;
}

bool MainWindow::AddSurfaceRegionBox()
{
    if(ui->tb_boundary->currentRow() == -1 || flag_Item_Face_Click ==false)
        return false;
    bool a,b;
    float min = ui->txt_Level_Min_Surface_Refine->text().toFloat(&a);
    float max = ui->txt_Level_Max_Surface_Refine->text().toFloat(&b);

    if(a== false || b==false)
    {
        QMessageBox::critical(this,"Error","Please input all values in this form...!");
        return false;
    }
    //snappy box
    for(int i = 0; i< mesh->snappyd->gBox.refi_Sur.n; i++)
    {
        //update if exist
        if(mesh->snappyd->gBox.refi_Sur.surfaces[i].name == ui->tb_boundary->currentItem()->text())
        {
            mesh->snappyd->gBox.refi_Sur.surfaces[i].lv1= min;
            mesh->snappyd->gBox.refi_Sur.surfaces[i].lv2= max;
            return true;
        }
    }
    //snappy cylinder
    for(int i = 0; i< mesh->snappyd->gCylin.refi_Sur.n; i++)
    {
        //update if exist
        if(mesh->snappyd->gCylin.refi_Sur.surfaces[i].name == ui->tb_boundary->currentItem()->text())
        {
            mesh->snappyd->gCylin.refi_Sur.surfaces[i].lv1= min;
            mesh->snappyd->gCylin.refi_Sur.surfaces[i].lv2= max;
            return true;
        }
    }
    //snappy sphere
    for(int i = 0; i< mesh->snappyd->gSphere.refi_Sur.n; i++)
    {
        //update if exist
        if(mesh->snappyd->gSphere.refi_Sur.surfaces[i].name == ui->tb_boundary->currentItem()->text())
        {
            mesh->snappyd->gSphere.refi_Sur.surfaces[i].lv1= min;
            mesh->snappyd->gSphere.refi_Sur.surfaces[i].lv2= max;
            return true;
        }
    }
}

bool MainWindow::AddUserDefine()
{
    if(flag_Item_Face_Click==true)
    {
        bool a,b;
        float lv1 = ui->txt_Level_Min_Surface_Refine->text().toFloat(&a);
        float lv2 = ui->txt_Level_Max_Surface_Refine->text().toFloat(&b);
        if(a && b)
        {
            GeomeUserDefine *gUserDefine = &mesh->snappyd->gUserDefine;
            for(int i = 0; i< gUserDefine->refi_Sur.n; i++)
            {
                QString currentSurface = gUserDefine->refi_Sur.surfaces[i].name;
                if(currentSurface == ui->tb_boundary->currentItem()->text())
                {
                    gUserDefine->refi_Sur.surfaces[i].lv1= lv1;
                    gUserDefine->refi_Sur.surfaces[i].lv2 =lv2;                    
                }
            }
        }
        else
        {
            QMessageBox::critical(this,"Error","Please input all values in this form...!");
            return false;
        }
    }    
    return true;
}

bool MainWindow::SetBoundingDistance(float dMinX, float dMaxX, float dMinY, float dMaxY, float dMinZ, float dMaxZ)
{
    float x1,x2,y1,y2,z1,z2;
    if(ui->cb_BoundingType->currentText() == "Automatic")
    {
        x1 = mesh->snappyd->defaultBounding.minBouDef.x;
        x2 = mesh->snappyd->defaultBounding.maxBouDef.x;
        y1 = mesh->snappyd->defaultBounding.minBouDef.y;
        y2 = mesh->snappyd->defaultBounding.maxBouDef.y;
        z1 = mesh->snappyd->defaultBounding.minBouDef.z;
        z2 = mesh->snappyd->defaultBounding.maxBouDef.z;
        if(mesh->snappyd->list_Surface_Min_Max.size()==0)
        {
            x1 =0;
            x2 =0;
            y1 =0;
            y2 =0;
            z1 =0;
            z2 =0;
        }
    }
    else
    {
        x1 = mesh->snappyd->defaultBounding.minBouDef.x - dMinX;
        x2 = mesh->snappyd->defaultBounding.maxBouDef.x + dMaxX;
        y1 = mesh->snappyd->defaultBounding.minBouDef.y - dMinY;
        y2 = mesh->snappyd->defaultBounding.maxBouDef.y + dMaxY;
        z1 = mesh->snappyd->defaultBounding.minBouDef.z - dMinZ;
        z2 = mesh->snappyd->defaultBounding.maxBouDef.z + dMaxZ;
        if(mesh->snappyd->list_Surface_Min_Max.size()==0)
        {
            x1 = -dMinX;
            x2 =dMaxX;
            y1 = -dMinY;
            y2 =dMaxY;
            z1 = -dMinZ;
            z2 =dMaxZ;
        }
    }

    mesh->blockd->vertice.v.resize(8);
    mesh->blockd->vertice.n =8;
    mesh->blockd->vertice.v[0].x=x1;
    mesh->blockd->vertice.v[0].y=y1;
    mesh->blockd->vertice.v[0].z=z1;

    mesh->blockd->vertice.v[1].x=x2;
    mesh->blockd->vertice.v[1].y=y1;
    mesh->blockd->vertice.v[1].z=z1;

    mesh->blockd->vertice.v[2].x=x2;
    mesh->blockd->vertice.v[2].y=y2;
    mesh->blockd->vertice.v[2].z=z1;

    mesh->blockd->vertice.v[3].x=x1;
    mesh->blockd->vertice.v[3].y=y2;
    mesh->blockd->vertice.v[3].z=z1;

    mesh->blockd->vertice.v[4].x=x1;
    mesh->blockd->vertice.v[4].y=y1;
    mesh->blockd->vertice.v[4].z=z2;

    mesh->blockd->vertice.v[5].x=x2;
    mesh->blockd->vertice.v[5].y=y1;
    mesh->blockd->vertice.v[5].z=z2;

    mesh->blockd->vertice.v[6].x=x2;
    mesh->blockd->vertice.v[6].y=y2;
    mesh->blockd->vertice.v[6].z=z2;

    mesh->blockd->vertice.v[7].x=x1;
    mesh->blockd->vertice.v[7].y=y2;
    mesh->blockd->vertice.v[7].z=z2;

    mesh->blockd->boundMesh.n = 6;
    mesh->blockd->boundMesh.bounsType.resize(6);
        mesh->blockd->boundMesh.bounsType[0].name = "MinY";
        mesh->blockd->boundMesh.bounsType[0].type = "wall";
        mesh->blockd->boundMesh.bounsType[0].n = 1;
        mesh->blockd->boundMesh.bounsType[0].faces.resize(1);
        mesh->blockd->boundMesh.bounsType[0].faces[0].points.resize(4);
        mesh->blockd->boundMesh.bounsType[0].faces[0].n = 4;
        mesh->blockd->boundMesh.bounsType[0].faces[0].points[0] = 3;
        mesh->blockd->boundMesh.bounsType[0].faces[0].points[1] = 7;
        mesh->blockd->boundMesh.bounsType[0].faces[0].points[2] = 6;
        mesh->blockd->boundMesh.bounsType[0].faces[0].points[3] = 2;


        mesh->blockd->boundMesh.bounsType[1].name = "MaxY";
        mesh->blockd->boundMesh.bounsType[1].type = "wall";
        mesh->blockd->boundMesh.bounsType[1].n = 1;
        mesh->blockd->boundMesh.bounsType[1].faces.resize(1);
        mesh->blockd->boundMesh.bounsType[1].faces[0].points.resize(4);
        mesh->blockd->boundMesh.bounsType[1].faces[0].n = 4;
        mesh->blockd->boundMesh.bounsType[1].faces[0].points[0] = 1;
        mesh->blockd->boundMesh.bounsType[1].faces[0].points[1] = 5;
        mesh->blockd->boundMesh.bounsType[1].faces[0].points[2] = 4;
        mesh->blockd->boundMesh.bounsType[1].faces[0].points[3] = 0;

    mesh->blockd->boundMesh.bounsType[2].name = "MinZ";
    mesh->blockd->boundMesh.bounsType[2].type = "wall";
    mesh->blockd->boundMesh.bounsType[2].n = 1;
    mesh->blockd->boundMesh.bounsType[2].faces.resize(1);
    mesh->blockd->boundMesh.bounsType[2].faces[0].points.resize(4);
    mesh->blockd->boundMesh.bounsType[2].faces[0].n = 4;
    mesh->blockd->boundMesh.bounsType[2].faces[0].points[0] = 4;
    mesh->blockd->boundMesh.bounsType[2].faces[0].points[1] = 5;
    mesh->blockd->boundMesh.bounsType[2].faces[0].points[2] = 6;
    mesh->blockd->boundMesh.bounsType[2].faces[0].points[3] = 7;

        mesh->blockd->boundMesh.bounsType[3].name = "MaxZ";
        mesh->blockd->boundMesh.bounsType[3].type = "wall";
        mesh->blockd->boundMesh.bounsType[3].n = 1;
        mesh->blockd->boundMesh.bounsType[3].faces.resize(1);;
        mesh->blockd->boundMesh.bounsType[3].faces[0].n = 4;
        mesh->blockd->boundMesh.bounsType[3].faces[0].points.resize(4);
        mesh->blockd->boundMesh.bounsType[3].faces[0].points[0] = 0;
        mesh->blockd->boundMesh.bounsType[3].faces[0].points[1] = 3;
        mesh->blockd->boundMesh.bounsType[3].faces[0].points[2] = 2;
        mesh->blockd->boundMesh.bounsType[3].faces[0].points[3] = 1;

    mesh->blockd->boundMesh.bounsType[4].name = "MinX";
    mesh->blockd->boundMesh.bounsType[4].type = "wall";
    mesh->blockd->boundMesh.bounsType[4].n = 1;
    mesh->blockd->boundMesh.bounsType[4].faces.resize(1);;
    mesh->blockd->boundMesh.bounsType[4].faces[0].n = 4;
    mesh->blockd->boundMesh.bounsType[4].faces[0].points.resize(4);
    mesh->blockd->boundMesh.bounsType[4].faces[0].points[0] = 0;
    mesh->blockd->boundMesh.bounsType[4].faces[0].points[1] = 4;
    mesh->blockd->boundMesh.bounsType[4].faces[0].points[2] = 7;
    mesh->blockd->boundMesh.bounsType[4].faces[0].points[3] = 3;

        mesh->blockd->boundMesh.bounsType[5].name = "MaxX";
        mesh->blockd->boundMesh.bounsType[5].type = "wall";
        mesh->blockd->boundMesh.bounsType[5].n = 1;
        mesh->blockd->boundMesh.bounsType[5].faces.resize(1);
        mesh->blockd->boundMesh.bounsType[5].faces[0].n = 4;
        mesh->blockd->boundMesh.bounsType[5].faces[0].points.resize(4);
        mesh->blockd->boundMesh.bounsType[5].faces[0].points[0] = 2;
        mesh->blockd->boundMesh.bounsType[5].faces[0].points[1] = 6;
        mesh->blockd->boundMesh.bounsType[5].faces[0].points[2] = 5;
        mesh->blockd->boundMesh.bounsType[5].faces[0].points[3] = 1;
    if(dMaxX > 0 || dMaxY > 0 || dMaxZ > 0 || dMinX > 0 || dMinY > 0 || dMinZ > 0)
    {
        AddFaceToList("MaxY");
        AddFaceToList("MinY");
        AddFaceToList("MinZ");
        AddFaceToList("MaxZ");
        AddFaceToList("MinX");
        AddFaceToList("MaxX");
        mesh->blockd->isAutomatic = false;
        //Save list surface
        listSurfaces.append("MaxY");
        listSurfaces.append("MinY");
        listSurfaces.append("MinZ");
        listSurfaces.append("MaxZ");
        listSurfaces.append("MinX");
        listSurfaces.append("MaxX");
        listSurfaces.removeDuplicates();
    }
    else
    {
        listSurfaces.removeOne("MaxY");
        listSurfaces.removeOne("MinY");
        listSurfaces.removeOne("MinZ");
        listSurfaces.removeOne("MaxZ");
        listSurfaces.removeOne("MinX");
        listSurfaces.removeOne("MaxX");
    }
    listSurfacesDefault = listSurfaces;
    mesh->updateGL();    
    return true;
}

void MainWindow::ResetDrawBoxs()
{
    mesh->snappyd->points.clear();
    mesh->snappyd->faces.clear();
    mesh->snappyd->facezones.clear();
    mesh->snappyd->facename.clear();
    mesh->snappyd->facetype.clear();
    for(int i=0; i < mesh->snappyd->gBox.n; i++)
    {
        GeomeBoxTypeDmesh temp = mesh->snappyd->gBox.boxes[i];
        int plus = mesh->snappyd->points.size();
        mesh->snappyd->points.resize(8+plus);
        mesh->snappyd->points[0+plus] = new  float[3];
        mesh->snappyd->points[0+plus][0] =  temp.min.x;//x1
        mesh->snappyd->points[0+plus][1] =  temp.min.y;//y1;
        mesh->snappyd->points[0+plus][2] =  temp.min.z;//z1;


        mesh->snappyd->points[1+plus] = new  float[3];
        mesh->snappyd->points[1+plus][0] = temp.max.x;//x2;
        mesh->snappyd->points[1+plus][1] = temp.min.y;//y1;
        mesh->snappyd->points[1+plus][2] = temp.min.z;//z1;

        mesh->snappyd->points[2+plus] = new  float[3];
        mesh->snappyd->points[2+plus][0] = temp.max.x;//x2;
        mesh->snappyd->points[2+plus][1] = temp.max.y;//y2;
        mesh->snappyd->points[2+plus][2] = temp.min.z;//z1;

        mesh->snappyd->points[3+plus] = new  float[3];
        mesh->snappyd->points[3+plus][0] = temp.min.x;//x1;
        mesh->snappyd->points[3+plus][1] = temp.max.y;//y2;
        mesh->snappyd->points[3+plus][2] = temp.min.z;//z1;

        mesh->snappyd->points[4+plus] = new  float[3];
        mesh->snappyd->points[4+plus][0] = temp.min.x;//x1;
        mesh->snappyd->points[4+plus][1] = temp.min.y;//y1;
        mesh->snappyd->points[4+plus][2] = temp.max.z;//z2;

        mesh->snappyd->points[5+plus] = new  float[3];
        mesh->snappyd->points[5+plus][0] = temp.max.x;//x2;
        mesh->snappyd->points[5+plus][1] = temp.min.y;//y1;
        mesh->snappyd->points[5+plus][2] = temp.max.z;//z2;

        mesh->snappyd->points[6+plus] = new  float[3];
        mesh->snappyd->points[6+plus][0] = temp.max.x;//x2;
        mesh->snappyd->points[6+plus][1] = temp.max.y;//y2;
        mesh->snappyd->points[6+plus][2] = temp.max.z;//z2;

        mesh->snappyd->points[7+plus] = new  float[3];
        mesh->snappyd->points[7+plus][0] = temp.min.x;//x1;
        mesh->snappyd->points[7+plus][1] = temp.max.y;//y2;
        mesh->snappyd->points[7+plus][2] = temp.max.z;//z2;

        int starend[2];
        starend[0] = mesh->snappyd->faces.size();

        mesh->snappyd->faces.resize(6+starend[0]);
        mesh->snappyd->faces[0+starend[0]] = new int[4];
        mesh->snappyd->faces[0+starend[0]][0] = 3 + plus;
        mesh->snappyd->faces[0+starend[0]][1] = 7 + plus;
        mesh->snappyd->faces[0+starend[0]][2] = 6 + plus;
        mesh->snappyd->faces[0+starend[0]][3] = 2 + plus;

        mesh->snappyd->faces[1+starend[0]] = new int[4];
        mesh->snappyd->faces[1+starend[0]][0] = 1 + plus;
        mesh->snappyd->faces[1+starend[0]][1] = 5 + plus;
        mesh->snappyd->faces[1+starend[0]][2] = 4 + plus;
        mesh->snappyd->faces[1+starend[0]][3] = 0 + plus;

        mesh->snappyd->faces[2+starend[0]] = new int[4];
        mesh->snappyd->faces[2+starend[0]][0] = 4 + plus;
        mesh->snappyd->faces[2+starend[0]][1] = 5 + plus;
        mesh->snappyd->faces[2+starend[0]][2] = 6 + plus;
        mesh->snappyd->faces[2+starend[0]][3] = 7 + plus;

        mesh->snappyd->faces[3+starend[0]] = new int[4];
        mesh->snappyd->faces[3+starend[0]][0] = 0 + plus;
        mesh->snappyd->faces[3+starend[0]][1] = 3 + plus;
        mesh->snappyd->faces[3+starend[0]][2] = 2 + plus;
        mesh->snappyd->faces[3+starend[0]][3] = 1 + plus;

        mesh->snappyd->faces[4+starend[0]] = new int[4];
        mesh->snappyd->faces[4+starend[0]][0] = 0 + plus;
        mesh->snappyd->faces[4+starend[0]][1] = 4 + plus;
        mesh->snappyd->faces[4+starend[0]][2] = 7 + plus;
        mesh->snappyd->faces[4+starend[0]][3] = 3 + plus;

        mesh->snappyd->faces[5+starend[0]] = new int[4];
        mesh->snappyd->faces[5+starend[0]][0] = 2 + plus;
        mesh->snappyd->faces[5+starend[0]][1] = 6 + plus;
        mesh->snappyd->faces[5+starend[0]][2] = 5 + plus;
        mesh->snappyd->faces[5+starend[0]][3] = 1 + plus;

        starend[1] = mesh->snappyd->faces.size();

        mesh->snappyd->facezones.resize( mesh->snappyd->facezones.size()+1);
        mesh->snappyd->facezones[mesh->snappyd->facezones.size()-1]= new int[2];
        mesh->snappyd->facezones[mesh->snappyd->facezones.size()-1][0]= starend[0];
        mesh->snappyd->facezones[mesh->snappyd->facezones.size()-1][1]= starend[1];
        mesh->snappyd->facetype.resize(mesh->snappyd->facetype.size() + 1);
        mesh->snappyd->facetype[mesh->snappyd->facetype.size() - 1] = 1;
        mesh->snappyd->facename.append(temp.name);
    }
    for(int i=0; i < mesh->snappyd->gBoxRegion.n; i++)
    {
        GeomeBoxTypeDmesh temp = mesh->snappyd->gBoxRegion.boxes[i];
        int plus = mesh->snappyd->points.size();
        mesh->snappyd->points.resize(8+plus);
        mesh->snappyd->points[0+plus] = new  float[3];
        mesh->snappyd->points[0+plus][0] =  temp.min.x;//x1
        mesh->snappyd->points[0+plus][1] =  temp.min.y;//y1;
        mesh->snappyd->points[0+plus][2] =  temp.min.z;//z1;


        mesh->snappyd->points[1+plus] = new  float[3];
        mesh->snappyd->points[1+plus][0] = temp.max.x;//x2;
        mesh->snappyd->points[1+plus][1] = temp.min.y;//y1;
        mesh->snappyd->points[1+plus][2] = temp.min.z;//z1;

        mesh->snappyd->points[2+plus] = new  float[3];
        mesh->snappyd->points[2+plus][0] = temp.max.x;//x2;
        mesh->snappyd->points[2+plus][1] = temp.max.y;//y2;
        mesh->snappyd->points[2+plus][2] = temp.min.z;//z1;

        mesh->snappyd->points[3+plus] = new  float[3];
        mesh->snappyd->points[3+plus][0] = temp.min.x;//x1;
        mesh->snappyd->points[3+plus][1] = temp.max.y;//y2;
        mesh->snappyd->points[3+plus][2] = temp.min.z;//z1;

        mesh->snappyd->points[4+plus] = new  float[3];
        mesh->snappyd->points[4+plus][0] = temp.min.x;//x1;
        mesh->snappyd->points[4+plus][1] = temp.min.y;//y1;
        mesh->snappyd->points[4+plus][2] = temp.max.z;//z2;

        mesh->snappyd->points[5+plus] = new  float[3];
        mesh->snappyd->points[5+plus][0] = temp.max.x;//x2;
        mesh->snappyd->points[5+plus][1] = temp.min.y;//y1;
        mesh->snappyd->points[5+plus][2] = temp.max.z;//z2;

        mesh->snappyd->points[6+plus] = new  float[3];
        mesh->snappyd->points[6+plus][0] = temp.max.x;//x2;
        mesh->snappyd->points[6+plus][1] = temp.max.y;//y2;
        mesh->snappyd->points[6+plus][2] = temp.max.z;//z2;

        mesh->snappyd->points[7+plus] = new  float[3];
        mesh->snappyd->points[7+plus][0] = temp.min.x;//x1;
        mesh->snappyd->points[7+plus][1] = temp.max.y;//y2;
        mesh->snappyd->points[7+plus][2] = temp.max.z;//z2;

        int starend[2];
        starend[0] = mesh->snappyd->faces.size();

        mesh->snappyd->faces.resize(6+starend[0]);
        mesh->snappyd->faces[0+starend[0]] = new int[4];
        mesh->snappyd->faces[0+starend[0]][0] = 3 + plus;
        mesh->snappyd->faces[0+starend[0]][1] = 7 + plus;
        mesh->snappyd->faces[0+starend[0]][2] = 6 + plus;
        mesh->snappyd->faces[0+starend[0]][3] = 2 + plus;

        mesh->snappyd->faces[1+starend[0]] = new int[4];
        mesh->snappyd->faces[1+starend[0]][0] = 1 + plus;
        mesh->snappyd->faces[1+starend[0]][1] = 5 + plus;
        mesh->snappyd->faces[1+starend[0]][2] = 4 + plus;
        mesh->snappyd->faces[1+starend[0]][3] = 0 + plus;

        mesh->snappyd->faces[2+starend[0]] = new int[4];
        mesh->snappyd->faces[2+starend[0]][0] = 4 + plus;
        mesh->snappyd->faces[2+starend[0]][1] = 5 + plus;
        mesh->snappyd->faces[2+starend[0]][2] = 6 + plus;
        mesh->snappyd->faces[2+starend[0]][3] = 7 + plus;

        mesh->snappyd->faces[3+starend[0]] = new int[4];
        mesh->snappyd->faces[3+starend[0]][0] = 0 + plus;
        mesh->snappyd->faces[3+starend[0]][1] = 3 + plus;
        mesh->snappyd->faces[3+starend[0]][2] = 2 + plus;
        mesh->snappyd->faces[3+starend[0]][3] = 1 + plus;

        mesh->snappyd->faces[4+starend[0]] = new int[4];
        mesh->snappyd->faces[4+starend[0]][0] = 0 + plus;
        mesh->snappyd->faces[4+starend[0]][1] = 4 + plus;
        mesh->snappyd->faces[4+starend[0]][2] = 7 + plus;
        mesh->snappyd->faces[4+starend[0]][3] = 3 + plus;

        mesh->snappyd->faces[5+starend[0]] = new int[4];
        mesh->snappyd->faces[5+starend[0]][0] = 2 + plus;
        mesh->snappyd->faces[5+starend[0]][1] = 6 + plus;
        mesh->snappyd->faces[5+starend[0]][2] = 5 + plus;
        mesh->snappyd->faces[5+starend[0]][3] = 1 + plus;

        starend[1] = mesh->snappyd->faces.size();

        mesh->snappyd->facezones.resize( mesh->snappyd->facezones.size()+1);
        mesh->snappyd->facezones[mesh->snappyd->facezones.size()-1]= new int[2];
        mesh->snappyd->facezones[mesh->snappyd->facezones.size()-1][0]= starend[0];
        mesh->snappyd->facezones[mesh->snappyd->facezones.size()-1][1]= starend[1];
        mesh->snappyd->facetype.resize(mesh->snappyd->facetype.size() + 1);
        mesh->snappyd->facetype[mesh->snappyd->facetype.size() - 1] = 0;
        mesh->snappyd->facename.append(temp.name);
    }
}

void MainWindow::LoadRefineDistanceSurface(QString currentSurface, int type)
{
    //Find in box
    if(type == 0)
    {
        GeomeBoxSurface *gBox = &mesh->snappyd->gBox;
        for(int i = 0; i< gBox->refi_Reg.n; i++)
        {
            if(gBox->refi_Reg.region[i].name == currentSurface)
            {
                int rSize = gBox->refi_Reg.region[i].distances.size();
                ui->tb_MeshRefineAroundSurface->setRowCount(rSize + 1);
                for(int j = 0; j < rSize; j++)
                {
                    QTableWidgetItem *distance = new QTableWidgetItem(QString::number(gBox->refi_Reg.region[i].distances[j].lv1));
                    QTableWidgetItem *level = new QTableWidgetItem(QString::number(gBox->refi_Reg.region[i].distances[j].lv2));
                    QTableWidgetItem *action = new QTableWidgetItem(QIcon(":/resource/images/delete-icon.png"),"");
                    ui->tb_MeshRefineAroundSurface->setItem(j,0,distance);
                    ui->tb_MeshRefineAroundSurface->setItem(j,1,level);
                    ui->tb_MeshRefineAroundSurface->setItem(j,2,action);
                }
                QTableWidgetItem *action = new QTableWidgetItem(QIcon(":/resource/images/tick.png"),"");
                ui->tb_MeshRefineAroundSurface->setItem(ui->tb_MeshRefineAroundSurface->rowCount() - 1,2,action);
                ui->tb_MeshRefineAroundSurface->setItem(ui->tb_MeshRefineAroundSurface->rowCount() - 1,0,new QTableWidgetItem(""));
                ui->tb_MeshRefineAroundSurface->setItem(ui->tb_MeshRefineAroundSurface->rowCount() - 1,1,new QTableWidgetItem(""));
                return;
            }
        }
    }
    //Find in cylinder
    else if(type == 1)
    {
        GeomeCylinderSurface *gCylin = &mesh->snappyd->gCylin;
        for(int i = 0; i< gCylin->refi_Reg.n; i++)
        {
            if(gCylin->refi_Reg.region[i].name == currentSurface)
            {
                int rSize = gCylin->refi_Reg.region[i].distances.size();
                ui->tb_MeshRefineAroundSurface->setRowCount(rSize + 1);
                for(int j = 0; j < rSize; j++)
                {
                    QTableWidgetItem *distance = new QTableWidgetItem(QString::number(gCylin->refi_Reg.region[i].distances[j].lv1));
                    QTableWidgetItem *level = new QTableWidgetItem(QString::number(gCylin->refi_Reg.region[i].distances[j].lv2));
                    QTableWidgetItem *action = new QTableWidgetItem(QIcon(":/resource/images/delete-icon.png"),"");
                    ui->tb_MeshRefineAroundSurface->setItem(j,0,distance);
                    ui->tb_MeshRefineAroundSurface->setItem(j,1,level);
                    ui->tb_MeshRefineAroundSurface->setItem(j,2,action);
                }
                QTableWidgetItem *action = new QTableWidgetItem(QIcon(":/resource/images/tick.png"),"");
                ui->tb_MeshRefineAroundSurface->setItem(ui->tb_MeshRefineAroundSurface->rowCount() - 1,2,action);
                ui->tb_MeshRefineAroundSurface->setItem(ui->tb_MeshRefineAroundSurface->rowCount() - 1,0,new QTableWidgetItem(""));
                ui->tb_MeshRefineAroundSurface->setItem(ui->tb_MeshRefineAroundSurface->rowCount() - 1,1,new QTableWidgetItem(""));
                return;
            }
        }
    }
    //Find in STL
    else if(type == 2)
    {
        GeomeUserDefine *gUserDefine = &mesh->snappyd->gUserDefine;
        for(int i = 0;i< gUserDefine->refi_Reg.n; i++)
        {
            if(gUserDefine->refi_Reg.region[i].name == currentSurface)
            {

                int rSize = gUserDefine->refi_Reg.region[i].distances.size();
                ui->tb_MeshRefineAroundSurface->setRowCount(rSize + 1);
                for(int j = 0; j < rSize; j++)
                {
                    QTableWidgetItem *distance = new QTableWidgetItem(QString::number(gUserDefine->refi_Reg.region[i].distances[j].lv1));
                    QTableWidgetItem *level = new QTableWidgetItem(QString::number(gUserDefine->refi_Reg.region[i].distances[j].lv2));
                    QTableWidgetItem *action = new QTableWidgetItem(QIcon(":/resource/images/delete-icon.png"),"");
                    ui->tb_MeshRefineAroundSurface->setItem(j,0,distance);
                    ui->tb_MeshRefineAroundSurface->setItem(j,1,level);
                    ui->tb_MeshRefineAroundSurface->setItem(j,2,action);
                }
                QTableWidgetItem *action = new QTableWidgetItem(QIcon(":/resource/images/tick.png"),"");
                ui->tb_MeshRefineAroundSurface->setItem(ui->tb_MeshRefineAroundSurface->rowCount() - 1,2,action);
                ui->tb_MeshRefineAroundSurface->setItem(ui->tb_MeshRefineAroundSurface->rowCount() - 1,0,new QTableWidgetItem(""));
                ui->tb_MeshRefineAroundSurface->setItem(ui->tb_MeshRefineAroundSurface->rowCount() - 1,1,new QTableWidgetItem(""));
                return;
            }
        }
    }
    //Find in sphere
    else if(type == 3)
    {
        GeomeSphereSurface *gSphere = &mesh->snappyd->gSphere;
        for(int i = 0; i< gSphere->refi_Reg.n; i++)
        {
            if(gSphere->refi_Reg.region[i].name == currentSurface)
            {
                int rSize = gSphere->refi_Reg.region[i].distances.size();
                ui->tb_MeshRefineAroundSurface->setRowCount(rSize + 1);
                for(int j = 0; j < rSize; j++)
                {
                    QTableWidgetItem *distance = new QTableWidgetItem(QString::number(gSphere->refi_Reg.region[i].distances[j].lv1));
                    QTableWidgetItem *level = new QTableWidgetItem(QString::number(gSphere->refi_Reg.region[i].distances[j].lv2));
                    QTableWidgetItem *action = new QTableWidgetItem(QIcon(":/resource/images/delete-icon.png"),"");
                    ui->tb_MeshRefineAroundSurface->setItem(j,0,distance);
                    ui->tb_MeshRefineAroundSurface->setItem(j,1,level);
                    ui->tb_MeshRefineAroundSurface->setItem(j,2,action);
                }
                QTableWidgetItem *action = new QTableWidgetItem(QIcon(":/resource/images/tick.png"),"");
                ui->tb_MeshRefineAroundSurface->setItem(ui->tb_MeshRefineAroundSurface->rowCount() - 1,2,action);
                ui->tb_MeshRefineAroundSurface->setItem(ui->tb_MeshRefineAroundSurface->rowCount() - 1,0,new QTableWidgetItem(""));
                ui->tb_MeshRefineAroundSurface->setItem(ui->tb_MeshRefineAroundSurface->rowCount() - 1,1,new QTableWidgetItem(""));
                return;
            }
        }
    }
}

void MainWindow::loadData()
{
    QString currentSurface = ui->tb_boundary->currentItem()->text();
    if(flag_btnSurface_Click == true)
    {
        for(int i = 0; i< mesh->snappyd->gBox.refi_Sur.n; i++)
        {
            if(mesh->snappyd->gBox.refi_Sur.surfaces[i].name == currentSurface)
            {
                ui->txt_Level_Min_Surface_Refine->setText(QString::number
                                                          (mesh->snappyd->gBox.refi_Sur.surfaces[i].lv1));
                ui->txt_Level_Max_Surface_Refine->setText(QString::number
                                                 (mesh->snappyd->gBox.refi_Sur.surfaces[i].lv2));
                LoadRefineDistanceSurface(currentSurface,0);
                ui->btn_MeshRefineAdvance->hide();
                ui->groupBox_MeshSurfaceAround->show();
                return;
            }
        }
        //snappy cylinder
        for(int i = 0; i< mesh->snappyd->gCylin.refi_Sur.n; i++)
        {
            if(mesh->snappyd->gCylin.refi_Sur.surfaces[i].name == currentSurface)
            {
                ui->txt_Level_Min_Surface_Refine->setText(QString::number
                                                          (mesh->snappyd->gCylin.refi_Sur.surfaces[i].lv1));
                ui->txt_Level_Max_Surface_Refine->setText(QString::number
                                                 (mesh->snappyd->gCylin.refi_Sur.surfaces[i].lv2));
                LoadRefineDistanceSurface(currentSurface,1);
                ui->btn_MeshRefineAdvance->hide();
                ui->groupBox_MeshSurfaceAround->show();
                return;
            }
        }
        //snappy cylinder
        for(int i = 0; i< mesh->snappyd->gSphere.refi_Sur.n; i++)
        {
            if(mesh->snappyd->gSphere.refi_Sur.surfaces[i].name == currentSurface)
            {
                ui->txt_Level_Min_Surface_Refine->setText(QString::number
                                                          (mesh->snappyd->gSphere.refi_Sur.surfaces[i].lv1));
                ui->txt_Level_Max_Surface_Refine->setText(QString::number
                                                 (mesh->snappyd->gSphere.refi_Sur.surfaces[i].lv2));
                LoadRefineDistanceSurface(currentSurface,3);
                ui->btn_MeshRefineAdvance->hide();
                ui->groupBox_MeshSurfaceAround->show();
                return;
            }
        }
        //snappy userdefine
        GeomeUserDefine *gUserDefine = &mesh->snappyd->gUserDefine;
        for(int i = 0; i< gUserDefine->refi_Sur.n; i++)
        {
            if(gUserDefine->refi_Sur.surfaces[i].name == currentSurface)
            {
                ui->txt_Level_Min_Surface_Refine->setText(QString::number
                                                          (gUserDefine->refi_Sur.surfaces[i].lv1));
                ui->txt_Level_Max_Surface_Refine->setText(QString::number
                                                 (gUserDefine->refi_Sur.surfaces[i].lv2));
                //Region
                LoadRefineDistanceSurface(currentSurface,2);

                ui->btn_MeshRefineAdvance->show();
                ui->groupBox_MeshSurfaceAround->show();
                return;
            }
        }
    }
    if(flag_btnVolume_Click == true)
    {
        RefinementRegions *refi_Reg = &mesh->snappyd->gBoxRegion.refi_Reg;
        for(int i = 0; i < refi_Reg->region.size(); i++)
        {
            if(refi_Reg->region[i].name == currentSurface)
            {
                if(refi_Reg->region[i].mode =="inside" )
                    ui->cb_MeshVolumeMode->setCurrentIndex(1);
                else if(refi_Reg->region[i].mode =="outside" )
                    ui->cb_MeshVolumeMode->setCurrentIndex(2);
                ui->txt_Level_Volume->setText(QString::number(refi_Reg->region[i].lv2));
            }
        }
        GeomeBoxRegion *gBoxRegion = &mesh->snappyd->gBoxRegion;
        for(int i = 0; i < gBoxRegion->n; i++)
        {
            if(gBoxRegion->boxes[i].name == currentSurface)
            {
                ui->cb_VolumeType->setCurrentIndex(0);
                ui->txt_Min_Box_Vol_X->setText(QString::number(gBoxRegion->boxes[i].min.x));
                ui->txt_Min_Box_Vol_Y->setText(QString::number(gBoxRegion->boxes[i].min.y));
                ui->txt_Min_Box_Vol_Z->setText(QString::number(gBoxRegion->boxes[i].min.z));
                ui->txt_Max_Box_Vol_X->setText(QString::number(gBoxRegion->boxes[i].max.x));
                ui->txt_Max_Box_Vol_Y->setText(QString::number(gBoxRegion->boxes[i].max.y));
                ui->txt_Max_Box_Vol_Z->setText(QString::number(gBoxRegion->boxes[i].max.z));
                return;
            }
        }
        refi_Reg = &mesh->snappyd->gCylinRegion.refi_Reg;
        for(int i = 0; i < refi_Reg->region.size(); i++)
        {
            if(refi_Reg->region[i].name == currentSurface)
            {
                if(refi_Reg->region[i].mode =="inside" )
                    ui->cb_MeshVolumeMode->setCurrentIndex(1);
                else if(refi_Reg->region[i].mode =="outside" )
                    ui->cb_MeshVolumeMode->setCurrentIndex(2);
                ui->txt_Level_Volume->setText(QString::number(refi_Reg->region[i].lv2));
            }
        }
        GeomeCylinderRegion *gCylinRegion = &mesh->snappyd->gCylinRegion;
        for(int i = 0; i < gCylinRegion->n; i++)
        {
            if(gCylinRegion->cylins[i].name == currentSurface)
            {
                ui->cb_VolumeType->setCurrentIndex(1);
                ui->txt_P1_Cyl_Vol_X->setText(QString::number(gCylinRegion->cylins[i].point1.x));
                ui->txt_P1_Cyl_Vol_Y->setText(QString::number(gCylinRegion->cylins[i].point1.y));
                ui->txt_P1_Cyl_Vol_Z->setText(QString::number(gCylinRegion->cylins[i].point1.z));
                ui->txt_P2_Cyl_Vol_X->setText(QString::number(gCylinRegion->cylins[i].point2.x));
                ui->txt_P2_Cyl_Vol_Y->setText(QString::number(gCylinRegion->cylins[i].point2.y));
                ui->txt_P2_Cyl_Vol_Z->setText(QString::number(gCylinRegion->cylins[i].point2.z));
                ui->txt_Radius_Cyl_Vol->setText(QString::number(gCylinRegion->cylins[i].radius));
                return;
            }
        }
        refi_Reg = &mesh->snappyd->gSphereRegion.refi_Reg;
        for(int i = 0; i < refi_Reg->region.size(); i++)
        {
            if(refi_Reg->region[i].name == currentSurface)
            {
                if(refi_Reg->region[i].mode =="inside" )
                    ui->cb_MeshVolumeMode->setCurrentIndex(1);
                else if(refi_Reg->region[i].mode =="outside" )
                    ui->cb_MeshVolumeMode->setCurrentIndex(2);
                ui->txt_Level_Volume->setText(QString::number(refi_Reg->region[i].distances[0].lv2));
            }
        }
        GeomeSphereRegion *gSphereRegion = &mesh->snappyd->gSphereRegion;
        for(int i = 0; i < gSphereRegion->n; i++)
        {
            if(gSphereRegion->sphere[i].name == currentSurface)
            {
                ui->cb_VolumeType->setCurrentIndex(2);
                ui->txt_P_Sphe_Vol_X->setText(QString::number(gSphereRegion->sphere[i].centre.x));
                ui->txt_P_Sphe_Vol_Y->setText(QString::number(gSphereRegion->sphere[i].centre.y));
                ui->txt_P_Sphe_Vol_Z->setText(QString::number(gSphereRegion->sphere[i].centre.z));
                ui->txt_Radius_Sphe_Vol->setText(QString::number(gSphereRegion->sphere[i].radius));
                return;
            }
        }
    }
    if(flag_btnSurGeometry_Click ==true)
    {
        for(int i=0; i< mesh->snappyd->gBox.n; i++)
        {
            if(currentSurface==  mesh->snappyd->gBox.boxes[i].name)
            {
                ui->txt_GeometrySurfaceFileStl->setText("");

                if(mesh->snappyd->gBox.boxes[i].type =="searchableBox")
                    ui->cb_SurfaceType->setCurrentIndex(0);

                ui->txt_GeometrySurfaceName->setText(mesh->snappyd->gBox.boxes[i].name);
                ui->txt_Min_Box_Sur_X->setText(QString::number(mesh->snappyd->gBox.boxes[i].min.x));
                ui->txt_Min_Box_Sur_Y->setText(QString::number(mesh->snappyd->gBox.boxes[i].min.y));
                ui->txt_Min_Box_Sur_Z->setText(QString::number(mesh->snappyd->gBox.boxes[i].min.z));

                ui->txt_Max_Box_Sur_X->setText(QString::number(mesh->snappyd->gBox.boxes[i].max.x));
                ui->txt_Max_Box_Sur_Y->setText(QString::number(mesh->snappyd->gBox.boxes[i].max.y));
                ui->txt_Max_Box_Sur_Z->setText(QString::number(mesh->snappyd->gBox.boxes[i].max.z));
                return;
            }
        }
        for(int i=0; i< mesh->snappyd->gCylin.n; i++)
        {
            if(currentSurface==  mesh->snappyd->gCylin.cylins[i].name)
            {
                ui->txt_GeometrySurfaceFileStl->setText("");

                if(mesh->snappyd->gCylin.cylins[i].type =="searchableCylinder")
                    ui->cb_SurfaceType->setCurrentIndex(1);

                ui->txt_GeometrySurfaceName->setText(mesh->snappyd->gCylin.cylins[i].name);
                ui->txt_P1_Cyl_Sur_X->setText(QString::number(mesh->snappyd->gCylin.cylins[i].point1.x));
                ui->txt_P1_Cyl_Sur_Y->setText(QString::number(mesh->snappyd->gCylin.cylins[i].point1.y));
                ui->txt_P1_Cyl_Sur_Z->setText(QString::number(mesh->snappyd->gCylin.cylins[i].point1.z));

                ui->txt_P2_Cyl_Sur_X->setText(QString::number(mesh->snappyd->gCylin.cylins[i].point2.x));
                ui->txt_P2_Cyl_Sur_Y->setText(QString::number(mesh->snappyd->gCylin.cylins[i].point2.y));
                ui->txt_P2_Cyl_Sur_Z->setText(QString::number(mesh->snappyd->gCylin.cylins[i].point2.z));

                ui->txt_Radius_Cyl_Sur->setText(QString::number(mesh->snappyd->gCylin.cylins[i].radius));
                return;
            }
        }
        for(int i=0; i< mesh->snappyd->gSphere.n; i++)
        {
            if(currentSurface==  mesh->snappyd->gSphere.sphere[i].name)
            {
                ui->txt_GeometrySurfaceFileStl->setText("");

                if(mesh->snappyd->gSphere.sphere[i].type =="searchableSphere")
                    ui->cb_SurfaceType->setCurrentIndex(2);
                ui->txt_GeometrySurfaceName->setText(mesh->snappyd->gSphere.sphere[i].name);
                ui->txt_P_Sphe_Sur_X->setText(QString::number(mesh->snappyd->gSphere.sphere[i].centre.x));
                ui->txt_P_Sphe_Sur_Y->setText(QString::number(mesh->snappyd->gSphere.sphere[i].centre.y));
                ui->txt_P_Sphe_Sur_Z->setText(QString::number(mesh->snappyd->gSphere.sphere[i].centre.z));

                ui->txt_Radius_Sphe_Sur->setText(QString::number(mesh->snappyd->gSphere.sphere[i].radius));
                return;
            }
        }
        for(int i=0; i< mesh->snappyd->gUserDefine.n; i++)
        {
            if(currentSurface==  mesh->snappyd->gUserDefine.user_Defines[i].name)
            {
                ui->txt_GeometrySurfaceFileStl->setText(mesh->snappyd->gUserDefine.user_Defines[i].direction);

                ui->txt_GeometrySurfaceName->setText("");
                ui->cb_SurfaceType->setCurrentIndex(0);

                ui->txt_Min_Box_Sur_X->setText("");
                ui->txt_Min_Box_Sur_Y->setText("");
                ui->txt_Min_Box_Sur_Z->setText("");

                ui->txt_Max_Box_Sur_X->setText("");
                ui->txt_Max_Box_Sur_Y->setText("");
                ui->txt_Max_Box_Sur_Z->setText("");

                ui->txt_P1_Cyl_Sur_X->setText("");
                ui->txt_P1_Cyl_Sur_Y->setText("");
                ui->txt_P1_Cyl_Sur_Z->setText("");

                ui->txt_P2_Cyl_Sur_X->setText("");
                ui->txt_P2_Cyl_Sur_Y->setText("");
                ui->txt_P2_Cyl_Sur_Z->setText("");

                ui->txt_Radius_Cyl_Sur->setText("");

                return;
            }
        }
    }
    if(flag_btn_MeshLayer_Click ==true)
    {
        for(int i=0;i< mesh->snappyd->add_Layers_Controls.layers.size(); i++)
        {
            if(mesh->snappyd->add_Layers_Controls.layers[i].name == ui->tb_boundary->currentItem()->text())
            {
                ui->txt_Layer_Bounding_2->setText(QString::number(mesh->snappyd->add_Layers_Controls.layers[i].nSurfaceLayers));
                return;
            }
        }
        ui->txt_Layer_Bounding_2->setText("0");
    }
}

void MainWindow::boundaryDefault()
{
    if(listSurfacesDefault.size() == 0)
        return;
    int n = listSurfacesDefault.size();
    for(int i = 0;i< n; i++){
        Boundary b;
        b.name = listSurfacesDefault[i];
        b.patchInfo.type="patch";
        b.patches.surfaces.append(listSurfacesDefault[i]);
        mesh->patchDict->boundaries.append(b);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// Geometry Area ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_btn_Surface_clicked()
{
    flag_btnSurGeometry_Click = true;
    flag_btnBounding_Click =false;
    flag_btnSurface_Click = false;
    flag_btnVolume_Click = false;
    flag_btn_MeshLayer_Click =false;
    ui->frame_GeometryBounding->hide();
    ui->frame_GeometrySurface->show();
    ui->btn_DeleteSurface->setEnabled(true);
    //reset all textbox
    ui->txt_GeometrySurfaceFileStl->setText("");
    //cylin
    ui->txt_GeometrySurfaceName->setText("");
    ui->cb_SurfaceType->setCurrentIndex(0);
    ui->txt_P1_Cyl_Sur_X->setText("");
    ui->txt_P1_Cyl_Sur_Y->setText("");
    ui->txt_P1_Cyl_Sur_Z->setText("");
    ui->txt_P2_Cyl_Sur_X->setText("");
    ui->txt_P2_Cyl_Sur_Y->setText("");
    ui->txt_P2_Cyl_Sur_Z->setText("");
    ui->txt_Radius_Cyl_Sur->setText("");
    //box
    ui->txt_Min_Box_Sur_X->setText("");
    ui->txt_Min_Box_Sur_Y->setText("");
    ui->txt_Min_Box_Sur_Z->setText("");
    ui->txt_Max_Box_Sur_X->setText("");
    ui->txt_Max_Box_Sur_Y->setText("");
    ui->txt_Max_Box_Sur_Z->setText("");
//    ui->txt_Log->clear();
    //hide orther
    Remove_All_Face();
    //Show all surface
    for(int i=0; i< mesh->snappyd->gBox.n; i++)
    {
        AddFaceToList(mesh->snappyd->gBox.boxes[i].name);
    }
    for(int i=0; i< mesh->snappyd->gCylin.n; i++)
    {
        AddFaceToList(mesh->snappyd->gCylin.cylins[i].name);
    }
    for(int i=0; i< mesh->snappyd->gSphere.n; i++)
    {
        AddFaceToList(mesh->snappyd->gSphere.sphere[i].name);
    }
    for(int i=0; i< mesh->snappyd->gUserDefine.n; i++)
    {
        AddFaceToList(mesh->snappyd->gUserDefine.user_Defines[i].name);
    }
}
void MainWindow::on_btn_Bounding_clicked()
{
    ui->frame_GeometrySurface->hide();
    ui->frame_BoundingGeometryType->hide();
    ui->frame_GeometryBounding->show();
    ui->btn_DeleteSurface->setEnabled(false);
    Remove_All_Face();
    if(mesh->blockd->boundMesh.n  > 0 && !mesh->blockd->isAutomatic)
    {
        //add list faces
        for(int i =0; i< mesh->blockd->boundMesh.n; i++)
        {
            AddFaceToList(mesh->blockd->boundMesh.bounsType[i].name);
        }
    }
    //choose default
    if(mesh->blockd->isAutomatic)
        ui->cb_BoundingType->setCurrentIndex(0);
}

void MainWindow::on_btn_Browse_clicked()
{
    file_name_STL = QFileDialog::getOpenFileName(this,"Import file stl","",tr("Slt (*.stl)"));
    if(file_name_STL != "")
        ui->txt_GeometrySurfaceFileStl->setText(file_name_STL);
}
void MainWindow::on_btn_GeoImportSurface_clicked()
{
    if(ui->txt_GeometrySurfaceFileStl->text()=="")
    {
        QMessageBox::information(this,"Error","Please select a STL file!");
        return;
    }
    else
    {
        //Get filename and name of folder containds file
        QStringList file_name = file_name_STL.split("/");
        if(file_name.length()==1)
            file_name = file_name_STL.split("\\");
        QString name = file_name[file_name.length()-1];
        QString _name = name.split(".")[0];
//        QString folder_STl = file_name_STL.split(file_name[file_name.length()-1])[0];
        //check name exists
        for(int i=0; i< mesh->snappyd->gUserDefine.n; i++)
        {
            if(name==  mesh->snappyd->gUserDefine.user_Defines[i].name_file)
            {
                QMessageBox::information(this,"Error","This name is already exists");
                return;
            }
        }

        //read file
        QString file1;
        QFile file(file_name_STL);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
             file1= file.readAll();
        }
        if(file1 == "")
        {
            QMessageBox::information(this,"Error","File is empty!");
            return;
        }


        mesh->snappyd->gUserDefine.n= mesh->snappyd->gUserDefine.n+1;
        mesh->snappyd->gUserDefine.user_Defines.resize(mesh->snappyd->gUserDefine.n);
        mesh->snappyd->gUserDefine.user_Defines[mesh->snappyd->gUserDefine.n-1].direction= file_name_STL;
        mesh->snappyd->gUserDefine.user_Defines[mesh->snappyd->gUserDefine.n-1].name_file=name;
        mesh->snappyd->gUserDefine.user_Defines[mesh->snappyd->gUserDefine.n-1].type="triSurfaceMesh";
        mesh->snappyd->gUserDefine.user_Defines[mesh->snappyd->gUserDefine.n-1].name=_name;
        AddFaceToList(_name);

        //add surface
        int n = mesh->snappyd->gUserDefine.refi_Sur.n+1;
        mesh->snappyd->gUserDefine.refi_Sur.n=mesh->snappyd->gUserDefine.refi_Sur.n+1;
        mesh->snappyd->gUserDefine.refi_Sur.surfaces.resize(n);
        mesh->snappyd->gUserDefine.refi_Sur.surfaces[n-1].lv1=0;
        mesh->snappyd->gUserDefine.refi_Sur.surfaces[n-1].lv2=0;
        mesh->snappyd->gUserDefine.refi_Sur.surfaces[n-1].name=_name;
        //add reg
        int m = mesh->snappyd->gUserDefine.refi_Reg.n+1;
        mesh->snappyd->gUserDefine.refi_Reg.n=mesh->snappyd->gUserDefine.refi_Reg.n+1;
        mesh->snappyd->gUserDefine.refi_Reg.region.resize(m);
        mesh->snappyd->gUserDefine.refi_Reg.region[m-1].mode = "distance";
        mesh->snappyd->gUserDefine.refi_Reg.region[m-1].name=_name;
        //chua co region nao
        mesh->snappyd->gUserDefine.refi_Sur.surfaces[n-1].n=0;
        int index =0;
        QStringList lines = file1.split("\n",QString::SkipEmptyParts);
        for(int i=0; i< lines.length(); i++)
        {
            if(lines[i].left(5)=="solid")
            {
                QString name_re = lines[i].split(" ")[1];
                mesh->snappyd->gUserDefine.refi_Sur.surfaces[n-1].regionSTLs.resize(index +1);
                mesh->snappyd->gUserDefine.refi_Sur.surfaces[n-1].regionSTLs[index].name=name_re;
                mesh->snappyd->gUserDefine.refi_Sur.surfaces[n-1].regionSTLs[index].lv1=0;
                mesh->snappyd->gUserDefine.refi_Sur.surfaces[n-1].regionSTLs[index].lv2=0;
                index++;
            }
        }
        mesh->snappyd->gUserDefine.refi_Sur.surfaces[n-1].n=index;

        mesh->snappyd->min_Max.max.x=-1000000.0;
        mesh->snappyd->min_Max.max.y=-1000000.0;
        mesh->snappyd->min_Max.max.z=-1000000.0;

        mesh->snappyd->min_Max.min.x=1000000.0;
        mesh->snappyd->min_Max.min.y=1000000.0;
        mesh->snappyd->min_Max.min.z=1000000.0;
        //Read file STL
        if(mesh->snappyd->ReadSTLFile(file_name_STL))
        {
            QStringList views = mesh->GetViewList();
            foreach(Solid solid, mesh->snappyd->sTL[mesh->snappyd->sTL.size() - 1].solids)
                listSurfaces.append(_name + "_" + solid.name);
            views.append(_name);
            mesh->SetViewList(views);

            SetBoundingDistance();
            ui->cb_BoundingType->setCurrentIndex(0);
        }
    }
}
void MainWindow::on_btn_GeoDefineSurface_clicked()
{
    QString surfaceName = ui->txt_GeometrySurfaceName->text();
    if(!CheckNameValid(surfaceName))
        return;
    //Save value for each type
    if(ui->cb_SurfaceType->currentText() == "Cylinder")
    {
       //define cylinder type
        if(ui->txt_P1_Cyl_Sur_X->text()=="" || ui->txt_P1_Cyl_Sur_Y->text()=="" || ui->txt_P1_Cyl_Sur_Z->text()=="" || ui->txt_P2_Cyl_Sur_X->text()==""
           ||ui->txt_P2_Cyl_Sur_Y->text()=="" || ui->txt_P2_Cyl_Sur_Z->text()=="" || ui->txt_Radius_Cyl_Sur->text()=="")
        {
            QMessageBox::information(this,"Error","Values are incorrect!");
            return;
        }
        else
        {
            bool a,b,c,d,e,k,l;
            float x1 = ui->txt_P1_Cyl_Sur_X->text().toFloat(&a);
            float x2 = ui->txt_P2_Cyl_Sur_X->text().toFloat(&b);
            float y1 = ui->txt_P1_Cyl_Sur_Y->text().toFloat(&c);
            float y2 = ui->txt_P2_Cyl_Sur_Y->text().toFloat(&d);
            float z1 = ui->txt_P1_Cyl_Sur_Z->text().toFloat(&e);
            float z2 = ui->txt_P2_Cyl_Sur_Z->text().toFloat(&k);

            float radius_cyl = ui->txt_Radius_Cyl_Sur->text().toFloat(&l);
            if(!a || !b || !c || !d ||!d || !e || !k || !l)
            {
                QMessageBox::information(this,"Error","Values are incorrect!");
                return;
            }
            //find min max default bouding
            float P1,P2,P3,P4;
            float costemp = (x1 - x2)/ sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2) );

            P1 = x1 + radius_cyl;//*costemp;
            P2 = x1 - radius_cyl;//*costemp;
            P3 = x2 + radius_cyl;//*costemp;
            P4 = x2 - radius_cyl;//*costemp;

            float minx= P1;
            float maxx = P1;
            if(minx>P2)
            {
                minx = P2;
            }
            if(minx>P3)
            {
                minx = P3;
            }
            if(minx>P4)
            {
                minx = P4;
            }
            if(maxx<P2)
            {
                maxx = P2;
            }
            if(maxx<P3)
            {
                maxx = P3;
            }
            if(maxx<P4)
            {
                maxx = P4;
            }

            costemp = (y1 - y2)/ sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2) );

            P1 = y1 + radius_cyl;//*costemp;
            P2 = y1 - radius_cyl;//*costemp;
            P3 = y2 + radius_cyl;//*costemp;
            P4 = y2 - radius_cyl;//*costemp;

            float miny= P1;
            float maxy = P1;
            if(miny>P2)
            {
                miny = P2;
            }
            if(miny>P3)
            {
                miny = P3;
            }
            if(miny>P4)
            {
                miny = P4;
            }

            if(maxy<P2)
            {
                maxy = P2;
            }
            if(maxy<P3)
            {
                maxy = P3;
            }
            if(maxy<P4)
            {
                maxy = P4;
            }

            costemp = (z1 - z2)/ sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2) );

            P1 = z1 + radius_cyl;//*costemp;
            P2 = z1 - radius_cyl;//*costemp;
            P3 = z2 + radius_cyl;//*costemp;
            P4 = z2 - radius_cyl;//*costemp;

            float minz= P1;
            float maxz = P1;
            if(minz>P2)
            {
                minz = P2;
            }
            if(minz>P3)
            {
                minz = P3;
            }
            if(minz>P4)
            {
                minz = P4;
            }

            if(maxz<P2)
            {
                maxz = P2;
            }
            if(maxz<P3)
            {
                maxz = P3;
            }
            if(maxz<P4)
            {
                maxz = P4;
            }

            //mesh->snappyd->FindMinMaxDefaultBounding(minx, miny, minz);
            //mesh->snappyd->FindMinMaxDefaultBounding(maxx, maxy, maxz);

            mesh->snappyd->min_Max.min.x = minx;
            mesh->snappyd->min_Max.min.y = miny;
            mesh->snappyd->min_Max.min.z = minz;

            mesh->snappyd->min_Max.max.x = maxx;
            mesh->snappyd->min_Max.max.y = maxy;
            mesh->snappyd->min_Max.max.z = maxz;

            //add min_Max temp to list min max;
            mesh->snappyd->min_Max.name_Surface = surfaceName;
            mesh->snappyd->list_Surface_Min_Max.append(mesh->snappyd->min_Max);
            mesh->snappyd->min_Max.name_Surface="";
            mesh->snappyd->FindMinMax(mesh->snappyd->list_Surface_Min_Max);

            //add geometry values to mesh
            mesh->snappyd->gCylin.n=mesh->snappyd->gCylin.n+1;
            mesh->snappyd->gCylin.cylins.resize(mesh->snappyd->gCylin.n);
            mesh->snappyd->gCylin.cylins[mesh->snappyd->gCylin.n-1].name=surfaceName;
            mesh->snappyd->gCylin.cylins[mesh->snappyd->gCylin.n-1].type = "searchableCylinder";
            mesh->snappyd->gCylin.cylins[mesh->snappyd->gCylin.n-1].point2.x=x2;
            mesh->snappyd->gCylin.cylins[mesh->snappyd->gCylin.n-1].point2.y=y2;
            mesh->snappyd->gCylin.cylins[mesh->snappyd->gCylin.n-1].point2.z=z2;
            mesh->snappyd->gCylin.cylins[mesh->snappyd->gCylin.n-1].point1.x=x1;
            mesh->snappyd->gCylin.cylins[mesh->snappyd->gCylin.n-1].point1.y=y1;
            mesh->snappyd->gCylin.cylins[mesh->snappyd->gCylin.n-1].point1.z=z1;
            mesh->snappyd->gCylin.cylins[mesh->snappyd->gCylin.n-1].radius= radius_cyl;

            //add surface and region default
            mesh->snappyd->gCylin.refi_Sur.n =mesh->snappyd->gCylin.refi_Sur.n+1;
            mesh->snappyd->gCylin.refi_Sur.surfaces.resize(mesh->snappyd->gCylin.refi_Sur.n);
            mesh->snappyd->gCylin.refi_Sur.surfaces[mesh->snappyd->gCylin.refi_Sur.n-1].lv1= 0;
            mesh->snappyd->gCylin.refi_Sur.surfaces[mesh->snappyd->gCylin.refi_Sur.n-1].lv2= 0;
            mesh->snappyd->gCylin.refi_Sur.surfaces[mesh->snappyd->gCylin.refi_Sur.n-1].name=surfaceName;

            mesh->snappyd->gCylin.refi_Reg.n = mesh->snappyd->gCylin.refi_Reg.n+1;
            mesh->snappyd->gCylin.refi_Reg.region.resize(mesh->snappyd->gCylin.refi_Reg.n);
            mesh->snappyd->gCylin.refi_Reg.region[mesh->snappyd->gCylin.refi_Reg.n-1].name = surfaceName;
            mesh->snappyd->gCylin.refi_Reg.region[mesh->snappyd->gCylin.refi_Reg.n-1].mode = "distance";

            AddFaceToList(surfaceName);            
            listSurfaces.append(surfaceName);

            QStringList views = mesh->GetViewList();
            views.append(surfaceName);
            mesh->SetViewList(views);

            SetBoundingDistance();
//            mesh->snappyd->facename.append(surfaceName);
            mesh->updateGL();
            ui->txt_Log->appendPlainText("Defining "+ surfaceName +" cylinder has been done");
        }
    }
    else if(ui->cb_SurfaceType->currentText() == "Box")
    {
       //define box type
        if(ui->txt_Max_Box_Sur_X->text()=="" || ui->txt_Max_Box_Sur_Y->text()=="" || ui->txt_Max_Box_Sur_Z->text()=="" ||
           ui->txt_Min_Box_Sur_X->text()=="" || ui->txt_Min_Box_Sur_Y->text()=="" || ui->txt_Min_Box_Sur_Z->text()=="")
        {
            QMessageBox::information(this,"Error","Values are incorrect!");
            return;
        }
        else
        {
            bool a,b,c,d,e,k;
            float x1 = ui->txt_Min_Box_Sur_X->text().toFloat(&a);
            float x2 = ui->txt_Max_Box_Sur_X->text().toFloat(&b);
            float y1 = ui->txt_Min_Box_Sur_Y->text().toFloat(&c);
            float y2 = ui->txt_Max_Box_Sur_Y->text().toFloat(&d);
            float z1 = ui->txt_Min_Box_Sur_Z->text().toFloat(&e);
            float z2 = ui->txt_Max_Box_Sur_Z->text().toFloat(&k);

            if(!a || !b || !c || !d ||!d || !e || !k)
            {
                QMessageBox::information(this,"Error","Values are incorrect!");
                return;
            }
            //find min max default bouding
            //mesh->snappyd->FindMinMaxDefaultBounding(x1, y1, z1);
            //mesh->snappyd->FindMinMaxDefaultBounding(x2, y2, z2);

            mesh->snappyd->min_Max.min.x = x1;
            mesh->snappyd->min_Max.min.y = y1;
            mesh->snappyd->min_Max.min.z = z1;

            mesh->snappyd->min_Max.max.x = x2;
            mesh->snappyd->min_Max.max.y = y2;
            mesh->snappyd->min_Max.max.z = z2;

            //add min_Max temp to list min max;
            mesh->snappyd->min_Max.name_Surface = surfaceName;
            mesh->snappyd->list_Surface_Min_Max.append(mesh->snappyd->min_Max);
            mesh->snappyd->min_Max.name_Surface="";
            mesh->snappyd->min_Max.min.x=1000000;
            mesh->snappyd->min_Max.min.y=1000000;
            mesh->snappyd->min_Max.min.z=1000000;
            mesh->snappyd->min_Max.max.x=-1000000;
            mesh->snappyd->min_Max.max.y=-1000000;
            mesh->snappyd->min_Max.max.z=-1000000;
            mesh->snappyd->FindMinMax(mesh->snappyd->list_Surface_Min_Max);

            mesh->snappyd->gBox.n=mesh->snappyd->gBox.n+1;
            mesh->snappyd->gBox.boxes.resize(mesh->snappyd->gBox.n);
            mesh->snappyd->gBox.boxes[mesh->snappyd->gBox.n-1].name=surfaceName;
            mesh->snappyd->gBox.boxes[mesh->snappyd->gBox.n-1].type="searchableBox";
            mesh->snappyd->gBox.boxes[mesh->snappyd->gBox.n-1].max.x=x2;
            mesh->snappyd->gBox.boxes[mesh->snappyd->gBox.n-1].max.y=y2;
            mesh->snappyd->gBox.boxes[mesh->snappyd->gBox.n-1].max.z=z2;
            mesh->snappyd->gBox.boxes[mesh->snappyd->gBox.n-1].min.x=x1;
            mesh->snappyd->gBox.boxes[mesh->snappyd->gBox.n-1].min.y=y1;
            mesh->snappyd->gBox.boxes[mesh->snappyd->gBox.n-1].min.z=z1;

            //add surface and region default
            mesh->snappyd->gBox.refi_Sur.n =mesh->snappyd->gBox.refi_Sur.n+1;
            mesh->snappyd->gBox.refi_Sur.surfaces.resize(mesh->snappyd->gBox.refi_Sur.n);
            mesh->snappyd->gBox.refi_Sur.surfaces[mesh->snappyd->gBox.refi_Sur.n-1].lv1= 0;
            mesh->snappyd->gBox.refi_Sur.surfaces[mesh->snappyd->gBox.refi_Sur.n-1].lv2= 0;
            mesh->snappyd->gBox.refi_Sur.surfaces[mesh->snappyd->gBox.refi_Sur.n-1].name=surfaceName;

            mesh->snappyd->gBox.refi_Reg.n = mesh->snappyd->gBox.refi_Reg.n+1;
            mesh->snappyd->gBox.refi_Reg.region.resize(mesh->snappyd->gBox.refi_Reg.n);
            mesh->snappyd->gBox.refi_Reg.region[mesh->snappyd->gBox.refi_Reg.n-1].name = surfaceName;
            mesh->snappyd->gBox.refi_Reg.region[mesh->snappyd->gBox.refi_Reg.n-1].mode = "distance";

            //draw box

            int plus = mesh->snappyd->points.size();
            mesh->snappyd->points.resize(8+plus);
            mesh->snappyd->points[0+plus] = new  float[3];
            mesh->snappyd->points[0+plus][0] = x1;
            mesh->snappyd->points[0+plus][1] = y1;
            mesh->snappyd->points[0+plus][2] = z1;


            mesh->snappyd->points[1+plus] = new  float[3];
            mesh->snappyd->points[1+plus][0] = x2;
            mesh->snappyd->points[1+plus][1] = y1;
            mesh->snappyd->points[1+plus][2] = z1;

            mesh->snappyd->points[2+plus] = new  float[3];
            mesh->snappyd->points[2+plus][0] = x2;
            mesh->snappyd->points[2+plus][1] = y2;
            mesh->snappyd->points[2+plus][2] = z1;

            mesh->snappyd->points[3+plus] = new  float[3];
            mesh->snappyd->points[3+plus][0] = x1;
            mesh->snappyd->points[3+plus][1] = y2;
            mesh->snappyd->points[3+plus][2] = z1;

            mesh->snappyd->points[4+plus] = new  float[3];
            mesh->snappyd->points[4+plus][0] = x1;
            mesh->snappyd->points[4+plus][1] = y1;
            mesh->snappyd->points[4+plus][2] = z2;

            mesh->snappyd->points[5+plus] = new  float[3];
            mesh->snappyd->points[5+plus][0] = x2;
            mesh->snappyd->points[5+plus][1] = y1;
            mesh->snappyd->points[5+plus][2] = z2;

            mesh->snappyd->points[6+plus] = new  float[3];
            mesh->snappyd->points[6+plus][0] = x2;
            mesh->snappyd->points[6+plus][1] = y2;
            mesh->snappyd->points[6+plus][2] = z2;

            mesh->snappyd->points[7+plus] = new  float[3];
            mesh->snappyd->points[7+plus][0] = x1;
            mesh->snappyd->points[7+plus][1] = y2;
            mesh->snappyd->points[7+plus][2] = z2;

            int starend[2];
            starend[0] = mesh->snappyd->faces.size();

            mesh->snappyd->faces.resize(6+starend[0]);
            mesh->snappyd->faces[0+starend[0]] = new int[4];
            mesh->snappyd->faces[0+starend[0]][0] = 3 + plus;
            mesh->snappyd->faces[0+starend[0]][1] = 7 + plus;
            mesh->snappyd->faces[0+starend[0]][2] = 6 + plus;
            mesh->snappyd->faces[0+starend[0]][3] = 2 + plus;

            mesh->snappyd->faces[1+starend[0]] = new int[4];
            mesh->snappyd->faces[1+starend[0]][0] = 1 + plus;
            mesh->snappyd->faces[1+starend[0]][1] = 5 + plus;
            mesh->snappyd->faces[1+starend[0]][2] = 4 + plus;
            mesh->snappyd->faces[1+starend[0]][3] = 0 + plus;

            mesh->snappyd->faces[2+starend[0]] = new int[4];
            mesh->snappyd->faces[2+starend[0]][0] = 4 + plus;
            mesh->snappyd->faces[2+starend[0]][1] = 5 + plus;
            mesh->snappyd->faces[2+starend[0]][2] = 6 + plus;
            mesh->snappyd->faces[2+starend[0]][3] = 7 + plus;

            mesh->snappyd->faces[3+starend[0]] = new int[4];
            mesh->snappyd->faces[3+starend[0]][0] = 0 + plus;
            mesh->snappyd->faces[3+starend[0]][1] = 3 + plus;
            mesh->snappyd->faces[3+starend[0]][2] = 2 + plus;
            mesh->snappyd->faces[3+starend[0]][3] = 1 + plus;

            mesh->snappyd->faces[4+starend[0]] = new int[4];
            mesh->snappyd->faces[4+starend[0]][0] = 0 + plus;
            mesh->snappyd->faces[4+starend[0]][1] = 4 + plus;
            mesh->snappyd->faces[4+starend[0]][2] = 7 + plus;
            mesh->snappyd->faces[4+starend[0]][3] = 3 + plus;

            mesh->snappyd->faces[5+starend[0]] = new int[4];
            mesh->snappyd->faces[5+starend[0]][0] = 2 + plus;
            mesh->snappyd->faces[5+starend[0]][1] = 6 + plus;
            mesh->snappyd->faces[5+starend[0]][2] = 5 + plus;
            mesh->snappyd->faces[5+starend[0]][3] = 1 + plus;

            starend[1] = mesh->snappyd->faces.size();

            mesh->snappyd->facezones.resize( mesh->snappyd->facezones.size()+1);
            mesh->snappyd->facezones[mesh->snappyd->facezones.size()-1]= new int[2];
            mesh->snappyd->facezones[mesh->snappyd->facezones.size()-1][0]= starend[0];
            mesh->snappyd->facezones[mesh->snappyd->facezones.size()-1][1]= starend[1];

            mesh->snappyd->facetype.resize(mesh->snappyd->facetype.size() + 1);
            mesh->snappyd->facetype[mesh->snappyd->facetype.size() - 1] = 1;

            mesh->snappyd->facename.append(surfaceName);

            QStringList views = mesh->GetViewList();
            views.append(surfaceName);
            mesh->SetViewList(views);

            SetBoundingDistance();

            AddFaceToList(surfaceName);
            listSurfaces.append(surfaceName);
            ui->txt_Log->appendPlainText("Successfully create "+ surfaceName +" box");
        }

    }
    else if(ui->cb_SurfaceType->currentText() == "Sphere")
    {
        //define cylinder type
         if(ui->txt_P_Sphe_Sur_X->text()=="" || ui->txt_P_Sphe_Sur_Y->text()=="" || ui->txt_P_Sphe_Sur_Z->text()=="" || ui->txt_Radius_Sphe_Sur->text()=="")
         {
             QMessageBox::information(this,"Error","Values are incorrect!");
             return;
         }
         else
         {
             bool a,b,c,d;
             float x = ui->txt_P_Sphe_Sur_X->text().toFloat(&a);
             float y = ui->txt_P_Sphe_Sur_Y->text().toFloat(&b);
             float z = ui->txt_P_Sphe_Sur_Z->text().toFloat(&c);

             float radius_sph = ui->txt_Radius_Sphe_Sur->text().toFloat(&d);

             if(!a || !b || !c || !d  )
             {
                 QMessageBox::information(this,"Error","Values are incorrect!");
                 return;
             }
             //find min max default bouding
             //mesh->snappyd->FindMinMaxDefaultBounding(x + radius_sph, y + radius_sph, z + radius_sph);
             //mesh->snappyd->FindMinMaxDefaultBounding(x - radius_sph, y - radius_sph, z - radius_sph);
             mesh->snappyd->min_Max.min.x = x + radius_sph;
             mesh->snappyd->min_Max.min.y = y + radius_sph;
             mesh->snappyd->min_Max.min.z = z + radius_sph;

             mesh->snappyd->min_Max.max.x = x - radius_sph;
             mesh->snappyd->min_Max.max.y = y - radius_sph;
             mesh->snappyd->min_Max.max.z = z - radius_sph;

             //add min_Max temp to list min max;
             mesh->snappyd->min_Max.name_Surface = surfaceName;
             mesh->snappyd->list_Surface_Min_Max.append(mesh->snappyd->min_Max);
             mesh->snappyd->min_Max.name_Surface = "";
             mesh->snappyd->FindMinMax(mesh->snappyd->list_Surface_Min_Max);

             //add geometry values to mesh
             mesh->snappyd->gSphere.n = mesh->snappyd->gSphere.n + 1;
             mesh->snappyd->gSphere.sphere.resize(mesh->snappyd->gSphere.n);
             mesh->snappyd->gSphere.sphere[mesh->snappyd->gSphere.n-1].name = surfaceName;
             mesh->snappyd->gSphere.sphere[mesh->snappyd->gSphere.n-1].type = "searchableSphere";
             mesh->snappyd->gSphere.sphere[mesh->snappyd->gSphere.n-1].centre.x = x;
             mesh->snappyd->gSphere.sphere[mesh->snappyd->gSphere.n-1].centre.y = y;
             mesh->snappyd->gSphere.sphere[mesh->snappyd->gSphere.n-1].centre.z = z;
             mesh->snappyd->gSphere.sphere[mesh->snappyd->gSphere.n-1].radius = radius_sph;

             //add surface and region default
             mesh->snappyd->gSphere.refi_Sur.n =mesh->snappyd->gSphere.refi_Sur.n+1;
             mesh->snappyd->gSphere.refi_Sur.surfaces.resize(mesh->snappyd->gSphere.refi_Sur.n);
             mesh->snappyd->gSphere.refi_Sur.surfaces[mesh->snappyd->gSphere.refi_Sur.n-1].lv1= 0;
             mesh->snappyd->gSphere.refi_Sur.surfaces[mesh->snappyd->gSphere.refi_Sur.n-1].lv2= 0;
             mesh->snappyd->gSphere.refi_Sur.surfaces[mesh->snappyd->gSphere.refi_Sur.n-1].name=surfaceName;

             mesh->snappyd->gSphere.refi_Reg.n = mesh->snappyd->gSphere.refi_Reg.n+1;
             mesh->snappyd->gSphere.refi_Reg.region.resize(mesh->snappyd->gSphere.refi_Reg.n);
             mesh->snappyd->gSphere.refi_Reg.region[mesh->snappyd->gSphere.refi_Reg.n-1].name = surfaceName;
             mesh->snappyd->gSphere.refi_Reg.region[mesh->snappyd->gSphere.refi_Reg.n-1].mode = "distance";

             AddFaceToList(surfaceName);
             listSurfaces.append(surfaceName);

             QStringList views = mesh->GetViewList();
             views.append(surfaceName);
             mesh->SetViewList(views);

             SetBoundingDistance();
 //            mesh->snappyd->facename.append(surfaceName);
             mesh->updateGL();
             ui->txt_Log->appendPlainText("Defining "+ surfaceName +" sphere has been done");
         }
    }
    ui->cb_BoundingType->setCurrentIndex(0);
}
void MainWindow::on_btn_GeoDefineBouding_clicked()
{
    if(ui->cb_BoundingType->currentText() == "Automatic")
    {
        SetBoundingDistance();
        mesh->blockd->isAutomatic = true;
        mesh->updateGL();
    }
    else if(ui->cb_BoundingType->currentText() == "Distance")
    {
        if(CheckAndSaveBoundingDistance())
            ui->txt_Log->appendPlainText("Settings for bounding distance have been save");
    }
    else
    {
        if(CheckAndSaveBoundingBoxInput())
            ui->txt_Log->appendPlainText("Values of bounding have been save");
    }
}

///////////////////////////// Checking Changing Input //////////////////////////////////////////

void MainWindow::on_cb_SurfaceType_currentIndexChanged(QString )
{
    if(ui->cb_SurfaceType->currentText() == "Box")
    {
        ui->frame_DefineSurfaceCylinder->hide();
        ui->frame_DefineSurfaceSphere->hide();
        ui->frame_DefineSurfaceBox->show();
    }
    else if(ui->cb_SurfaceType->currentText() == "Cylinder")
    {
        ui->frame_DefineSurfaceBox->hide();
        ui->frame_DefineSurfaceSphere->hide();
        ui->frame_DefineSurfaceCylinder->show();
    }
    else if(ui->cb_SurfaceType->currentText() == "Sphere")
    {
        ui->frame_DefineSurfaceBox->hide();
        ui->frame_DefineSurfaceCylinder->hide();
        ui->frame_DefineSurfaceSphere->show();
    }
}
void MainWindow::on_cb_BoundingType_currentIndexChanged(QString )
{
    if(ui->cb_BoundingType->currentText() == "Automatic")
    {
        ui->groupBox_GeometryBoundingDefine->hide();
        ui->groupBox_GeometryBoundingDistance->hide();
    }
    else if(ui->cb_BoundingType->currentText() == "Distance")
    {
//        if(mesh->blockd->vertice.n > 6)
//        {
//            mesh->blockd->vertice.v[0].x;
//            mesh->blockd->vertice.v[0].y;
//            mesh->blockd->vertice.v[0].z;
//            mesh->blockd->vertice.v[6].x;
//            mesh->blockd->vertice.v[6].y;
//            mesh->blockd->vertice.v[6].z;

//            float x1 = mesh->snappyd->defaultBounding.minBouDef.x - mesh->blockd->vertice.v[0].x;
//            float x2 = mesh->blockd->vertice.v[6].x - mesh->snappyd->defaultBounding.maxBouDef.x;
//            float y1 = mesh->snappyd->defaultBounding.minBouDef.y - mesh->blockd->vertice.v[0].y;
//            float y2 = mesh->blockd->vertice.v[6].y - mesh->snappyd->defaultBounding.maxBouDef.y;
//            float z1 = mesh->snappyd->defaultBounding.minBouDef.z - mesh->blockd->vertice.v[0].z;
//            float z2 = mesh->blockd->vertice.v[6].z - mesh->snappyd->defaultBounding.maxBouDef.z;

//            ui->txt_DistanceMinX->setText(QString::number(x1));
//            ui->txt_DistanceMinY->setText(QString::number(y1));
//            ui->txt_DistanceMinZ->setText(QString::number(z1));
//            ui->txt_DistanceMaxX->setText(QString::number(x2));
//            ui->txt_DistanceMaxY->setText(QString::number(y2));
//            ui->txt_DistanceMaxZ->setText(QString::number(z2));
//        }
//        else
//        {
            ui->txt_DistanceMinX->setText("0");
            ui->txt_DistanceMinY->setText("0");
            ui->txt_DistanceMinZ->setText("0");
            ui->txt_DistanceMaxX->setText("0");
            ui->txt_DistanceMaxY->setText("0");
            ui->txt_DistanceMaxZ->setText("0");
//        }

        ui->groupBox_GeometryBoundingDefine->hide();
        ui->groupBox_GeometryBoundingDistance->show();
    }
    else if(ui->cb_BoundingType->currentText() == "Define")
    {
        if(mesh->blockd->vertice.n > 6)
        {
            float x1 = mesh->blockd->vertice.v[0].x;
            float x2 = mesh->blockd->vertice.v[6].x;
            float y1 = mesh->blockd->vertice.v[0].y;
            float y2 = mesh->blockd->vertice.v[6].y;
            float z1 = mesh->blockd->vertice.v[0].z;
            float z2 = mesh->blockd->vertice.v[6].z;

            ui->txt_Min_X_Bounding->setText(QString::number(x1));
            ui->txt_Min_Y_Bounding->setText(QString::number(y1));
            ui->txt_Min_Z_Bounding->setText(QString::number(z1));
            ui->txt_Max_X_Bounding->setText(QString::number(x2));
            ui->txt_Max_Y_Bounding->setText(QString::number(y2));
            ui->txt_Max_Z_Bounding->setText(QString::number(z2));
        }
        else
        {
            ui->txt_Min_X_Bounding->setText("0");
            ui->txt_Min_Y_Bounding->setText("0");
            ui->txt_Min_Z_Bounding->setText("0");
            ui->txt_Max_X_Bounding->setText("0");
            ui->txt_Max_Y_Bounding->setText("0");
            ui->txt_Max_Z_Bounding->setText("0");
        }
        ui->groupBox_GeometryBoundingDistance->hide();
        ui->groupBox_GeometryBoundingDefine->show();
    }
}
void MainWindow::on_cb_BoundingGeometryType_currentIndexChanged(QString )
{
    if(ui->cb_BoundingGeometryType->currentText() == "Box"){
        ui->frame_GeometryBoundingCylinder->hide();
        ui->frame_GeometryBoundingBox->show();
    }
    else if(ui->cb_BoundingGeometryType->currentText() == "Cylinder"){
        ui->frame_GeometryBoundingBox->hide();
        ui->frame_GeometryBoundingCylinder->show();
    }
}

///////////////////////////// Checking Bounding Input //////////////////////////////////////////

void MainWindow::on_txt_Min_X_Bounding_editingFinished()
{
    bool result;
    double a = ui->txt_Min_X_Bounding->text().toFloat(&result);
    if(result ==false)
    {
        ui->txt_Min_X_Bounding->undo();
        return;
    }
}
void MainWindow::on_txt_Min_Y_Bounding_editingFinished()
{
    bool result;
    double a = ui->txt_Min_Y_Bounding->text().toFloat(&result);
    if(result ==false)
    {
        ui->txt_Min_Y_Bounding->undo();
        return;
    }
}
void MainWindow::on_txt_Min_Z_Bounding_editingFinished()
{
    bool result;
    double a = ui->txt_Min_Z_Bounding->text().toFloat(&result);
    if(result ==false)
    {
        ui->txt_Min_Z_Bounding->undo();
        return;
    }
}
void MainWindow::on_txt_Max_X_Bounding_editingFinished()
{
    bool result;
    double a = ui->txt_Max_X_Bounding->text().toFloat(&result);
    if(result ==false)
    {
        ui->txt_Max_X_Bounding->undo();
        return;
    }
}
void MainWindow::on_txt_Max_Y_Bounding_editingFinished()
{
    bool result;
    double a = ui->txt_Max_Y_Bounding->text().toFloat(&result);
    if(result ==false)
    {
        ui->txt_Max_Y_Bounding->undo();
        return;
    }
}
void MainWindow::on_txt_Max_Z_Bounding_editingFinished()
{
    bool result;
    double a = ui->txt_Max_Z_Bounding->text().toFloat(&result);
    if(result ==false)
    {
        ui->txt_Max_Z_Bounding->undo();
        return;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////  Mesh Area  ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_txt_X_textChanged(QString value)
{
    bool a;
    if(value == "")
        return;
    value.toFloat(&a);
    if(!a)
    {
        ui->txt_X->undo();
        QMessageBox::critical(this,"Error","Please input a valid value!");
        return;
    }
    if(mesh->blockd->vertice.n > 5)
    {
        float delta = (mesh->blockd->vertice.v[6].x - mesh->blockd->vertice.v[0].x)/value.toFloat();
        ui->txt_DeltaX->setText(QString::number(delta,'f',2));
    }
}

void MainWindow::on_txt_Y_textChanged(QString value)
{
    bool a;
    if(value == "")
        return;
    value.toFloat(&a);
    if(!a)
    {
        ui->txt_Y->undo();
        QMessageBox::critical(this,"Error","Please input a valid value!");
        return;
    }
    if(mesh->blockd->vertice.n > 5)
    {
        float delta = (mesh->blockd->vertice.v[6].y - mesh->blockd->vertice.v[0].y)/value.toFloat();
        ui->txt_DeltaY->setText(QString::number(delta,'f',2));
    }
}

void MainWindow::on_txt_Z_textChanged(QString value)
{
    bool a;
    if(value == "")
        return;
    value.toFloat(&a);
    if(!a)
    {
        ui->txt_Z->undo();
        QMessageBox::critical(this,"Error","Please input a valid value!");
        return;
    }
    if(mesh->blockd->vertice.n > 5)
    {
        float delta = (mesh->blockd->vertice.v[6].z - mesh->blockd->vertice.v[0].z)/value.toFloat();
        ui->txt_DeltaZ->setText(QString::number(delta,'f',2));
    }
}

void MainWindow::on_btn_RefineBase_clicked()
{
    mesh->blockd->gen_Bounding.element.x = ui->txt_X->text().toFloat();
    mesh->blockd->gen_Bounding.element.y = ui->txt_Y->text().toFloat();
    mesh->blockd->gen_Bounding.element.z = ui->txt_Z->text().toFloat();

    ui->txt_Log->appendPlainText("Settings for base mesh have been save");
}

void MainWindow::on_btn_MeshBounding_clicked()
{
    flag_btnBounding_Click =true;
    flag_btnSurGeometry_Click =false;
    flag_btnSurface_Click = false;
    flag_btnVolume_Click = false;
    flag_Item_Face_Click = false;
    flag_btnSpecific_Click =false;
    flag_btn_MeshLayer_Click =false;
    //hide frame Specific

    ui->frame_MeshRefinement->hide();
    ui->frame_MeshVolume->hide();
    ui->frame_MeshLayer->hide();
    ui->frame_MeshBounding->show();

    //remove first
    Remove_All_Face();

    //add list faces
    for(int i =0; i< mesh->blockd->boundMesh.n; i++)
    {
        AddFaceToList(mesh->blockd->boundMesh.bounsType[i].name);
    }
    ui->txt_X->setText(QString::number(mesh->blockd->gen_Bounding.element.x));
    ui->txt_Y->setText(QString::number(mesh->blockd->gen_Bounding.element.y));
    ui->txt_Z->setText(QString::number(mesh->blockd->gen_Bounding.element.z));
}
void MainWindow::on_btn_MeshSurface_clicked()
{
    flag_btnSurface_Click =true;
    flag_btnBounding_Click =false;
    flag_btnVolume_Click = false;
    flag_btnSurGeometry_Click =false;
    flag_Item_Face_Click = false;
    flag_btn_MeshLayer_Click =false;
    ui->frame_MeshVolume->hide();
    ui->frame_MeshSurface->show();
    ui->txt_Level_Min_Surface_Refine->setText("");
    ui->txt_Level_Max_Surface_Refine->setText("");
    Remove_All_Face();
    for(int i=0; i< mesh->snappyd->gBox.n; i++)
    {
        AddFaceToList(mesh->snappyd->gBox.boxes[i].name);
    }
    for(int i=0; i< mesh->snappyd->gCylin.n; i++)
    {
        AddFaceToList(mesh->snappyd->gCylin.cylins[i].name);
    }
    for(int i=0; i< mesh->snappyd->gSphere.n; i++)
    {
        AddFaceToList(mesh->snappyd->gSphere.sphere[i].name);
    }
    for(int i=0; i< mesh->snappyd->gUserDefine.n; i++)
    {
        AddFaceToList(mesh->snappyd->gUserDefine.user_Defines[i].name);
    }
    //Define mesh refine distance
    int tbSize = ui->tb_MeshRefineAroundSurface->width() - ui->tb_MeshRefineAroundSurface->verticalHeader()->width();
    ui->tb_MeshRefineAroundSurface->setColumnWidth(0,2*tbSize/5);
    ui->tb_MeshRefineAroundSurface->setColumnWidth(1,2*tbSize/5);
    ui->tb_MeshRefineAroundSurface->setColumnWidth(2,0.7*tbSize/5);

}
void MainWindow::on_btn_MeshVolume_clicked()
{
    flag_btnSurface_Click =false;
    flag_btnBounding_Click =false;
    flag_btnSurGeometry_Click =false;
    flag_btnVolume_Click = true;
    flag_Item_Face_Click = false;
    flag_btn_MeshLayer_Click =false;
    ui->frame_MeshSurface->hide();
    ui->frame_MeshVolume->show();
    ui->txt_Level_Volume->setText("");
    Remove_All_Face();
    for(int i=0; i< mesh->snappyd->gBoxRegion.n; i++)
    {
        AddFaceToList(mesh->snappyd->gBoxRegion.boxes[i].name);
    }
    for(int i=0; i< mesh->snappyd->gCylinRegion.n; i++)
    {
        AddFaceToList(mesh->snappyd->gCylinRegion.cylins[i].name);
    }
    ui->cb_MeshVolumeMode->setCurrentIndex(0);
}

void MainWindow::on_btn_MeshRefineSurface_clicked()
{
    if(AddUserDefine() && AddSurfaceRegionBox())
        ui->txt_Log->appendPlainText("Defining of "+ ui->tb_boundary->currentItem()->text() +" surface has been done");
}
void MainWindow::on_btn_MeshRefineVolume_clicked()
{
    QString volumeName = ui->txt_GeometryVolumeName->text();
    if(!CheckNameValid(volumeName))
        return;
    //Save value for each type
    if(ui->cb_VolumeType->currentText() == "Box")
    {
       //define box type
        if(ui->txt_Max_Box_Vol_X->text()=="" || ui->txt_Max_Box_Vol_Y->text()=="" || ui->txt_Max_Box_Vol_Z->text()=="" ||
           ui->txt_Min_Box_Vol_X->text()=="" || ui->txt_Min_Box_Vol_Y->text()=="" || ui->txt_Min_Box_Vol_Z->text()=="")
        {
            QMessageBox::information(this,"Error","Values are incorrect!");
            return;
        }
        else
        {
            bool a,b,c,d,e,k;
            float x1 = ui->txt_Min_Box_Vol_X->text().toFloat(&a);
            float x2 = ui->txt_Max_Box_Vol_X->text().toFloat(&b);
            float y1 = ui->txt_Min_Box_Vol_Y->text().toFloat(&c);
            float y2 = ui->txt_Max_Box_Vol_Y->text().toFloat(&d);
            float z1 = ui->txt_Min_Box_Vol_Z->text().toFloat(&e);
            float z2 = ui->txt_Max_Box_Vol_Z->text().toFloat(&k);

            if(!a || !b || !c || !d ||!d || !e || !k)
            {
                QMessageBox::information(this,"Error","Values are incorrect!");
                return;
            }

            GeomeBoxRegion *gBoxRegion = &mesh->snappyd->gBoxRegion;
            int index = gBoxRegion->n;
            gBoxRegion->n = index + 1;
            gBoxRegion->boxes.resize(gBoxRegion->n);
            gBoxRegion->boxes[index].name = volumeName;
            gBoxRegion->boxes[index].type = ui->cb_VolumeType->currentText();
            gBoxRegion->boxes[index].max.x=x2;
            gBoxRegion->boxes[index].max.y=y2;
            gBoxRegion->boxes[index].max.z=z2;
            gBoxRegion->boxes[index].min.x=x1;
            gBoxRegion->boxes[index].min.y=y1;
            gBoxRegion->boxes[index].min.z=z1;

            RefinementRegions *refi_Reg = &gBoxRegion->refi_Reg;
            index = refi_Reg->n;
            refi_Reg->n = index + 1;
            refi_Reg->region.resize(refi_Reg->n);
            refi_Reg->region[index].name = volumeName;
            refi_Reg->region[index].mode = ui->cb_MeshVolumeMode->currentText();
            refi_Reg->region[index].lv1 = 100000;
            refi_Reg->region[index].lv2 = ui->txt_Level_Volume->text().toFloat();


            int plus = mesh->snappyd->points.size();
            mesh->snappyd->points.resize(8+plus);
            mesh->snappyd->points[0+plus] = new  float[3];
            mesh->snappyd->points[0+plus][0] = x1;
            mesh->snappyd->points[0+plus][1] = y1;
            mesh->snappyd->points[0+plus][2] = z1;


            mesh->snappyd->points[1+plus] = new  float[3];
            mesh->snappyd->points[1+plus][0] = x2;
            mesh->snappyd->points[1+plus][1] = y1;
            mesh->snappyd->points[1+plus][2] = z1;

            mesh->snappyd->points[2+plus] = new  float[3];
            mesh->snappyd->points[2+plus][0] = x2;
            mesh->snappyd->points[2+plus][1] = y2;
            mesh->snappyd->points[2+plus][2] = z1;

            mesh->snappyd->points[3+plus] = new  float[3];
            mesh->snappyd->points[3+plus][0] = x1;
            mesh->snappyd->points[3+plus][1] = y2;
            mesh->snappyd->points[3+plus][2] = z1;

            mesh->snappyd->points[4+plus] = new  float[3];
            mesh->snappyd->points[4+plus][0] = x1;
            mesh->snappyd->points[4+plus][1] = y1;
            mesh->snappyd->points[4+plus][2] = z2;

            mesh->snappyd->points[5+plus] = new  float[3];
            mesh->snappyd->points[5+plus][0] = x2;
            mesh->snappyd->points[5+plus][1] = y1;
            mesh->snappyd->points[5+plus][2] = z2;

            mesh->snappyd->points[6+plus] = new  float[3];
            mesh->snappyd->points[6+plus][0] = x2;
            mesh->snappyd->points[6+plus][1] = y2;
            mesh->snappyd->points[6+plus][2] = z2;

            mesh->snappyd->points[7+plus] = new  float[3];
            mesh->snappyd->points[7+plus][0] = x1;
            mesh->snappyd->points[7+plus][1] = y2;
            mesh->snappyd->points[7+plus][2] = z2;

            int starend[2];
            starend[0] = mesh->snappyd->faces.size();

            mesh->snappyd->faces.resize(6+starend[0]);
            mesh->snappyd->faces[0+starend[0]] = new int[4];
            mesh->snappyd->faces[0+starend[0]][0] = 3 + plus;
            mesh->snappyd->faces[0+starend[0]][1] = 7 + plus;
            mesh->snappyd->faces[0+starend[0]][2] = 6 + plus;
            mesh->snappyd->faces[0+starend[0]][3] = 2 + plus;

            mesh->snappyd->faces[1+starend[0]] = new int[4];
            mesh->snappyd->faces[1+starend[0]][0] = 1 + plus;
            mesh->snappyd->faces[1+starend[0]][1] = 5 + plus;
            mesh->snappyd->faces[1+starend[0]][2] = 4 + plus;
            mesh->snappyd->faces[1+starend[0]][3] = 0 + plus;

            mesh->snappyd->faces[2+starend[0]] = new int[4];
            mesh->snappyd->faces[2+starend[0]][0] = 4 + plus;
            mesh->snappyd->faces[2+starend[0]][1] = 5 + plus;
            mesh->snappyd->faces[2+starend[0]][2] = 6 + plus;
            mesh->snappyd->faces[2+starend[0]][3] = 7 + plus;

            mesh->snappyd->faces[3+starend[0]] = new int[4];
            mesh->snappyd->faces[3+starend[0]][0] = 0 + plus;
            mesh->snappyd->faces[3+starend[0]][1] = 3 + plus;
            mesh->snappyd->faces[3+starend[0]][2] = 2 + plus;
            mesh->snappyd->faces[3+starend[0]][3] = 1 + plus;

            mesh->snappyd->faces[4+starend[0]] = new int[4];
            mesh->snappyd->faces[4+starend[0]][0] = 0 + plus;
            mesh->snappyd->faces[4+starend[0]][1] = 4 + plus;
            mesh->snappyd->faces[4+starend[0]][2] = 7 + plus;
            mesh->snappyd->faces[4+starend[0]][3] = 3 + plus;

            mesh->snappyd->faces[5+starend[0]] = new int[4];
            mesh->snappyd->faces[5+starend[0]][0] = 2 + plus;
            mesh->snappyd->faces[5+starend[0]][1] = 6 + plus;
            mesh->snappyd->faces[5+starend[0]][2] = 5 + plus;
            mesh->snappyd->faces[5+starend[0]][3] = 1 + plus;

            starend[1] = mesh->snappyd->faces.size();

            mesh->snappyd->facezones.resize( mesh->snappyd->facezones.size()+1);
            mesh->snappyd->facezones[mesh->snappyd->facezones.size()-1]= new int[2];
            mesh->snappyd->facezones[mesh->snappyd->facezones.size()-1][0]= starend[0];
            mesh->snappyd->facezones[mesh->snappyd->facezones.size()-1][1]= starend[1];

            mesh->snappyd->facetype.resize(mesh->snappyd->facetype.size() + 1);
            mesh->snappyd->facetype[mesh->snappyd->facetype.size() - 1] = 0;

            mesh->snappyd->facename.append(volumeName);

            QStringList views = mesh->GetViewList();
            views.append(volumeName);
            mesh->SetViewList(views);

            AddFaceToList(volumeName);

            mesh->updateGL();
            ui->txt_Log->appendPlainText("Defining of "+ volumeName +" box has been done");
        }

    }
    else if(ui->cb_VolumeType->currentText() == "Cylinder")
    {
       //define cylinder type
        if(ui->txt_P1_Cyl_Vol_X->text()=="" || ui->txt_P1_Cyl_Vol_Y->text()=="" || ui->txt_P1_Cyl_Vol_Z->text()=="" || ui->txt_P2_Cyl_Vol_X->text()==""
           ||ui->txt_P2_Cyl_Vol_Y->text()=="" || ui->txt_P2_Cyl_Vol_Z->text()=="" || ui->txt_Radius_Cyl_Vol->text()=="")
        {
            QMessageBox::information(this,"Error","Values are invalid!");
            return;
        }
        else
        {
            bool a,b,c,d,e,k,l;
            float x1 = ui->txt_P1_Cyl_Vol_X->text().toFloat(&a);
            float x2 = ui->txt_P2_Cyl_Vol_X->text().toFloat(&b);
            float y1 = ui->txt_P1_Cyl_Vol_Y->text().toFloat(&c);
            float y2 = ui->txt_P2_Cyl_Vol_Y->text().toFloat(&d);
            float z1 = ui->txt_P1_Cyl_Vol_Z->text().toFloat(&e);
            float z2 = ui->txt_P2_Cyl_Vol_Z->text().toFloat(&k);

            float radius_cyl = ui->txt_Radius_Cyl_Vol->text().toFloat(&l);
            if(!a || !b || !c || !d ||!d || !e || !k || !l)
            {
                QMessageBox::information(this,"Error","Values are invalid!");
                return;
            }
            GeomeCylinderRegion *gCylinRegion = &mesh->snappyd->gCylinRegion;

            int index = gCylinRegion->n;
            gCylinRegion->n = index + 1;
            gCylinRegion->cylins.resize(gCylinRegion->n);
            gCylinRegion->cylins[index].name=volumeName;
            gCylinRegion->cylins[index].type=ui->cb_VolumeType->currentText();
            gCylinRegion->cylins[index].point2.x=x2;
            gCylinRegion->cylins[index].point2.y=y2;
            gCylinRegion->cylins[index].point2.z=z2;
            gCylinRegion->cylins[index].point1.x=x1;
            gCylinRegion->cylins[index].point1.y=y1;
            gCylinRegion->cylins[index].point1.z=z1;
            gCylinRegion->cylins[index].radius= radius_cyl;

            RefinementRegions *refi_Reg = &gCylinRegion->refi_Reg;
            index = refi_Reg->n;
            refi_Reg->n = index + 1;
            refi_Reg->region.resize(refi_Reg->n);
            refi_Reg->region[index].name = volumeName;
            refi_Reg->region[index].mode = ui->cb_MeshVolumeMode->currentText();
            refi_Reg->region[index].lv1 = 100000;
            refi_Reg->region[index].lv2 = ui->txt_Level_Volume->text().toFloat();

            AddFaceToList(volumeName);

            QStringList views = mesh->GetViewList();
            views.append(volumeName);
            mesh->SetViewList(views);

            mesh->updateGL();
            ui->txt_Log->appendPlainText("Defining  of "+ volumeName +" cylinder has been done");
        }

    }
    else if(ui->cb_VolumeType->currentText() == "Sphere")
    {
       //define sphere type
        if(ui->txt_P_Sphe_Vol_X->text()=="" || ui->txt_P_Sphe_Vol_Y->text()=="" || ui->txt_P_Sphe_Vol_Z->text()=="" || ui->txt_Radius_Sphe_Vol->text()=="")
        {
            QMessageBox::information(this,"Error","Values are invalid!");
            return;
        }
        else
        {
            bool a,b,c,d;
            float x = ui->txt_P_Sphe_Vol_X->text().toFloat(&a);
            float y = ui->txt_P_Sphe_Vol_Y->text().toFloat(&b);
            float z = ui->txt_P_Sphe_Vol_Z->text().toFloat(&c);

            float radius_sph = ui->txt_Radius_Sphe_Vol->text().toFloat(&d);
            if(!a || !b || !c || !d)
            {
                QMessageBox::information(this,"Error","Values are invalid!");
                return;
            }

            GeomeSphereRegion *gSphereRegion = &mesh->snappyd->gSphereRegion;

            int index = gSphereRegion->n;
            gSphereRegion->n = index + 1;
            gSphereRegion->sphere.resize(gSphereRegion->n);
            gSphereRegion->sphere[index].name=volumeName;
            gSphereRegion->sphere[index].type=ui->cb_VolumeType->currentText();
            gSphereRegion->sphere[index].centre.x=x;
            gSphereRegion->sphere[index].centre.y=y;
            gSphereRegion->sphere[index].centre.z=z;
            gSphereRegion->sphere[index].radius= radius_sph;

            RefinementRegions *refi_Reg = &gSphereRegion->refi_Reg;
            index = refi_Reg->n;
            refi_Reg->n = index + 1;
            refi_Reg->region.resize(refi_Reg->n);
            refi_Reg->region[index].name = volumeName;
            refi_Reg->region[index].mode = ui->cb_MeshVolumeMode->currentText();
            refi_Reg->region[index].lv1 = 100000;
            refi_Reg->region[index].lv2 = ui->txt_Level_Volume->text().toFloat();

            AddFaceToList(volumeName);

            QStringList views = mesh->GetViewList();
            views.append(volumeName);
            mesh->SetViewList(views);

            mesh->updateGL();
            ui->txt_Log->appendPlainText("Defining of "+ volumeName +" sphere has been done");
        }
    }
}

void MainWindow::on_btn_MeshLayer_clicked()
{
    flag_btn_MeshLayer_Click =true;
    flag_btnBounding_Click =false;
    flag_btnSurGeometry_Click =false;
    flag_btnSurface_Click = false;
    flag_btnVolume_Click = false;
    flag_Item_Face_Click = false;
    flag_btnSpecific_Click =false;
    ui->frame_MeshBounding->hide();
    ui->frame_MeshRefinement->hide();
    ui->frame_MeshVolume->hide();
    ui->frame_MeshLayer->show();

    //Cong edit: add layer to suraces
    Remove_All_Face();
    for(int i = 0; i<mesh->snappyd->gBox.n; i++)
    {
        AddFaceToList(mesh->snappyd->gBox.boxes[i].name);
    }
    for(int i = 0; i<mesh->snappyd->gCylin.n; i++)
    {
        AddFaceToList(mesh->snappyd->gCylin.cylins[i].name);
    }
    for(int i = 0; i<mesh->snappyd->gSphere.n; i++)
    {
        AddFaceToList(mesh->snappyd->gSphere.sphere[i].name);
    }
    for(int i = 0; i<mesh->snappyd->gUserDefine.n; i++)
    {
        AddFaceToList(mesh->snappyd->gUserDefine.user_Defines[i].name);
        for(int j = 0 ; j < mesh->snappyd->gUserDefine.refi_Sur.surfaces[i].n; j++)
        {
            AddFaceToList(mesh->snappyd->gUserDefine.user_Defines[i].name + "_" + mesh->snappyd->gUserDefine.refi_Sur.surfaces[i].regionSTLs[j].name);
        }
    }
    if(!mesh->blockd->isAutomatic)
    {
        for(int i = 0; i< mesh->blockd->boundMesh.n; i++)
        {
            AddFaceToList(mesh->blockd->boundMesh.bounsType[i].name);
        }
    }
}
void MainWindow::on_btn_RefineLayer_clicked()
{
    if(flag_Item_Face_Click == false)
        return;
    //Cong edit: add layer to suraces
    bool is_Number;
    int layer = ui->txt_Layer_Bounding_2->text().toInt(&is_Number);
    if(is_Number==false)
    {
        QMessageBox::information(this,"Error","Values are incorrect!");
        return;
    }
    int n = mesh->snappyd->add_Layers_Controls.nLayer;
    //if exists, update
    for(int i=0;i< n; i++)
    {
        if(mesh->snappyd->add_Layers_Controls.layers[i].name == ui->tb_boundary->currentItem()->text())
        {
            mesh->snappyd->add_Layers_Controls.layers[i].nSurfaceLayers =layer;
            ui->txt_Log->appendPlainText("Settings for layer of " + ui->tb_boundary->currentItem()->text() + " have been done");
            return;
        }
    }
    //add new
    mesh->snappyd->add_Layers_Controls.nLayer = n + 1;
    mesh->snappyd->add_Layers_Controls.layers.resize(n+1);
    mesh->snappyd->add_Layers_Controls.layers[n].name = ui->tb_boundary->currentItem()->text();
    mesh->snappyd->add_Layers_Controls.layers[n].nSurfaceLayers = layer;
    ui->txt_Log->appendPlainText("Settings for layer of " + ui->tb_boundary->currentItem()->text() + " have been done");
}

void MainWindow::on_btn_MeshSurfaceGeneral_clicked()
{
    W_MeshSurfaceRefineGeneral *w_general;
    w_general = new W_MeshSurfaceRefineGeneral(mesh);
    w_general->exec();
    if(w_general->result())
        ui->txt_Log->appendPlainText("Settings for general value of layers have been done");
}
void MainWindow::on_ckb_MeshVolumeCellZone_clicked(bool checked)
{
    if(checked)
        ui->groupBox_MeshVolumeCellZone->show();
    else
        ui->groupBox_MeshVolumeCellZone->hide();
}

void MainWindow::on_tb_boundary_clicked(QModelIndex index)
{
    flag_Item_Face_Click =true;
}
void MainWindow::on_btn_MeshRefinement_clicked()
{
    ui->frame_MeshBounding->hide();
    ui->frame_MeshLayer->hide();
    ui->frame_MeshVolume->hide();
    ui->frame_MeshRefinement->show();
}
void MainWindow::on_cb_VolumeType_currentIndexChanged(QString )
{
    if(ui->cb_VolumeType->currentText() == "Box")
    {
        ui->frame_DefineVolumeCylinder->hide();
        ui->frame_DefineVolumeSphere->hide();
        ui->frame_DefineVolumeBox->show();
    }
    else if(ui->cb_VolumeType->currentText() == "Cylinder")
    {
        ui->frame_DefineVolumeBox->hide();
        ui->frame_DefineVolumeSphere->hide();
        ui->frame_DefineVolumeCylinder->show();
    }
    else if(ui->cb_VolumeType->currentText() == "Sphere")
    {
        ui->frame_DefineVolumeBox->hide();
        ui->frame_DefineVolumeCylinder->hide();
        ui->frame_DefineVolumeSphere->show();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Boundary Area //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_btn_CreateBoundary_clicked()
{
    if(CheckNameValid(ui->txt_BoundaryName->text()))
    {
        if(ui->cb_BoundaryType->currentIndex() == 0)
        {
            QMessageBox::critical(this,"Error","Please select type of boundary!");
            return;
        }
        foreach(Boundary b,mesh->patchDict->boundaries)
        {
            if(b.name == ui->txt_BoundaryName->text())
            {
                QMessageBox::critical(this,"Error","This name is already existed...!");
                return;
            }
        }
    }
    else
        return;
    QStringList surfaces;
    foreach(QTableWidgetItem *s, ui->tb_MeshSurface->selectedItems())
    {
        surfaces.append(s->text());
        listSurfaces.removeOne(s->text());
    }
    if(surfaces.size() > 0)
    {
        Boundary b;
        b.name = ui->txt_BoundaryName->text();
        b.patches.surfaces = surfaces;
        b.patchInfo.type = ui->cb_BoundaryType->currentText();
        mesh->patchDict->boundaries.append(b);
        LoadBoundaryControlItems();
        ui->txt_Log->appendPlainText("These surfaces have been merged to boundary " + b.name);
    }
    else
        QMessageBox::critical(this,"Error","Please select as least as once surface!");
}
void MainWindow::on_btn_DeleteBoundary_clicked()
{
    if(ui->tb_MeshBoundary->selectedItems().count() > 0)
    {
        foreach(QTableWidgetItem *b, ui->tb_MeshBoundary->selectedItems())
        {
            for(int i = 0; i < mesh->patchDict->boundaries.size(); i++)
            {
                if(mesh->patchDict->boundaries[i].name == b->text())
                {
                    foreach(QString surface,mesh->patchDict->boundaries[i].patches.surfaces)
                    {
                        listSurfaces.append(surface);
                    }
                    mesh->patchDict->boundaries.removeAt(i);
                }
            }
        }
        LoadBoundaryControlItems();
        if(ui->tb_MeshBoundary->selectedItems().count() > 1)
            ui->txt_Log->appendPlainText("These boundaries have been deleted");
        else
            ui->txt_Log->appendPlainText("This boundary have been deleted");
    }
    else
        QMessageBox::critical(this,"Error","Please select as least as once boundary!");
}

////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////  Create Mesh  //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_btn_CreateMesh_clicked()
{
    bool flag_YesNo = false;
    if(path_Open != "")
    {
        switch(QMessageBox::question(this,"IAE Bresil","Do you want to ovewrite this case?",QMessageBox::Yes |
                                      QMessageBox::No, QMessageBox::NoButton))
        {
            case QMessageBox::Yes:
            {
                flag_YesNo = true;
                break;
            }
            case QMessageBox::No:
                break;
            default:
                break;
        }
    }
    if(flag_YesNo == false)
    {
        QString saveCase = QFileDialog::getSaveFileName(this);
        if(saveCase != "")
        {
            if(!OpenFoam::CopyDir("Data/OpenFoamDefault",saveCase))
            {
                ui->txt_Log->appendPlainText("Can't copy default value to " + saveCase);
                return;
            }
            QFile(saveCase + "/constant/polyMesh/blockMeshDict").remove();
            OpenFoam::RemoveDir(saveCase + QDir::separator() + "0");
            QDir a;
            a.mkdir(saveCase + QDir::separator() + "0");

            mesh->blockd->Write_Block_Dmesh(saveCase + "/constant/polyMesh/");
            mesh->snappyd->Write_Snappy(saveCase + "/system/");
            if(!QDir(saveCase + "/constant/triSurface").exists())
                QDir().mkdir(saveCase + "/constant/triSurface");
            for(int i=0; i<mesh->snappyd->gUserDefine.n; i++)
            {
                QFile(mesh->snappyd->gUserDefine.user_Defines[i].direction).copy(saveCase + "/constant/triSurface/" + mesh->snappyd->gUserDefine.user_Defines[i].name_file);
            }
            if(mesh->patchDict->boundaries.size() > 0)
            {
                mesh->patchDict->WritePatchDict(saveCase + "/system");
            }
            ui->txt_Log->appendPlainText("Creating mesh...!");
            createrThread = new MyThread();
            connect(createrThread,SIGNAL(changed(QString)),this,SLOT(Thread_Changed(QString)));
            OpenFoam::SetOpenFOAMPath(saveCase);
            createrThread->SetCommand("blockMesh");
            createrThread->ThreadName("blockMesh");
            createrThread->start();
            while(createrThread->isRunning())
            {
                QApplication::processEvents();
            }
            for(int i = 0; i < mesh->snappyd->gUserDefine.refi_Fea.n; i++)
            {
                createrThread->SetCommand("surfaceFeatureExtract -includedAngle " + QString::number(mesh->snappyd->gUserDefine.refi_Fea.feature[i].angle) +
                                          " constant/triSurface/" + mesh->snappyd->gUserDefine.refi_Fea.feature[i].name + ".stl " + mesh->snappyd->gUserDefine.refi_Fea.feature[i].name);
                createrThread->ThreadName("blockMesh");
                createrThread->start();
                while(createrThread->isRunning())
                {
                    QApplication::processEvents();
                }
            }
            createrThread->SetSubCommand("-overwrite",2);
            createrThread->SetCommand("snappyHexMesh");
            createrThread->ThreadName("snappyHexMesh");
            createrThread->start();
            while(createrThread->isRunning())
            {
                QApplication::processEvents();
            }
            createrThread->SetSubCommand("-overwrite",2);
            createrThread->SetCommand("createPatch");
            createrThread->ThreadName("createPatch");
            createrThread->start();
            while(createrThread->isRunning())
            {
                QApplication::processEvents();
            }

            //save file setting bounding
            QStringList list;
            QString str_para;
            if(ui->cb_BoundingType->currentIndex() ==0)
            {
                str_para = "automatic";
                list.append(str_para);
            }
            else if(ui->cb_BoundingType->currentIndex() ==1)
            {
                QString x1 = ui->txt_DistanceMinX->text();
                QString y1 = ui->txt_DistanceMinY->text();
                QString z1 = ui->txt_DistanceMinZ->text();

                QString x2 = ui->txt_DistanceMaxX->text();
                QString y2 = ui->txt_DistanceMaxY->text();
                QString z2 = ui->txt_DistanceMaxZ->text();

                str_para = "distance " + x1 + " " + y1 + " " + z1 + " " + x2 + " " + y2 + " " + z2;
                list.append(str_para);
            }
            else if(ui->cb_BoundingType->currentIndex() ==2)
            {
                str_para = "define";
                list.append(str_para);
            }
            mesh->blockd->Write_Bounding(saveCase + "/constant/polyMesh/setting.dat", list);

        }
    }
    else
    {
        //use current folder
        QFile(path_Open + "/constant/polyMesh/blockMeshDict").remove();

        mesh->blockd->Write_Block_Dmesh(path_Open + "/constant/polyMesh/");

        QFile(path_Open + "/system/snappyHexMeshDict").remove();
        mesh->snappyd->Write_Snappy(path_Open + "/system/");
        if(!QDir(path_Open + "/constant/triSurface").exists())
            QDir().mkdir(path_Open + "/constant/triSurface");
        for(int i=0; i<mesh->snappyd->gUserDefine.n; i++)
        {
            QFile(mesh->snappyd->gUserDefine.user_Defines[i].direction).copy(path_Open + "/constant/triSurface/" + mesh->snappyd->gUserDefine.user_Defines[i].name_file);
        }
        if(mesh->patchDict->boundaries.size() > 0)
        {
            QFile(path_Open + "/system/createPatchDict").remove();
            mesh->patchDict->WritePatchDict(path_Open + "/system");
        }
        ui->txt_Log->appendPlainText("Creating mesh...!");
        createrThread = new MyThread();
        connect(createrThread,SIGNAL(changed(QString)),this,SLOT(Thread_Changed(QString)));
        OpenFoam::SetOpenFOAMPath(path_Open);
        createrThread->SetCommand("blockMesh");
        createrThread->ThreadName("blockMesh");
        createrThread->start();
        while(createrThread->isRunning())
        {
            QApplication::processEvents();
        }
        for(int i = 0; i < mesh->snappyd->gUserDefine.refi_Fea.n; i++)
        {
            createrThread->SetCommand("surfaceFeatureExtract -includedAngle " + QString::number(mesh->snappyd->gUserDefine.refi_Fea.feature[i].angle) +
                                      " constant/triSurface/" + mesh->snappyd->gUserDefine.refi_Fea.feature[i].name + ".stl " + mesh->snappyd->gUserDefine.refi_Fea.feature[i].name);
            createrThread->ThreadName("blockMesh");
            createrThread->start();
            while(createrThread->isRunning())
            {
                QApplication::processEvents();
            }
        }
        createrThread->SetSubCommand("-overwrite",2);
        createrThread->SetCommand("snappyHexMesh");
        createrThread->ThreadName("snappyHexMesh");
        createrThread->start();
        while(createrThread->isRunning())
        {
            QApplication::processEvents();
        }
        createrThread->SetSubCommand("-overwrite",2);
        createrThread->SetCommand("createPatch");
        createrThread->ThreadName("createPatch");
        createrThread->start();
        while(createrThread->isRunning())
        {
            QApplication::processEvents();
        }

        //save file setting bounding
        QStringList list;
        QString str_para;
        if(ui->cb_BoundingType->currentIndex() ==0)
        {
            str_para = "automatic";
            list.append(str_para);
        }
        else if(ui->cb_BoundingType->currentIndex() ==1)
        {
            QString x1 = ui->txt_DistanceMinX->text();
            QString y1 = ui->txt_DistanceMinY->text();
            QString z1 = ui->txt_DistanceMinZ->text();

            QString x2 = ui->txt_DistanceMaxX->text();
            QString y2 = ui->txt_DistanceMaxY->text();
            QString z2 = ui->txt_DistanceMaxZ->text();

            str_para = "distance " + x1 + " " + y1 + " " + z1 + " " + x2 + " " + y2 + " " + z2;
            list.append(str_para);
        }
        else if(ui->cb_BoundingType->currentIndex() ==2)
        {
            str_para = "define";
            list.append(str_para);
        }
        QFile(path_Open + "/constant/polyMesh/setting.dat").remove();
        mesh->blockd->Write_Bounding(path_Open + "/constant/polyMesh/setting.dat", list);
    }
}

void MainWindow::on_btn_GenerateLocation_clicked()
{
    if(SetLocation())
        ui->txt_Log->appendPlainText("Settings for location vetices have been done");
}

void MainWindow::on_btn_DeleteSurface_clicked()
{
    if(ui->tb_boundary->selectedItems().count() > 0)
    {
        //flag delete
        bool flag_delete = false;
        //Delete simple surface - Box
        QString name = ui->tb_boundary->currentItem()->text();
        int n  = mesh->snappyd->gBox.n;

        //Delele layer
        for(int j=0; j < mesh->snappyd->add_Layers_Controls.nLayer; j++)
        {
            if(mesh->snappyd->add_Layers_Controls.layers[j].name == name)
            {
                flag_delete = true;
                //remove layer
                mesh->snappyd->add_Layers_Controls.layers.remove(j);
                int n_size = mesh->snappyd->add_Layers_Controls.nLayer -1;
                mesh->snappyd->add_Layers_Controls.layers.resize(n_size);
                mesh->snappyd->add_Layers_Controls.nLayer =n_size;
            }
        }
        //Delete in list min max
        for(int k=0; k < mesh->snappyd->list_Surface_Min_Max.length(); k++)
        {
            if(mesh->snappyd->list_Surface_Min_Max[k].name_Surface == name)
            {
                //remove it
                mesh->snappyd->list_Surface_Min_Max.removeAt(k);
            }
        }
        //Update min max
        mesh->snappyd->FindMinMax(mesh->snappyd->list_Surface_Min_Max);
        //Delete Geometry And Refinements
        for(int i=0; i < n; i++)
        {
            if(mesh->snappyd->gBox.boxes[i].name ==name)
            {
                //remove box
                mesh->snappyd->gBox.boxes.remove(i);
                mesh->snappyd->gBox.boxes.resize(mesh->snappyd->gBox.n -1);
                mesh->snappyd->gBox.n = mesh->snappyd->gBox.n -1;
                //find surface
                for(int j=0; j < mesh->snappyd->gBox.refi_Sur.n; j++)
                {
                    if(mesh->snappyd->gBox.refi_Sur.surfaces[j].name == name)
                    {
                        //remove surface
                        mesh->snappyd->gBox.refi_Sur.surfaces.remove(j);
                        int n_size =  mesh->snappyd->gBox.refi_Sur.n-1;
                        mesh->snappyd->gBox.refi_Sur.surfaces.resize(n_size);
                        mesh->snappyd->gBox.refi_Sur.n = n_size;
                    }
                }
                //find region
                for(int j=0; j < mesh->snappyd->gBox.refi_Reg.n; j++)
                {
                    if(mesh->snappyd->gBox.refi_Reg.region[j].name == name)
                    {
                        //remove surface
                        mesh->snappyd->gBox.refi_Reg.region.remove(j);
                        int n_size = mesh->snappyd->gBox.refi_Reg.n-1;
                        mesh->snappyd->gBox.refi_Reg.region.resize(n_size);
                        mesh->snappyd->gBox.refi_Reg.n = n_size;

                    }
                }

                //remove row on table
                ui->txt_Log->appendPlainText("Successfully delete "+ name +" box");
                Remove_All_Face();
                for(int j=0; j< mesh->snappyd->gBox.n; j++)
                {
                    AddFaceToList(mesh->snappyd->gBox.boxes[j].name);
                }
                for(int j=0; j< mesh->snappyd->gCylin.n; j++)
                {
                    AddFaceToList(mesh->snappyd->gCylin.cylins[j].name);
                }
                for(int j=0; j< mesh->snappyd->gSphere.n; j++)
                {
                    AddFaceToList(mesh->snappyd->gSphere.sphere[j].name);
                }
                for(int j=0; j< mesh->snappyd->gUserDefine.n; j++)
                {
                    AddFaceToList(mesh->snappyd->gUserDefine.user_Defines[j].name);
                }
                listSurfaces.removeOne(name);
                ResetDrawBoxs();
                mesh->updateGL();
                if(flag_btnSurGeometry_Click)
                {
                    ui->txt_GeometrySurfaceFileStl->setText("");
                    ui->txt_GeometrySurfaceName->setText("");
                    ui->txt_Min_Box_Sur_X->setText("");
                    ui->txt_Min_Box_Sur_Y->setText("");
                    ui->txt_Min_Box_Sur_Z->setText("");

                    ui->txt_Max_Box_Sur_X->setText("");
                    ui->txt_Max_Box_Sur_Y->setText("");
                    ui->txt_Max_Box_Sur_Z->setText("");
                }
                if(flag_btnSurface_Click)
                {
                    ui->txt_Level_Min_Surface_Refine->setText("");
                    ui->txt_Level_Max_Surface_Refine->setText("");
                }
                if(flag_btn_MeshLayer_Click)
                {
                    ui->txt_Layer_Bounding_2->setText("");
                }
                return;
            }

        }

        //delete Cylinder
        GeomeCylinderSurface *gCylin = &mesh->snappyd->gCylin;
        n = gCylin->n;
        for(int i=0; i < n; i++)
        {
            if(gCylin->cylins[i].name == name)
            {
                //Delele Geometry
                gCylin->cylins.remove(i);
                gCylin->n = gCylin->n - 1;
                //find surface
                for(int j=0; j < gCylin->refi_Sur.n; j++)
                {
                    if(gCylin->refi_Sur.surfaces[j].name == name)
                    {
                        //remove surface
                        gCylin->refi_Sur.surfaces.remove(j);
                        int n_size =  gCylin->refi_Sur.n-1;
                        gCylin->refi_Sur.surfaces.resize(n_size);
                        gCylin->refi_Sur.n = n_size;
                    }
                }
                //find region
                for(int j=0; j < gCylin->refi_Reg.n; j++)
                {
                    if(gCylin->refi_Reg.region[j].name == name)
                    {
                        //remove surface
                        gCylin->refi_Reg.region.remove(j);
                        int n_size = gCylin->refi_Reg.n-1;
                        gCylin->refi_Reg.region.resize(n_size);
                        gCylin->refi_Reg.n = n_size;

                    }
                }

                //remove row on table
                ui->txt_Log->appendPlainText("Successfully delete "+ name +" box");
                Remove_All_Face();
                for(int j=0; j< mesh->snappyd->gBox.n; j++)
                {
                    AddFaceToList(mesh->snappyd->gBox.boxes[j].name);
                }
                for(int j=0; j< mesh->snappyd->gCylin.n; j++)
                {
                    AddFaceToList(mesh->snappyd->gCylin.cylins[j].name);
                }
                for(int j=0; j< mesh->snappyd->gSphere.n; j++)
                {
                    AddFaceToList(mesh->snappyd->gSphere.sphere[j].name);
                }
                for(int j=0; j< mesh->snappyd->gUserDefine.n; j++)
                {
                    AddFaceToList(mesh->snappyd->gUserDefine.user_Defines[j].name);
                }
                listSurfaces.removeOne(name);
                //ResetDrawBoxs();
                mesh->updateGL();
                if(flag_btnSurGeometry_Click)
                {
                    ui->txt_GeometrySurfaceFileStl->setText("");
                    ui->txt_GeometrySurfaceName->setText("");
                    ui->txt_P1_Cyl_Sur_X->setText("");
                    ui->txt_P1_Cyl_Sur_Y->setText("");
                    ui->txt_P1_Cyl_Sur_Z->setText("");

                    ui->txt_P2_Cyl_Sur_X->setText("");
                    ui->txt_P2_Cyl_Sur_Y->setText("");
                    ui->txt_P2_Cyl_Sur_Z->setText("");

                    ui->txt_Radius_Cyl_Sur->setText("");
                }
                if(flag_btnSurface_Click)
                {
                    ui->txt_Level_Min_Surface_Refine->setText("");
                    ui->txt_Level_Max_Surface_Refine->setText("");
                }
                if(flag_btn_MeshLayer_Click)
                {
                    ui->txt_Layer_Bounding_2->setText("");
                }
                return;
            }
        }

        //delete spheres
        GeomeSphereSurface *gSphere = &mesh->snappyd->gSphere;
        n = gSphere->n;
        for(int i=0; i < n; i++)
        {
            if(gSphere->sphere[i].name == name)
            {
                //Delele Geometry
                gSphere->sphere.remove(i);
                gSphere->n = gSphere->n - 1;
                //find surface
                for(int j=0; j < gSphere->refi_Sur.n; j++)
                {
                    if(gSphere->refi_Sur.surfaces[j].name == name)
                    {
                        //remove surface
                        gSphere->refi_Sur.surfaces.remove(j);
                        int n_size =  gSphere->refi_Sur.n-1;
                        gSphere->refi_Sur.surfaces.resize(n_size);
                        gSphere->refi_Sur.n = n_size;
                    }
                }
                //find region
                for(int j=0; j < gSphere->refi_Reg.n; j++)
                {
                    if(gSphere->refi_Reg.region[j].name == name)
                    {
                        //remove surface
                        gSphere->refi_Reg.region.remove(j);
                        int n_size = gSphere->refi_Reg.n-1;
                        gSphere->refi_Reg.region.resize(n_size);
                        gSphere->refi_Reg.n = n_size;

                    }
                }

                //remove row on table
                ui->txt_Log->appendPlainText("Successfully delete "+ name +" box");
                Remove_All_Face();
                for(int j=0; j< mesh->snappyd->gBox.n; j++)
                {
                    AddFaceToList(mesh->snappyd->gBox.boxes[j].name);
                }
                for(int j=0; j< mesh->snappyd->gCylin.n; j++)
                {
                    AddFaceToList(mesh->snappyd->gCylin.cylins[j].name);
                }
                for(int j=0; j< mesh->snappyd->gSphere.n; j++)
                {
                    AddFaceToList(mesh->snappyd->gSphere.sphere[j].name);
                }
                for(int j=0; j< mesh->snappyd->gUserDefine.n; j++)
                {
                    AddFaceToList(mesh->snappyd->gUserDefine.user_Defines[j].name);
                }
                listSurfaces.removeOne(name);
                mesh->updateGL();
                if(flag_btnSurGeometry_Click)
                {
                    ui->txt_GeometrySurfaceFileStl->setText("");
                    ui->txt_GeometrySurfaceName->setText("");
                    ui->txt_P_Sphe_Sur_X->setText("");
                    ui->txt_P_Sphe_Sur_Y->setText("");
                    ui->txt_P_Sphe_Sur_Z->setText("");
                    ui->txt_Radius_Sphe_Sur->setText("");
                }
                if(flag_btnSurface_Click)
                {
                    ui->txt_Level_Min_Surface_Refine->setText("");
                    ui->txt_Level_Max_Surface_Refine->setText("");
                }
                if(flag_btn_MeshLayer_Click)
                {
                    ui->txt_Layer_Bounding_2->setText("");
                }
                return;
            }
        }

        //Delete User Define
        n = mesh->snappyd->gUserDefine.n;
        for(int i=0; i < n; i++)
        {
            if(mesh->snappyd->gUserDefine.user_Defines[i].name == name)
            {
                //remove user define
                mesh->snappyd->gUserDefine.user_Defines.remove(i);
                mesh->snappyd->gUserDefine.user_Defines.resize(mesh->snappyd->gUserDefine.n -1);
                mesh->snappyd->gUserDefine.n = mesh->snappyd->gUserDefine.n -1;
                //find surface
                for(int j=0; j < mesh->snappyd->gUserDefine.refi_Sur.n; j++)
                {
                    if(mesh->snappyd->gUserDefine.refi_Sur.surfaces[j].name == name)
                    {
                        //remove surface
                        mesh->snappyd->gUserDefine.refi_Sur.surfaces.remove(j);
                        int n_size =  mesh->snappyd->gUserDefine.refi_Sur.n-1;
                        mesh->snappyd->gUserDefine.refi_Sur.surfaces.resize(n_size);
                        mesh->snappyd->gUserDefine.refi_Sur.n = n_size;
                    }
                }
                //find region
                for(int j=0; j < mesh->snappyd->gUserDefine.refi_Reg.n; j++)
                {
                    if(mesh->snappyd->gUserDefine.refi_Reg.region[j].name == name)
                    {
                        //remove surface
                        mesh->snappyd->gUserDefine.refi_Reg.region.remove(j);
                        int n_size = mesh->snappyd->gUserDefine.refi_Reg.n-1;
                        mesh->snappyd->gUserDefine.refi_Reg.region.resize(n_size);
                        mesh->snappyd->gUserDefine.refi_Reg.n = n_size;

                    }
                }
                //find feature
                for(int j=0; j < mesh->snappyd->gUserDefine.refi_Fea.n; j++)
                {
                    if(mesh->snappyd->gUserDefine.refi_Fea.feature[j].name == name)
                    {
                        //remove surface
                        mesh->snappyd->gUserDefine.refi_Fea.feature.remove(j);
                        int n_size = mesh->snappyd->gUserDefine.refi_Fea.n-1;
                        mesh->snappyd->gUserDefine.refi_Fea.feature.resize(n_size);
                        mesh->snappyd->gUserDefine.refi_Fea.n = n_size;

                    }
                }
                //remove row on table
                ui->txt_Log->appendPlainText("Successfully delete "+ name +" STL");
                Remove_All_Face();
                for(int j=0; j< mesh->snappyd->gBox.n; j++)
                {
                    AddFaceToList(mesh->snappyd->gBox.boxes[j].name);
                }
                for(int j=0; j< mesh->snappyd->gCylin.n; j++)
                {
                    AddFaceToList(mesh->snappyd->gCylin.cylins[j].name);
                }
                for(int j=0; j< mesh->snappyd->gSphere.n; j++)
                {
                    AddFaceToList(mesh->snappyd->gSphere.sphere[j].name);
                }
                for(int j=0; j< mesh->snappyd->gUserDefine.n; j++)
                {
                    AddFaceToList(mesh->snappyd->gUserDefine.user_Defines[j].name);
                }


                if(flag_btnSurGeometry_Click)
                {
                    ui->txt_GeometrySurfaceFileStl->setText("");
                    ui->txt_GeometrySurfaceName->setText("");
                    ui->txt_Min_Box_Sur_X->setText("");
                    ui->txt_Min_Box_Sur_Y->setText("");
                    ui->txt_Min_Box_Sur_Z->setText("");

                    ui->txt_Max_Box_Sur_X->setText("");
                    ui->txt_Max_Box_Sur_Y->setText("");
                    ui->txt_Max_Box_Sur_Z->setText("");
                }
                if(flag_btnSurface_Click)
                {
                    ui->txt_Level_Min_Surface_Refine->setText("");
                    ui->txt_Level_Max_Surface_Refine->setText("");
                }
                if(flag_btn_MeshLayer_Click)
                {
                    ui->txt_Layer_Bounding_2->setText("");
                }
                //delete STL & delete in facename,facetype
                int size_STL = mesh->snappyd->sTL.size();
                for(int j=0; j< size_STL; j++)
                {
                    if(mesh->snappyd->sTL[j].name == name)
                    {
                        int k = 0;
                        while(k < listSurfaces.size())
                        {
                            if(listSurfaces[k].contains(name + "_"))
                                listSurfaces.removeAt(k);
                            else
                                k++;
                        }
                        mesh->snappyd->sTL.remove(j);
                        mesh->snappyd->sTL.resize(size_STL-1);
                        mesh->updateGL();
                        return;
                    }
                }

            }

        }

        //Delete simple region - Box
        n = mesh->snappyd->gBoxRegion.n;
        for(int i=0; i < n; i++)
        {
            if(mesh->snappyd->gBoxRegion.boxes[i].name == name)
            {
                mesh->snappyd->gBoxRegion.boxes.remove(i);
                mesh->snappyd->gBoxRegion.n = mesh->snappyd->gBoxRegion.n -1;
                //find region
                for(int j=0; j < mesh->snappyd->gBoxRegion.refi_Reg.n; j++)
                {
                    if(mesh->snappyd->gBoxRegion.refi_Reg.region[j].name == name)
                    {
                        //remove surface
                        mesh->snappyd->gBoxRegion.refi_Reg.region.remove(j);
                        mesh->snappyd->gBoxRegion.refi_Reg.n = mesh->snappyd->gBoxRegion.refi_Reg.n-1;
                    }
                }
                Remove_All_Face();
                for(int j=0; j< mesh->snappyd->gBox.n; j++)
                {
                    AddFaceToList(mesh->snappyd->gBox.boxes[j].name);
                }
                ResetDrawBoxs();
                mesh->updateGL();
                return;
            }
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    //open folder
    if(path_Open != "")
    {
        QFile(path_Open + "/constant/polyMesh/blockMeshDict").remove();

        mesh->blockd->Write_Block_Dmesh(path_Open + "/constant/polyMesh");
        //save file setting bounding
        QStringList list;
        QString str_para;
        if(ui->cb_BoundingType->currentIndex() ==0)
        {
            str_para = "automatic";
            list.append(str_para);
        }
        else if(ui->cb_BoundingType->currentIndex() ==1)
        {
            QString x1 = ui->txt_DistanceMinX->text();
            QString y1 = ui->txt_DistanceMinY->text();
            QString z1 = ui->txt_DistanceMinZ->text();

            QString x2 = ui->txt_DistanceMaxX->text();
            QString y2 = ui->txt_DistanceMaxY->text();
            QString z2 = ui->txt_DistanceMaxZ->text();

            str_para = "distance " + x1 + " " + y1 + " " + z1 + " " + x2 + " " + y2 + " " + z2;
            list.append(str_para);
        }
        else if(ui->cb_BoundingType->currentIndex() ==2)
        {
            str_para = "define";
            list.append(str_para);
        }
        QFile(path_Open + "/constant/polyMesh/setting.dat").remove();
        mesh->blockd->Write_Bounding(path_Open + "/constant/polyMesh/setting.dat", list);

        //save snappy
        QFile(path_Open + "/system/snappyHexMeshDict").remove();
        mesh->snappyd->Write_Snappy(path_Open + "/system");
        if(!QDir(path_Open + "/constant/triSurface").exists())
            QDir().mkdir(path_Open + "/constant/triSurface");
        for(int i=0; i<mesh->snappyd->gUserDefine.n; i++)
        {
            QFile(mesh->snappyd->gUserDefine.user_Defines[i].direction).copy(path_Open + "/constant/triSurface/" + mesh->snappyd->gUserDefine.user_Defines[i].name_file);
        }
        if(mesh->patchDict->boundaries.size() > 0)
        {
            QFile(path_Open + "/system/createPatchDict").remove();
            mesh->patchDict->WritePatchDict(path_Open + "/system");
        }
        QMessageBox::information(
                this,
                tr("DMESH"), "Save file successfully!");
        return;
    }
    //save new
    QString saveCase = QFileDialog::getSaveFileName(this);
    if(saveCase != "")
    {
        if(!OpenFoam::CopyDir("Data/OpenFoamDefault",saveCase))
        {
            ui->txt_Log->appendPlainText("Can't copy default value to " + saveCase);
            return;
        }
        QFile(saveCase + "/constant/polyMesh/blockMeshDict").remove();
        OpenFoam::RemoveDir(saveCase + QDir::separator() + "0");
        QDir a;
        a.mkdir(saveCase + QDir::separator() + "0");

        mesh->blockd->Write_Block_Dmesh(saveCase + "/constant/polyMesh");
        //save file setting bounding
        QStringList list;
        QString str_para;
        if(ui->cb_BoundingType->currentIndex() ==0)
        {
            str_para = "automatic";
            list.append(str_para);
        }
        else if(ui->cb_BoundingType->currentIndex() ==1)
        {
            QString x1 = ui->txt_DistanceMinX->text();
            QString y1 = ui->txt_DistanceMinY->text();
            QString z1 = ui->txt_DistanceMinZ->text();

            QString x2 = ui->txt_DistanceMaxX->text();
            QString y2 = ui->txt_DistanceMaxY->text();
            QString z2 = ui->txt_DistanceMaxZ->text();

            str_para = "distance " + x1 + " " + y1 + " " + z1 + " " + x2 + " " + y2 + " " + z2;
            list.append(str_para);
        }
        else if(ui->cb_BoundingType->currentIndex() ==2)
        {
            str_para = "define";
            list.append(str_para);
        }
        mesh->blockd->Write_Bounding(saveCase + "/constant/polyMesh/setting.dat", list);

        //save snappy
        mesh->snappyd->Write_Snappy(saveCase + "/system");
        if(!QDir(saveCase + "/constant/triSurface").exists())
            QDir().mkdir(saveCase + "/constant/triSurface");
        for(int i=0; i<mesh->snappyd->gUserDefine.n; i++)
        {
            QFile(mesh->snappyd->gUserDefine.user_Defines[i].direction).copy(saveCase + "/constant/triSurface/" + mesh->snappyd->gUserDefine.user_Defines[i].name_file);
        }
        if(mesh->patchDict->boundaries.size() > 0)
        {
            mesh->patchDict->WritePatchDict(saveCase + "/system");
        }
    }
}

void MainWindow::on_btn_ViewMesh_clicked()
{
    QStringList views;
    for(int i = 0; i < ui->tb_boundary->rowCount(); i++)
    {
        if(ui->tb_boundary->item(i,0)->isSelected() == true)
            views.append(ui->tb_boundary->item(i,0)->text());
    }
    mesh->SetViewList(views);
}

void MainWindow::on_actionX_triggered()
{
    mesh->SetRotate(-90,0,90);
}

void MainWindow::on_actionY_triggered()
{
    mesh->SetRotate(-90,0,0);
}

void MainWindow::on_actionZ_triggered()
{
    mesh->SetRotate(0,180,0);
}

void MainWindow::on_actionX2_triggered()
{
    mesh->SetRotate(-90,0,-90);
}

void MainWindow::on_actionY2_triggered()
{
    mesh->SetRotate(-90,0,180);
}

void MainWindow::on_actionZ2_triggered()
{
    mesh->SetRotate(0,0,0);
}

void MainWindow::on_actionXYZ_triggered()
{
    mesh->SetRotate(30,-45,0);
}

void MainWindow::on_actionShow_Hide_Center_Coordinate_triggered()
{
    mesh->SetDrawCoordinate(ui->actionShow_Hide_Note_Coordinate->isChecked(),ui->actionShow_Hide_Center_Coordinate->isChecked());
}
void MainWindow::on_actionShow_Hide_Note_Coordinate_triggered()
{
    mesh->SetDrawCoordinate(ui->actionShow_Hide_Note_Coordinate->isChecked(),ui->actionShow_Hide_Center_Coordinate->isChecked());
}

void MainWindow::on_btn_MeshRefineAdvance_clicked()
{
    if(ui->tb_boundary->currentRow() < 0)
        return;
    QString currentSurface = ui->tb_boundary->currentItem()->text();
    RefinementSurfaceSTL *rSurface;
    RefinementFeaturesSTL *rFeature = &mesh->snappyd->gUserDefine.refi_Fea;
    for(int i = 0 ; i < mesh->snappyd->gUserDefine.refi_Sur.n; i++)
    {
        if(mesh->snappyd->gUserDefine.refi_Sur.surfaces[i].name == currentSurface)
            rSurface = &mesh->snappyd->gUserDefine.refi_Sur.surfaces[i];
    }

    W_MeshSTLRefineAdvance *w = new W_MeshSTLRefineAdvance(rSurface,rFeature,mesh->snappyd->resolveFeatureAngle);
    if(w->exec())
    {
        ui->txt_Log->appendPlainText("Defining of advance values for "+ ui->tb_boundary->currentItem()->text() +" surface has been done");
    }
}

void MainWindow::on_tb_MeshRefineAroundSurface_cellClicked(int row, int column)
{
    if(ui->tb_boundary->currentRow() < 0)
        return;
    QString currentSurface = ui->tb_boundary->currentItem()->text();
    if(column == 2)
    {
        if(row < ui->tb_MeshRefineAroundSurface->rowCount() - 1)
        {
            if(QMessageBox::warning(this,"Warning","Do you want to delete this field"))
            {
                float lv = ui->tb_MeshRefineAroundSurface->item(row,0)->text().toFloat();
                //Foreach Box
                for(int i = 0; i< mesh->snappyd->gBox.refi_Reg.n; i++)
                {
                    if(mesh->snappyd->gBox.refi_Reg.region[i].name == currentSurface)
                    {
                        for(int j = 0; j < mesh->snappyd->gBox.refi_Reg.region[i].distances.size(); j ++)
                        {
                            if(mesh->snappyd->gBox.refi_Reg.region[i].distances[j].lv1 == lv)
                            {
                                mesh->snappyd->gBox.refi_Reg.region[i].distances.remove(j);
                                break;
                            }
                        }
                        LoadRefineDistanceSurface(currentSurface,0);
                        return;
                    }
                }
                //Foreach Cilynder
                for(int i = 0; i< mesh->snappyd->gCylin.refi_Reg.n; i++)
                {
                    if(mesh->snappyd->gCylin.refi_Reg.region[i].name == currentSurface)
                    {
                        for(int j = 0; j < mesh->snappyd->gCylin.refi_Reg.region[i].distances.size(); j ++)
                        {
                            if(mesh->snappyd->gCylin.refi_Reg.region[i].distances[j].lv1 == lv)
                            {
                                mesh->snappyd->gCylin.refi_Reg.region[i].distances.remove(j);
                                break;
                            }
                        }
                        LoadRefineDistanceSurface(currentSurface,1);
                        return;
                    }
                }
                //Foreach Sphere
                for(int i = 0; i< mesh->snappyd->gSphere.refi_Reg.n; i++)
                {
                    if(mesh->snappyd->gSphere.refi_Reg.region[i].name == currentSurface)
                    {
                        for(int j = 0; j < mesh->snappyd->gSphere.refi_Reg.region[i].distances.size(); j ++)
                        {
                            if(mesh->snappyd->gSphere.refi_Reg.region[i].distances[j].lv1 == lv)
                            {
                                mesh->snappyd->gSphere.refi_Reg.region[i].distances.remove(j);
                                break;
                            }
                        }
                        LoadRefineDistanceSurface(currentSurface,3);
                        return;
                    }
                }
                //Foreach STL
                for(int i = 0; i< mesh->snappyd->gUserDefine.refi_Reg.n; i++)
                {
                    if(mesh->snappyd->gUserDefine.refi_Reg.region[i].name == currentSurface)
                    {
                        for(int j = 0; j < mesh->snappyd->gUserDefine.refi_Reg.region[i].distances.size(); j ++)
                        {
                            if(mesh->snappyd->gUserDefine.refi_Reg.region[i].distances[j].lv1 == lv)
                            {
                                mesh->snappyd->gUserDefine.refi_Reg.region[i].distances.remove(j);
                                break;
                            }
                        }
                        LoadRefineDistanceSurface(currentSurface,2);
                        return;
                    }
                }
            }
        }
        else
        {
            RefinementDistance r;
            QString c = ui->tb_MeshRefineAroundSurface->item(row,0)->text();
            QString d = ui->tb_MeshRefineAroundSurface->item(row,1)->text();
            bool a,b;
            r.lv1 = c.toFloat(&a);
            r.lv2 = d.toFloat(&b);
            if(!a || !b)
            {
                QMessageBox::warning(this,"Error","Please input all field...!");
                return;
            }
            ui->tb_MeshRefineAroundSurface->insertRow(ui->tb_MeshRefineAroundSurface->rowCount()-1);
            for(int i = 0; i< mesh->snappyd->gBox.refi_Reg.n; i++)
            {
                if(mesh->snappyd->gBox.refi_Reg.region[i].name == currentSurface)
                {
                    int j;
                    for(j = 0; j < mesh->snappyd->gBox.refi_Reg.region[i].distances.size(); j ++)
                    {
                        if(mesh->snappyd->gBox.refi_Reg.region[i].distances[j].lv1 > r.lv1)
                        {
                            mesh->snappyd->gBox.refi_Reg.region[i].distances.insert(j,r);
                            break;
                        }
                    }
                    if(j == mesh->snappyd->gBox.refi_Reg.region[i].distances.size())
                        mesh->snappyd->gBox.refi_Reg.region[i].distances.append(r);
                    LoadRefineDistanceSurface(currentSurface,0);
                    ui->txt_Log->appendPlainText("Settings of refining distance for " + currentSurface + " have been done");
                    return;
                }
            }
            for(int i = 0; i< mesh->snappyd->gCylin.refi_Reg.n; i++)
            {
                if(mesh->snappyd->gCylin.refi_Reg.region[i].name == currentSurface)
                {
                    int j;
                    for(j = 0; j < mesh->snappyd->gCylin.refi_Reg.region[i].distances.size(); j ++)
                    {
                        if(mesh->snappyd->gCylin.refi_Reg.region[i].distances[j].lv1 > r.lv1)
                        {
                            mesh->snappyd->gCylin.refi_Reg.region[i].distances.insert(j,r);
                            break;
                        }
                    }
                    if(j == mesh->snappyd->gCylin.refi_Reg.region[i].distances.size())
                        mesh->snappyd->gCylin.refi_Reg.region[i].distances.append(r);
                    LoadRefineDistanceSurface(currentSurface,1);
                    ui->txt_Log->appendPlainText("Settings of refining distance for " + currentSurface + " have been done");
                    return;
                }
            }
            for(int i = 0; i< mesh->snappyd->gSphere.refi_Reg.n; i++)
            {
                if(mesh->snappyd->gSphere.refi_Reg.region[i].name == currentSurface)
                {
                    int j;
                    for(j = 0; j < mesh->snappyd->gSphere.refi_Reg.region[i].distances.size(); j ++)
                    {
                        if(mesh->snappyd->gSphere.refi_Reg.region[i].distances[j].lv1 > r.lv1)
                        {
                            mesh->snappyd->gSphere.refi_Reg.region[i].distances.insert(j,r);
                            break;
                        }
                    }
                    if(j == mesh->snappyd->gSphere.refi_Reg.region[i].distances.size())
                        mesh->snappyd->gSphere.refi_Reg.region[i].distances.append(r);
                    LoadRefineDistanceSurface(currentSurface,3);
                    ui->txt_Log->appendPlainText("Settings of refining distance for " + currentSurface + " have been done");
                    return;
                }
            }
            for(int i = 0; i< mesh->snappyd->gUserDefine.refi_Reg.n; i++)
            {
                if(mesh->snappyd->gUserDefine.refi_Reg.region[i].name == currentSurface)
                {
                    int j;
                    for(j = 0; j < mesh->snappyd->gUserDefine.refi_Reg.region[i].distances.size(); j ++)
                    {
                        if(mesh->snappyd->gUserDefine.refi_Reg.region[i].distances[j].lv1 > r.lv1)
                        {
                            mesh->snappyd->gUserDefine.refi_Reg.region[i].distances.insert(j,r);
                            break;
                        }
                    }
                    if(j == mesh->snappyd->gUserDefine.refi_Reg.region[i].distances.size())
                        mesh->snappyd->gUserDefine.refi_Reg.region[i].distances.append(r);
                    LoadRefineDistanceSurface(currentSurface,2);
                    ui->txt_Log->appendPlainText("Settings of refining distance for " + currentSurface + " have been done");
                    return;
                }
            }
        }
    }
}

void MainWindow::on_txt_Level_Min_Surface_Refine_editingFinished()
{
    if(ui->txt_Level_Min_Surface_Refine->text()=="")
    {
        return;
    }
    bool a;
    int temp = ui->txt_Level_Min_Surface_Refine->text().toInt(&a);
    if(a ==false)
    {
        QMessageBox::warning(this,"Error","Please enter a number greater than or equal 0!");
        ui->txt_Level_Min_Surface_Refine->undo();
    }
    else
    {
        if(temp<0)
        {
            QMessageBox::warning(this,"Error","Please enter a number greater than or equal 0");
            ui->txt_Level_Min_Surface_Refine->undo();
        }
    }


}

void MainWindow::on_txt_Level_Max_Surface_Refine_editingFinished()
{
    if(ui->txt_Level_Max_Surface_Refine->text()=="")
    {
        return;
    }
    bool a;
    int temp = ui->txt_Level_Max_Surface_Refine->text().toInt(&a);
    if(a ==false)
    {
        QMessageBox::warning(this,"Error","Please enter a number greater than or equal 0!");
        ui->txt_Level_Max_Surface_Refine->undo();
    }
    else
    {
        if(temp<0)
        {
            QMessageBox::warning(this,"Error","Please enter a number greater than or equal 0");
            ui->txt_Level_Max_Surface_Refine->undo();
        }
    }

}

void MainWindow::on_txt_Level_Volume_editingFinished()
{
    if(ui->txt_Level_Volume->text()=="")
    {
        return;
    }
    bool a;
    int temp = ui->txt_Level_Volume->text().toInt(&a);
    if(a ==false)
    {
        QMessageBox::warning(this,"Error","Please enter a number greater than or equal 0!");
        ui->txt_Level_Volume->undo();
    }
    else
    {
        if(temp<0)
        {
            QMessageBox::warning(this,"Error","Please enter a number greater than or equal 0");
            ui->txt_Level_Volume->undo();
        }
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QString dir;
        dir= QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                               QDir::currentPath(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
        if(dir!="")
        {
            this->setWindowTitle(dir);

            bool temp1 = mesh->blockd->Read_Block_Dmesh(dir + "/constant/polyMesh/blockMeshDict");
            if(temp1== false)
            {
                QMessageBox::information(
                        this,
                        tr("DMESH"), "blockMeshDict file is not found.");
                return;
            }
            bool temp2 = mesh->snappyd->Read_Snappy(dir + "/system/snappyHexMeshDict");
            if(temp2 == false)
            {
                QMessageBox::information(
                        this,
                        tr("DMESH"), "snappyHexMeshDict file is not found.");
                return;
            }
            //read patch file
            mesh->patchDict->ReadPatchDict(dir + "/system/createPatchDict");

            //set path Open
            path_Open = dir;

            //DRAW BOUNDARY
            QString type_bou = mesh->blockd->Read_Setting(dir + "/constant/polyMesh/setting.dat");
            if(type_bou.trimmed().split(" ")[0] == "automatic")
            {
                ui->cb_BoundingType->setCurrentIndex(0);
                mesh->blockd->isAutomatic = true;
            }
            else if(type_bou.trimmed().split(" ")[0] == "distance")
            {
                ui->cb_BoundingType->setCurrentIndex(1);
                QStringList list_dis = type_bou.trimmed().split(" ",QString::SkipEmptyParts);
                ui->txt_DistanceMinX->setText(list_dis[1]);
                ui->txt_DistanceMinY->setText(list_dis[2]);
                ui->txt_DistanceMinZ->setText(list_dis[3]);

                ui->txt_DistanceMaxX->setText(list_dis[4]);
                ui->txt_DistanceMaxY->setText(list_dis[5]);
                ui->txt_DistanceMaxZ->setText(list_dis[6]);
                mesh->blockd->isAutomatic = false;
            }
            else if(type_bou.trimmed().split(" ")[0] == "define")
            {
                ui->cb_BoundingType->setCurrentIndex(2);
                ui->txt_Min_X_Bounding->setText(QString::number(mesh->blockd->vertice.v[0].x));
                ui->txt_Min_Y_Bounding->setText(QString::number(mesh->blockd->vertice.v[0].y));
                ui->txt_Min_Z_Bounding->setText(QString::number(mesh->blockd->vertice.v[0].z));

                ui->txt_Max_X_Bounding->setText(QString::number(mesh->blockd->vertice.v[6].x));
                ui->txt_Max_Y_Bounding->setText(QString::number(mesh->blockd->vertice.v[6].y));
                ui->txt_Max_Z_Bounding->setText(QString::number(mesh->blockd->vertice.v[6].z));
                mesh->blockd->isAutomatic = false;
            }

            //load location
            ui->txt_X_Location->setText(QString::number(mesh->snappyd->locationInMesh.x));
            ui->txt_Y_Location->setText(QString::number(mesh->snappyd->locationInMesh.y));
            ui->txt_Z_Location->setText(QString::number(mesh->snappyd->locationInMesh.z));
        }
        else
        {
            this->setWindowTitle("DMesh");
            return;
        }

        //bool temp;

        QStringList views = mesh->GetViewList();
        for(int i = 0; i < mesh->snappyd->gUserDefine.user_Defines.size(); i++)
        {
            QString path = dir + "/constant/triSurface/" + mesh->snappyd->gUserDefine.user_Defines[i].name_file;
            //temp = mesh->snappyd->ReadSTLFile(path);
            //Read file STL
            if(mesh->snappyd->ReadSTLFile(path))
            {
                QString _name = mesh->snappyd->gUserDefine.user_Defines[i].name;
                foreach(Solid solid, mesh->snappyd->sTL[mesh->snappyd->sTL.size() - 1].solids)
                {
                    bool flag_Path = false;
                    for(int j= 0; j< mesh->patchDict->boundaries.length(); j++)
                    {
                        for(int k = 0; k< mesh->patchDict->boundaries[j].patches.surfaces.length(); k++)
                        {
                            if(mesh->patchDict->boundaries[j].patches.surfaces[k] == _name + "_" + solid.name)
                            {
                                flag_Path = true;
                            }
                        }
                    }
                    if(flag_Path == false)
                    {
                        listSurfaces.append(_name + "_" + solid.name);
                    }

                }
            }
            mesh->snappyd->gUserDefine.user_Defines[i].direction = path;
            views.append(mesh->snappyd->gUserDefine.user_Defines[i].name);
        }
        for(int i = 0; i < mesh->snappyd->gBox.boxes.size(); i++)
        {
            //draw box
            int plus = mesh->snappyd->points.size();
            mesh->snappyd->points.resize(8+plus);
            mesh->snappyd->points[0+plus] = new  float[3];
            mesh->snappyd->points[0+plus][0] = mesh->snappyd->gBox.boxes[i].min.x;//x1
            mesh->snappyd->points[0+plus][1] = mesh->snappyd->gBox.boxes[i].min.y;//y1;
            mesh->snappyd->points[0+plus][2] = mesh->snappyd->gBox.boxes[i].min.z;//z1;


            mesh->snappyd->points[1+plus] = new  float[3];
            mesh->snappyd->points[1+plus][0] = mesh->snappyd->gBox.boxes[i].max.x;//x2;
            mesh->snappyd->points[1+plus][1] = mesh->snappyd->gBox.boxes[i].min.y;//y1;
            mesh->snappyd->points[1+plus][2] = mesh->snappyd->gBox.boxes[i].min.z;//z1;

            mesh->snappyd->points[2+plus] = new  float[3];
            mesh->snappyd->points[2+plus][0] = mesh->snappyd->gBox.boxes[i].max.x;//x2;
            mesh->snappyd->points[2+plus][1] = mesh->snappyd->gBox.boxes[i].max.y;//y2;
            mesh->snappyd->points[2+plus][2] = mesh->snappyd->gBox.boxes[i].min.z;//z1;

            mesh->snappyd->points[3+plus] = new  float[3];
            mesh->snappyd->points[3+plus][0] = mesh->snappyd->gBox.boxes[i].min.x;//x1;
            mesh->snappyd->points[3+plus][1] = mesh->snappyd->gBox.boxes[i].max.y;//y2;
            mesh->snappyd->points[3+plus][2] = mesh->snappyd->gBox.boxes[i].min.z;//z1;

            mesh->snappyd->points[4+plus] = new  float[3];
            mesh->snappyd->points[4+plus][0] = mesh->snappyd->gBox.boxes[i].min.x;//x1;
            mesh->snappyd->points[4+plus][1] = mesh->snappyd->gBox.boxes[i].min.y;//y1;
            mesh->snappyd->points[4+plus][2] = mesh->snappyd->gBox.boxes[i].max.z;//z2;

            mesh->snappyd->points[5+plus] = new  float[3];
            mesh->snappyd->points[5+plus][0] = mesh->snappyd->gBox.boxes[i].max.x;//x2;
            mesh->snappyd->points[5+plus][1] = mesh->snappyd->gBox.boxes[i].min.y;//y1;
            mesh->snappyd->points[5+plus][2] = mesh->snappyd->gBox.boxes[i].max.z;//z2;

            mesh->snappyd->points[6+plus] = new  float[3];
            mesh->snappyd->points[6+plus][0] = mesh->snappyd->gBox.boxes[i].max.x;//x2;
            mesh->snappyd->points[6+plus][1] = mesh->snappyd->gBox.boxes[i].max.y;//y2;
            mesh->snappyd->points[6+plus][2] = mesh->snappyd->gBox.boxes[i].max.z;//z2;

            mesh->snappyd->points[7+plus] = new  float[3];
            mesh->snappyd->points[7+plus][0] = mesh->snappyd->gBox.boxes[i].min.x;//x1;
            mesh->snappyd->points[7+plus][1] = mesh->snappyd->gBox.boxes[i].max.y;//y2;
            mesh->snappyd->points[7+plus][2] = mesh->snappyd->gBox.boxes[i].max.z;//z2;

            int starend[2];
            starend[0] = mesh->snappyd->faces.size();

            mesh->snappyd->faces.resize(6+starend[0]);
            mesh->snappyd->faces[0+starend[0]] = new int[4];
            mesh->snappyd->faces[0+starend[0]][0] = 3 + plus;
            mesh->snappyd->faces[0+starend[0]][1] = 7 + plus;
            mesh->snappyd->faces[0+starend[0]][2] = 6 + plus;
            mesh->snappyd->faces[0+starend[0]][3] = 2 + plus;

            mesh->snappyd->faces[1+starend[0]] = new int[4];
            mesh->snappyd->faces[1+starend[0]][0] = 1 + plus;
            mesh->snappyd->faces[1+starend[0]][1] = 5 + plus;
            mesh->snappyd->faces[1+starend[0]][2] = 4 + plus;
            mesh->snappyd->faces[1+starend[0]][3] = 0 + plus;

            mesh->snappyd->faces[2+starend[0]] = new int[4];
            mesh->snappyd->faces[2+starend[0]][0] = 4 + plus;
            mesh->snappyd->faces[2+starend[0]][1] = 5 + plus;
            mesh->snappyd->faces[2+starend[0]][2] = 6 + plus;
            mesh->snappyd->faces[2+starend[0]][3] = 7 + plus;

            mesh->snappyd->faces[3+starend[0]] = new int[4];
            mesh->snappyd->faces[3+starend[0]][0] = 0 + plus;
            mesh->snappyd->faces[3+starend[0]][1] = 3 + plus;
            mesh->snappyd->faces[3+starend[0]][2] = 2 + plus;
            mesh->snappyd->faces[3+starend[0]][3] = 1 + plus;

            mesh->snappyd->faces[4+starend[0]] = new int[4];
            mesh->snappyd->faces[4+starend[0]][0] = 0 + plus;
            mesh->snappyd->faces[4+starend[0]][1] = 4 + plus;
            mesh->snappyd->faces[4+starend[0]][2] = 7 + plus;
            mesh->snappyd->faces[4+starend[0]][3] = 3 + plus;

            mesh->snappyd->faces[5+starend[0]] = new int[4];
            mesh->snappyd->faces[5+starend[0]][0] = 2 + plus;
            mesh->snappyd->faces[5+starend[0]][1] = 6 + plus;
            mesh->snappyd->faces[5+starend[0]][2] = 5 + plus;
            mesh->snappyd->faces[5+starend[0]][3] = 1 + plus;

            starend[1] = mesh->snappyd->faces.size();

            mesh->snappyd->facezones.resize( mesh->snappyd->facezones.size()+1);
            mesh->snappyd->facezones[mesh->snappyd->facezones.size()-1]= new int[2];
            mesh->snappyd->facezones[mesh->snappyd->facezones.size()-1][0]= starend[0];
            mesh->snappyd->facezones[mesh->snappyd->facezones.size()-1][1]= starend[1];

            mesh->snappyd->facetype.resize(mesh->snappyd->facetype.size() + 1);
            mesh->snappyd->facetype[mesh->snappyd->facetype.size() - 1] = 1;

            mesh->snappyd->facename.append(mesh->snappyd->gBox.boxes[i].name);

            views.append(mesh->snappyd->gBox.boxes[i].name);
            bool flag_Path = false;
            for(int j= 0; j< mesh->patchDict->boundaries.length(); j++)
            {
                for(int k = 0; k< mesh->patchDict->boundaries[j].patches.surfaces.length(); k++)
                {
                    if(mesh->patchDict->boundaries[j].patches.surfaces[k] == mesh->snappyd->gBox.boxes[i].name)
                    {
                        flag_Path = true;
                    }
                }
            }
            if(flag_Path == false)
            {
                listSurfaces.append(mesh->snappyd->gBox.boxes[i].name);
            }

            //find min max
            mesh->snappyd->min_Max.min.x = mesh->snappyd->gBox.boxes[i].min.x;
            mesh->snappyd->min_Max.min.y = mesh->snappyd->gBox.boxes[i].min.y;
            mesh->snappyd->min_Max.min.z = mesh->snappyd->gBox.boxes[i].min.z;

            mesh->snappyd->min_Max.max.x = mesh->snappyd->gBox.boxes[i].max.x;
            mesh->snappyd->min_Max.max.y = mesh->snappyd->gBox.boxes[i].max.y;
            mesh->snappyd->min_Max.max.z = mesh->snappyd->gBox.boxes[i].max.z;

            //add min_Max temp to list min max;
            mesh->snappyd->min_Max.name_Surface = mesh->snappyd->gBox.boxes[i].name;
            mesh->snappyd->list_Surface_Min_Max.append(mesh->snappyd->min_Max);
            mesh->snappyd->min_Max.name_Surface="";
            mesh->snappyd->min_Max.min.x=1000000;
            mesh->snappyd->min_Max.min.y=1000000;
            mesh->snappyd->min_Max.min.z=1000000;
            mesh->snappyd->min_Max.max.x=-1000000;
            mesh->snappyd->min_Max.max.y=-1000000;
            mesh->snappyd->min_Max.max.z=-1000000;
            mesh->snappyd->FindMinMax(mesh->snappyd->list_Surface_Min_Max);
        }

        //Cylinder
        for(int i = 0; i < mesh->snappyd->gCylin.cylins.size(); i++)
        {
            views.append(mesh->snappyd->gCylin.cylins[i].name);
            bool flag_Path = false;
            for(int j= 0; j< mesh->patchDict->boundaries.length(); j++)
            {
                for(int k = 0; k< mesh->patchDict->boundaries[j].patches.surfaces.length(); k++)
                {
                    if(mesh->patchDict->boundaries[j].patches.surfaces[k] == mesh->snappyd->gCylin.cylins[i].name)
                    {
                        flag_Path = true;
                    }
                }
            }
            if(flag_Path == false)
            {
                listSurfaces.append(mesh->snappyd->gCylin.cylins[i].name);
            }

            //find minmax
            float x1 = mesh->snappyd->gCylin.cylins[i].point1.x;
            float x2 = mesh->snappyd->gCylin.cylins[i].point2.x;
            float y1 = mesh->snappyd->gCylin.cylins[i].point1.y;
            float y2 = mesh->snappyd->gCylin.cylins[i].point2.y;
            float z1 = mesh->snappyd->gCylin.cylins[i].point1.z;
            float z2 = mesh->snappyd->gCylin.cylins[i].point2.z;

            float radius_cyl = mesh->snappyd->gCylin.cylins[i].radius;
            //find min max default bouding
            float P1,P2,P3,P4;

            P1 = x1 + radius_cyl;//*costemp;
            P2 = x1 - radius_cyl;//*costemp;
            P3 = x2 + radius_cyl;//*costemp;
            P4 = x2 - radius_cyl;//*costemp;

            float minx= P1;
            float maxx = P1;
            if(minx>P2)
            {
                minx = P2;
            }
            if(minx>P3)
            {
                minx = P3;
            }
            if(minx>P4)
            {
                minx = P4;
            }
            if(maxx<P2)
            {
                maxx = P2;
            }
            if(maxx<P3)
            {
                maxx = P3;
            }
            if(maxx<P4)
            {
                maxx = P4;
            }

            P1 = y1 + radius_cyl;//*costemp;
            P2 = y1 - radius_cyl;//*costemp;
            P3 = y2 + radius_cyl;//*costemp;
            P4 = y2 - radius_cyl;//*costemp;

            float miny= P1;
            float maxy = P1;
            if(miny>P2)
            {
                miny = P2;
            }
            if(miny>P3)
            {
                miny = P3;
            }
            if(miny>P4)
            {
                miny = P4;
            }

            if(maxy<P2)
            {
                maxy = P2;
            }
            if(maxy<P3)
            {
                maxy = P3;
            }
            if(maxy<P4)
            {
                maxy = P4;
            }

            P1 = z1 + radius_cyl;//*costemp;
            P2 = z1 - radius_cyl;//*costemp;
            P3 = z2 + radius_cyl;//*costemp;
            P4 = z2 - radius_cyl;//*costemp;

            float minz= P1;
            float maxz = P1;
            if(minz>P2)
            {
                minz = P2;
            }
            if(minz>P3)
            {
                minz = P3;
            }
            if(minz>P4)
            {
                minz = P4;
            }

            if(maxz<P2)
            {
                maxz = P2;
            }
            if(maxz<P3)
            {
                maxz = P3;
            }
            if(maxz<P4)
            {
                maxz = P4;
            }

            mesh->snappyd->min_Max.min.x = minx;
            mesh->snappyd->min_Max.min.y = miny;
            mesh->snappyd->min_Max.min.z = minz;

            mesh->snappyd->min_Max.max.x = maxx;
            mesh->snappyd->min_Max.max.y = maxy;
            mesh->snappyd->min_Max.max.z = maxz;

            //add min_Max temp to list min max;
            mesh->snappyd->min_Max.name_Surface = mesh->snappyd->gCylin.cylins[i].name;
            mesh->snappyd->list_Surface_Min_Max.append(mesh->snappyd->min_Max);
            mesh->snappyd->min_Max.name_Surface="";
            mesh->snappyd->FindMinMax(mesh->snappyd->list_Surface_Min_Max);
        }
        //Sphere
        for(int i = 0; i < mesh->snappyd->gSphere.sphere.size(); i++)
        {
            views.append(mesh->snappyd->gSphere.sphere[i].name);
            bool flag_Path = false;
            for(int j= 0; j< mesh->patchDict->boundaries.length(); j++)
            {
                for(int k = 0; k< mesh->patchDict->boundaries[j].patches.surfaces.length(); k++)
                {
                    if(mesh->patchDict->boundaries[j].patches.surfaces[k] == mesh->snappyd->gSphere.sphere[i].name)
                    {
                        flag_Path = true;
                    }
                }
            }
            if(flag_Path == false)
            {
                listSurfaces.append(mesh->snappyd->gSphere.sphere[i].name);
            }

            //finf min max
            mesh->snappyd->min_Max.min.x = mesh->snappyd->gSphere.sphere[i].centre.x + mesh->snappyd->gSphere.sphere[i].radius;
            mesh->snappyd->min_Max.min.y = mesh->snappyd->gSphere.sphere[i].centre.y + mesh->snappyd->gSphere.sphere[i].radius;
            mesh->snappyd->min_Max.min.z = mesh->snappyd->gSphere.sphere[i].centre.z + mesh->snappyd->gSphere.sphere[i].radius;

            mesh->snappyd->min_Max.max.x = mesh->snappyd->gSphere.sphere[i].centre.x - mesh->snappyd->gSphere.sphere[i].radius;
            mesh->snappyd->min_Max.max.y = mesh->snappyd->gSphere.sphere[i].centre.y - mesh->snappyd->gSphere.sphere[i].radius;
            mesh->snappyd->min_Max.max.z = mesh->snappyd->gSphere.sphere[i].centre.z - mesh->snappyd->gSphere.sphere[i].radius;

            //add min_Max temp to list min max;
            mesh->snappyd->min_Max.name_Surface = mesh->snappyd->gSphere.sphere[i].name;
            mesh->snappyd->list_Surface_Min_Max.append(mesh->snappyd->min_Max);
            mesh->snappyd->min_Max.name_Surface = "";
            mesh->snappyd->FindMinMax(mesh->snappyd->list_Surface_Min_Max);
        }

        //regions ...................

        for(int i = 0; i < mesh->snappyd->gBoxRegion.boxes.size(); i++)
        {
            //draw box
            int plus = mesh->snappyd->points.size();
            mesh->snappyd->points.resize(8+plus);
            mesh->snappyd->points[0+plus] = new  float[3];
            mesh->snappyd->points[0+plus][0] = mesh->snappyd->gBoxRegion.boxes[i].min.x;//x1
            mesh->snappyd->points[0+plus][1] = mesh->snappyd->gBoxRegion.boxes[i].min.y;//y1;
            mesh->snappyd->points[0+plus][2] = mesh->snappyd->gBoxRegion.boxes[i].min.z;//z1;


            mesh->snappyd->points[1+plus] = new  float[3];
            mesh->snappyd->points[1+plus][0] = mesh->snappyd->gBoxRegion.boxes[i].max.x;//x2;
            mesh->snappyd->points[1+plus][1] = mesh->snappyd->gBoxRegion.boxes[i].min.y;//y1;
            mesh->snappyd->points[1+plus][2] = mesh->snappyd->gBoxRegion.boxes[i].min.z;//z1;

            mesh->snappyd->points[2+plus] = new  float[3];
            mesh->snappyd->points[2+plus][0] = mesh->snappyd->gBoxRegion.boxes[i].max.x;//x2;
            mesh->snappyd->points[2+plus][1] = mesh->snappyd->gBoxRegion.boxes[i].max.y;//y2;
            mesh->snappyd->points[2+plus][2] = mesh->snappyd->gBoxRegion.boxes[i].min.z;//z1;

            mesh->snappyd->points[3+plus] = new  float[3];
            mesh->snappyd->points[3+plus][0] = mesh->snappyd->gBoxRegion.boxes[i].min.x;//x1;
            mesh->snappyd->points[3+plus][1] = mesh->snappyd->gBoxRegion.boxes[i].max.y;//y2;
            mesh->snappyd->points[3+plus][2] = mesh->snappyd->gBoxRegion.boxes[i].min.z;//z1;

            mesh->snappyd->points[4+plus] = new  float[3];
            mesh->snappyd->points[4+plus][0] = mesh->snappyd->gBoxRegion.boxes[i].min.x;//x1;
            mesh->snappyd->points[4+plus][1] = mesh->snappyd->gBoxRegion.boxes[i].min.y;//y1;
            mesh->snappyd->points[4+plus][2] = mesh->snappyd->gBoxRegion.boxes[i].max.z;//z2;

            mesh->snappyd->points[5+plus] = new  float[3];
            mesh->snappyd->points[5+plus][0] = mesh->snappyd->gBoxRegion.boxes[i].max.x;//x2;
            mesh->snappyd->points[5+plus][1] = mesh->snappyd->gBoxRegion.boxes[i].min.y;//y1;
            mesh->snappyd->points[5+plus][2] = mesh->snappyd->gBoxRegion.boxes[i].max.z;//z2;

            mesh->snappyd->points[6+plus] = new  float[3];
            mesh->snappyd->points[6+plus][0] = mesh->snappyd->gBoxRegion.boxes[i].max.x;//x2;
            mesh->snappyd->points[6+plus][1] = mesh->snappyd->gBoxRegion.boxes[i].max.y;//y2;
            mesh->snappyd->points[6+plus][2] = mesh->snappyd->gBoxRegion.boxes[i].max.z;//z2;

            mesh->snappyd->points[7+plus] = new  float[3];
            mesh->snappyd->points[7+plus][0] = mesh->snappyd->gBoxRegion.boxes[i].min.x;//x1;
            mesh->snappyd->points[7+plus][1] = mesh->snappyd->gBoxRegion.boxes[i].max.y;//y2;
            mesh->snappyd->points[7+plus][2] = mesh->snappyd->gBoxRegion.boxes[i].max.z;//z2;

            int starend[2];
            starend[0] = mesh->snappyd->faces.size();

            mesh->snappyd->faces.resize(6+starend[0]);
            mesh->snappyd->faces[0+starend[0]] = new int[4];
            mesh->snappyd->faces[0+starend[0]][0] = 3 + plus;
            mesh->snappyd->faces[0+starend[0]][1] = 7 + plus;
            mesh->snappyd->faces[0+starend[0]][2] = 6 + plus;
            mesh->snappyd->faces[0+starend[0]][3] = 2 + plus;

            mesh->snappyd->faces[1+starend[0]] = new int[4];
            mesh->snappyd->faces[1+starend[0]][0] = 1 + plus;
            mesh->snappyd->faces[1+starend[0]][1] = 5 + plus;
            mesh->snappyd->faces[1+starend[0]][2] = 4 + plus;
            mesh->snappyd->faces[1+starend[0]][3] = 0 + plus;

            mesh->snappyd->faces[2+starend[0]] = new int[4];
            mesh->snappyd->faces[2+starend[0]][0] = 4 + plus;
            mesh->snappyd->faces[2+starend[0]][1] = 5 + plus;
            mesh->snappyd->faces[2+starend[0]][2] = 6 + plus;
            mesh->snappyd->faces[2+starend[0]][3] = 7 + plus;

            mesh->snappyd->faces[3+starend[0]] = new int[4];
            mesh->snappyd->faces[3+starend[0]][0] = 0 + plus;
            mesh->snappyd->faces[3+starend[0]][1] = 3 + plus;
            mesh->snappyd->faces[3+starend[0]][2] = 2 + plus;
            mesh->snappyd->faces[3+starend[0]][3] = 1 + plus;

            mesh->snappyd->faces[4+starend[0]] = new int[4];
            mesh->snappyd->faces[4+starend[0]][0] = 0 + plus;
            mesh->snappyd->faces[4+starend[0]][1] = 4 + plus;
            mesh->snappyd->faces[4+starend[0]][2] = 7 + plus;
            mesh->snappyd->faces[4+starend[0]][3] = 3 + plus;

            mesh->snappyd->faces[5+starend[0]] = new int[4];
            mesh->snappyd->faces[5+starend[0]][0] = 2 + plus;
            mesh->snappyd->faces[5+starend[0]][1] = 6 + plus;
            mesh->snappyd->faces[5+starend[0]][2] = 5 + plus;
            mesh->snappyd->faces[5+starend[0]][3] = 1 + plus;

            starend[1] = mesh->snappyd->faces.size();

            mesh->snappyd->facezones.resize( mesh->snappyd->facezones.size()+1);
            mesh->snappyd->facezones[mesh->snappyd->facezones.size()-1]= new int[2];
            mesh->snappyd->facezones[mesh->snappyd->facezones.size()-1][0]= starend[0];
            mesh->snappyd->facezones[mesh->snappyd->facezones.size()-1][1]= starend[1];

            mesh->snappyd->facetype.resize(mesh->snappyd->facetype.size() + 1);
            mesh->snappyd->facetype[mesh->snappyd->facetype.size() - 1] = 1;

            mesh->snappyd->facename.append(mesh->snappyd->gBoxRegion.boxes[i].name);

            views.append(mesh->snappyd->gBoxRegion.boxes[i].name);
            //find min max
            mesh->snappyd->min_Max.min.x = mesh->snappyd->gBoxRegion.boxes[i].min.x;
            mesh->snappyd->min_Max.min.y = mesh->snappyd->gBoxRegion.boxes[i].min.y;
            mesh->snappyd->min_Max.min.z = mesh->snappyd->gBoxRegion.boxes[i].min.z;

            mesh->snappyd->min_Max.max.x = mesh->snappyd->gBoxRegion.boxes[i].max.x;
            mesh->snappyd->min_Max.max.y = mesh->snappyd->gBoxRegion.boxes[i].max.y;
            mesh->snappyd->min_Max.max.z = mesh->snappyd->gBoxRegion.boxes[i].max.z;

            //add min_Max temp to list min max;
            mesh->snappyd->min_Max.name_Surface = mesh->snappyd->gBoxRegion.boxes[i].name;
            mesh->snappyd->list_Surface_Min_Max.append(mesh->snappyd->min_Max);
            mesh->snappyd->min_Max.name_Surface="";
            mesh->snappyd->min_Max.min.x=1000000;
            mesh->snappyd->min_Max.min.y=1000000;
            mesh->snappyd->min_Max.min.z=1000000;
            mesh->snappyd->min_Max.max.x=-1000000;
            mesh->snappyd->min_Max.max.y=-1000000;
            mesh->snappyd->min_Max.max.z=-1000000;
            mesh->snappyd->FindMinMax(mesh->snappyd->list_Surface_Min_Max);
        }

        //Cylinder
        for(int i = 0; i < mesh->snappyd->gCylinRegion.cylins.size(); i++)
        {
            views.append(mesh->snappyd->gCylinRegion.cylins[i].name);

            //find minmax
            float x1 = mesh->snappyd->gCylinRegion.cylins[i].point1.x;
            float x2 = mesh->snappyd->gCylinRegion.cylins[i].point2.x;
            float y1 = mesh->snappyd->gCylinRegion.cylins[i].point1.y;
            float y2 = mesh->snappyd->gCylinRegion.cylins[i].point2.y;
            float z1 = mesh->snappyd->gCylinRegion.cylins[i].point1.z;
            float z2 = mesh->snappyd->gCylinRegion.cylins[i].point2.z;

            float radius_cyl = mesh->snappyd->gCylinRegion.cylins[i].radius;
            //find min max default bouding
            float P1,P2,P3,P4;

            P1 = x1 + radius_cyl;//*costemp;
            P2 = x1 - radius_cyl;//*costemp;
            P3 = x2 + radius_cyl;//*costemp;
            P4 = x2 - radius_cyl;//*costemp;

            float minx= P1;
            float maxx = P1;
            if(minx>P2)
            {
                minx = P2;
            }
            if(minx>P3)
            {
                minx = P3;
            }
            if(minx>P4)
            {
                minx = P4;
            }
            if(maxx<P2)
            {
                maxx = P2;
            }
            if(maxx<P3)
            {
                maxx = P3;
            }
            if(maxx<P4)
            {
                maxx = P4;
            }

            P1 = y1 + radius_cyl;//*costemp;
            P2 = y1 - radius_cyl;//*costemp;
            P3 = y2 + radius_cyl;//*costemp;
            P4 = y2 - radius_cyl;//*costemp;

            float miny= P1;
            float maxy = P1;
            if(miny>P2)
            {
                miny = P2;
            }
            if(miny>P3)
            {
                miny = P3;
            }
            if(miny>P4)
            {
                miny = P4;
            }

            if(maxy<P2)
            {
                maxy = P2;
            }
            if(maxy<P3)
            {
                maxy = P3;
            }
            if(maxy<P4)
            {
                maxy = P4;
            }

            P1 = z1 + radius_cyl;//*costemp;
            P2 = z1 - radius_cyl;//*costemp;
            P3 = z2 + radius_cyl;//*costemp;
            P4 = z2 - radius_cyl;//*costemp;

            float minz= P1;
            float maxz = P1;
            if(minz>P2)
            {
                minz = P2;
            }
            if(minz>P3)
            {
                minz = P3;
            }
            if(minz>P4)
            {
                minz = P4;
            }

            if(maxz<P2)
            {
                maxz = P2;
            }
            if(maxz<P3)
            {
                maxz = P3;
            }
            if(maxz<P4)
            {
                maxz = P4;
            }

            mesh->snappyd->min_Max.min.x = minx;
            mesh->snappyd->min_Max.min.y = miny;
            mesh->snappyd->min_Max.min.z = minz;

            mesh->snappyd->min_Max.max.x = maxx;
            mesh->snappyd->min_Max.max.y = maxy;
            mesh->snappyd->min_Max.max.z = maxz;

            //add min_Max temp to list min max;
            mesh->snappyd->min_Max.name_Surface = mesh->snappyd->gCylinRegion.cylins[i].name;
            mesh->snappyd->list_Surface_Min_Max.append(mesh->snappyd->min_Max);
            mesh->snappyd->min_Max.name_Surface="";
            mesh->snappyd->FindMinMax(mesh->snappyd->list_Surface_Min_Max);
        }
        //Sphere
        for(int i = 0; i < mesh->snappyd->gSphereRegion.sphere.size(); i++)
        {
            views.append(mesh->snappyd->gSphereRegion.sphere[i].name);

            //finf min max
            mesh->snappyd->min_Max.min.x = mesh->snappyd->gSphereRegion.sphere[i].centre.x + mesh->snappyd->gSphereRegion.sphere[i].radius;
            mesh->snappyd->min_Max.min.y = mesh->snappyd->gSphereRegion.sphere[i].centre.y + mesh->snappyd->gSphereRegion.sphere[i].radius;
            mesh->snappyd->min_Max.min.z = mesh->snappyd->gSphereRegion.sphere[i].centre.z + mesh->snappyd->gSphereRegion.sphere[i].radius;

            mesh->snappyd->min_Max.max.x = mesh->snappyd->gSphereRegion.sphere[i].centre.x - mesh->snappyd->gSphereRegion.sphere[i].radius;
            mesh->snappyd->min_Max.max.y = mesh->snappyd->gSphereRegion.sphere[i].centre.y - mesh->snappyd->gSphereRegion.sphere[i].radius;
            mesh->snappyd->min_Max.max.z = mesh->snappyd->gSphereRegion.sphere[i].centre.z - mesh->snappyd->gSphereRegion.sphere[i].radius;

            //add min_Max temp to list min max;
            mesh->snappyd->min_Max.name_Surface = mesh->snappyd->gSphereRegion.sphere[i].name;
            mesh->snappyd->list_Surface_Min_Max.append(mesh->snappyd->min_Max);
            mesh->snappyd->min_Max.name_Surface = "";
            mesh->snappyd->FindMinMax(mesh->snappyd->list_Surface_Min_Max);
        }

        mesh->SetViewList(views);
        mesh->updateGL();
}

void MainWindow::SetButtonDisplayEnable(bool value)
{
    ui->actionX->setEnabled(value);
    ui->actionX2->setEnabled(value);
    ui->actionY->setEnabled(value);
    ui->actionY2->setEnabled(value);
    ui->actionZ->setEnabled(value);
    ui->actionZ2->setEnabled(value);
    ui->actionXYZ->setEnabled(value);
    ui->actionShow_Hide_Center_Coordinate->setEnabled(value);
    ui->actionShow_Hide_Note_Coordinate->setEnabled(value);
    ui->actionCapture->setEnabled(value);
}

void MainWindow::SetButtonDefault()
{
    SetButtonDisplayEnable(false);
    ui->actionOpen->setEnabled(false);
}

void MainWindow::SetButtonEnable(bool value)
{
    ui->actionOpen->setEnabled(value);
    ui->actionSave->setEnabled(value);

    ui->btn_Boundary->setEnabled(value);
    ui->btn_Mesh->setEnabled(value);
    ui->btn_Generate->setEnabled(value);
    ui->btn_Geometry->setEnabled(value);
}

void MainWindow::on_actionQuit_triggered()
{
    this->close();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat() ) {
        if(event->key() == Qt::Key_Control){
            mesh->isCtrlPress=false;
            event->ignore();
        }

    } else {
        if(event->key() == Qt::Key_Control){
            mesh->isCtrlPress=true;
            event->accept();
        }

    }
//    if(event->key() == Qt::Key_Control){
//        mesh->isCtrlPress=true;
//    }
//    else
//        mesh->isCtrlPress=false;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat() ) {
        if(event->key() == Qt::Key_Control){
            mesh->isCtrlPress=true;
            event->ignore();
        }

    } else {
        if(event->key() == Qt::Key_Control){
            mesh->isCtrlPress=false;
            event->accept();
        }
    }
}



void MainWindow::on_tb_boundary_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
{
    flag_Item_Face_Click = true;
    QString currentSurface = ui->tb_boundary->currentItem()->text();
    if(flag_btnSurface_Click == true)
    {
        for(int i = 0; i< mesh->snappyd->gBox.refi_Sur.n; i++)
        {
            if(mesh->snappyd->gBox.refi_Sur.surfaces[i].name == currentSurface)
            {
                ui->txt_Level_Min_Surface_Refine->setText(QString::number
                                                          (mesh->snappyd->gBox.refi_Sur.surfaces[i].lv1));
                ui->txt_Level_Max_Surface_Refine->setText(QString::number
                                                 (mesh->snappyd->gBox.refi_Sur.surfaces[i].lv2));
                LoadRefineDistanceSurface(currentSurface,0);
                ui->btn_MeshRefineAdvance->hide();
                ui->groupBox_MeshSurfaceAround->show();
                return;
            }
        }
        //snappy cylinder
        for(int i = 0; i< mesh->snappyd->gCylin.refi_Sur.n; i++)
        {
            if(mesh->snappyd->gCylin.refi_Sur.surfaces[i].name == currentSurface)
            {
                ui->txt_Level_Min_Surface_Refine->setText(QString::number
                                                          (mesh->snappyd->gCylin.refi_Sur.surfaces[i].lv1));
                ui->txt_Level_Max_Surface_Refine->setText(QString::number
                                                 (mesh->snappyd->gCylin.refi_Sur.surfaces[i].lv2));
                LoadRefineDistanceSurface(currentSurface,1);
                ui->btn_MeshRefineAdvance->hide();
                ui->groupBox_MeshSurfaceAround->show();
                return;
            }
        }
        //snappy cylinder
        for(int i = 0; i< mesh->snappyd->gSphere.refi_Sur.n; i++)
        {
            if(mesh->snappyd->gSphere.refi_Sur.surfaces[i].name == currentSurface)
            {
                ui->txt_Level_Min_Surface_Refine->setText(QString::number
                                                          (mesh->snappyd->gSphere.refi_Sur.surfaces[i].lv1));
                ui->txt_Level_Max_Surface_Refine->setText(QString::number
                                                 (mesh->snappyd->gSphere.refi_Sur.surfaces[i].lv2));
                LoadRefineDistanceSurface(currentSurface,3);
                ui->btn_MeshRefineAdvance->hide();
                ui->groupBox_MeshSurfaceAround->show();
                return;
            }
        }
        //snappy userdefine
        GeomeUserDefine *gUserDefine = &mesh->snappyd->gUserDefine;
        for(int i = 0; i< gUserDefine->refi_Sur.n; i++)
        {
            if(gUserDefine->refi_Sur.surfaces[i].name == currentSurface)
            {
                ui->txt_Level_Min_Surface_Refine->setText(QString::number
                                                          (gUserDefine->refi_Sur.surfaces[i].lv1));
                ui->txt_Level_Max_Surface_Refine->setText(QString::number
                                                 (gUserDefine->refi_Sur.surfaces[i].lv2));
                //Region
                LoadRefineDistanceSurface(currentSurface,2);

                ui->btn_MeshRefineAdvance->show();
                ui->groupBox_MeshSurfaceAround->show();
                return;
            }
        }
    }
    if(flag_btnVolume_Click == true)
    {
        //ui->txt_Level_Volume->setText("0");
        //ui->cb_MeshVolumeMode->setCurrentIndex(0);
        ui->txt_GeometryVolumeName->setText(currentSurface);
        RefinementRegions *refi_Reg = &mesh->snappyd->gBoxRegion.refi_Reg;
        for(int i = 0; i < refi_Reg->region.size(); i++)
        {
            if(refi_Reg->region[i].name == currentSurface)
            {
                if(refi_Reg->region[i].mode =="inside" )
                    ui->cb_MeshVolumeMode->setCurrentIndex(1);
                else if(refi_Reg->region[i].mode =="outside" )
                    ui->cb_MeshVolumeMode->setCurrentIndex(2);
                ui->txt_Level_Volume->setText(QString::number(refi_Reg->region[i].lv2));
            }
        }
        GeomeBoxRegion *gBoxRegion = &mesh->snappyd->gBoxRegion;
        for(int i = 0; i < gBoxRegion->n; i++)
        {
            if(gBoxRegion->boxes[i].name == currentSurface)
            {
                ui->cb_VolumeType->setCurrentIndex(0);
                ui->txt_Min_Box_Vol_X->setText(QString::number(gBoxRegion->boxes[i].min.x));
                ui->txt_Min_Box_Vol_Y->setText(QString::number(gBoxRegion->boxes[i].min.y));
                ui->txt_Min_Box_Vol_Z->setText(QString::number(gBoxRegion->boxes[i].min.z));
                ui->txt_Max_Box_Vol_X->setText(QString::number(gBoxRegion->boxes[i].max.x));
                ui->txt_Max_Box_Vol_Y->setText(QString::number(gBoxRegion->boxes[i].max.y));
                ui->txt_Max_Box_Vol_Z->setText(QString::number(gBoxRegion->boxes[i].max.z));
                return;
            }
        }
        refi_Reg = &mesh->snappyd->gCylinRegion.refi_Reg;
        for(int i = 0; i < refi_Reg->region.size(); i++)
        {
            if(refi_Reg->region[i].name == currentSurface)
            {
                if(refi_Reg->region[i].mode =="inside" )
                    ui->cb_MeshVolumeMode->setCurrentIndex(1);
                else if(refi_Reg->region[i].mode =="outside" )
                    ui->cb_MeshVolumeMode->setCurrentIndex(2);
                ui->txt_Level_Volume->setText(QString::number(refi_Reg->region[i].lv2));
            }
        }
        GeomeCylinderRegion *gCylinRegion = &mesh->snappyd->gCylinRegion;
        for(int i = 0; i < gCylinRegion->n; i++)
        {
            if(gCylinRegion->cylins[i].name == currentSurface)
            {
                ui->cb_VolumeType->setCurrentIndex(1);
                ui->txt_P1_Cyl_Vol_X->setText(QString::number(gCylinRegion->cylins[i].point1.x));
                ui->txt_P1_Cyl_Vol_Y->setText(QString::number(gCylinRegion->cylins[i].point1.y));
                ui->txt_P1_Cyl_Vol_Z->setText(QString::number(gCylinRegion->cylins[i].point1.z));
                ui->txt_P2_Cyl_Vol_X->setText(QString::number(gCylinRegion->cylins[i].point2.x));
                ui->txt_P2_Cyl_Vol_Y->setText(QString::number(gCylinRegion->cylins[i].point2.y));
                ui->txt_P2_Cyl_Vol_Z->setText(QString::number(gCylinRegion->cylins[i].point2.z));
                ui->txt_Radius_Cyl_Vol->setText(QString::number(gCylinRegion->cylins[i].radius));
                return;
            }
        }
        refi_Reg = &mesh->snappyd->gSphereRegion.refi_Reg;
        for(int i = 0; i < refi_Reg->region.size(); i++)
        {
            if(refi_Reg->region[i].name == currentSurface)
            {
                if(refi_Reg->region[i].mode =="inside" )
                    ui->cb_MeshVolumeMode->setCurrentIndex(1);
                else if(refi_Reg->region[i].mode =="outside" )
                    ui->cb_MeshVolumeMode->setCurrentIndex(2);
                ui->txt_Level_Volume->setText(QString::number(refi_Reg->region[i].lv2));
            }
        }
        GeomeSphereRegion *gSphereRegion = &mesh->snappyd->gSphereRegion;
        for(int i = 0; i < gSphereRegion->n; i++)
        {
            if(gSphereRegion->sphere[i].name == currentSurface)
            {
                ui->cb_VolumeType->setCurrentIndex(2);
                ui->txt_P_Sphe_Vol_X->setText(QString::number(gSphereRegion->sphere[i].centre.x));
                ui->txt_P_Sphe_Vol_Y->setText(QString::number(gSphereRegion->sphere[i].centre.y));
                ui->txt_P_Sphe_Vol_Z->setText(QString::number(gSphereRegion->sphere[i].centre.z));
                ui->txt_Radius_Sphe_Vol->setText(QString::number(gSphereRegion->sphere[i].radius));
                return;
            }
        }
    }
    if(flag_btnSurGeometry_Click ==true)
    {
        for(int i=0; i< mesh->snappyd->gBox.n; i++)
        {
            if(currentSurface==  mesh->snappyd->gBox.boxes[i].name)
            {
                ui->txt_GeometrySurfaceFileStl->setText("");

                if(mesh->snappyd->gBox.boxes[i].type =="searchableBox")
                    ui->cb_SurfaceType->setCurrentIndex(0);

                ui->txt_GeometrySurfaceName->setText(mesh->snappyd->gBox.boxes[i].name);
                ui->txt_Min_Box_Sur_X->setText(QString::number(mesh->snappyd->gBox.boxes[i].min.x));
                ui->txt_Min_Box_Sur_Y->setText(QString::number(mesh->snappyd->gBox.boxes[i].min.y));
                ui->txt_Min_Box_Sur_Z->setText(QString::number(mesh->snappyd->gBox.boxes[i].min.z));

                ui->txt_Max_Box_Sur_X->setText(QString::number(mesh->snappyd->gBox.boxes[i].max.x));
                ui->txt_Max_Box_Sur_Y->setText(QString::number(mesh->snappyd->gBox.boxes[i].max.y));
                ui->txt_Max_Box_Sur_Z->setText(QString::number(mesh->snappyd->gBox.boxes[i].max.z));
                return;
            }
        }
        for(int i=0; i< mesh->snappyd->gCylin.n; i++)
        {
            if(currentSurface==  mesh->snappyd->gCylin.cylins[i].name)
            {
                ui->txt_GeometrySurfaceFileStl->setText("");

                if(mesh->snappyd->gCylin.cylins[i].type =="searchableCylinder")
                    ui->cb_SurfaceType->setCurrentIndex(1);

                ui->txt_GeometrySurfaceName->setText(mesh->snappyd->gCylin.cylins[i].name);
                ui->txt_P1_Cyl_Sur_X->setText(QString::number(mesh->snappyd->gCylin.cylins[i].point1.x));
                ui->txt_P1_Cyl_Sur_Y->setText(QString::number(mesh->snappyd->gCylin.cylins[i].point1.y));
                ui->txt_P1_Cyl_Sur_Z->setText(QString::number(mesh->snappyd->gCylin.cylins[i].point1.z));

                ui->txt_P2_Cyl_Sur_X->setText(QString::number(mesh->snappyd->gCylin.cylins[i].point2.x));
                ui->txt_P2_Cyl_Sur_Y->setText(QString::number(mesh->snappyd->gCylin.cylins[i].point2.y));
                ui->txt_P2_Cyl_Sur_Z->setText(QString::number(mesh->snappyd->gCylin.cylins[i].point2.z));

                ui->txt_Radius_Cyl_Sur->setText(QString::number(mesh->snappyd->gCylin.cylins[i].radius));
                return;
            }
        }
        for(int i=0; i< mesh->snappyd->gSphere.n; i++)
        {
            if(currentSurface==  mesh->snappyd->gSphere.sphere[i].name)
            {
                ui->txt_GeometrySurfaceFileStl->setText("");

                if(mesh->snappyd->gSphere.sphere[i].type =="searchableSphere")
                    ui->cb_SurfaceType->setCurrentIndex(2);
                ui->txt_GeometrySurfaceName->setText(mesh->snappyd->gSphere.sphere[i].name);
                ui->txt_P_Sphe_Sur_X->setText(QString::number(mesh->snappyd->gSphere.sphere[i].centre.x));
                ui->txt_P_Sphe_Sur_Y->setText(QString::number(mesh->snappyd->gSphere.sphere[i].centre.y));
                ui->txt_P_Sphe_Sur_Z->setText(QString::number(mesh->snappyd->gSphere.sphere[i].centre.z));

                ui->txt_Radius_Sphe_Sur->setText(QString::number(mesh->snappyd->gSphere.sphere[i].radius));
                return;
            }
        }
        for(int i=0; i< mesh->snappyd->gUserDefine.n; i++)
        {
            if(currentSurface==  mesh->snappyd->gUserDefine.user_Defines[i].name)
            {
                ui->txt_GeometrySurfaceFileStl->setText(mesh->snappyd->gUserDefine.user_Defines[i].direction);

                ui->txt_GeometrySurfaceName->setText("");
                ui->cb_SurfaceType->setCurrentIndex(0);

                ui->txt_Min_Box_Sur_X->setText("");
                ui->txt_Min_Box_Sur_Y->setText("");
                ui->txt_Min_Box_Sur_Z->setText("");

                ui->txt_Max_Box_Sur_X->setText("");
                ui->txt_Max_Box_Sur_Y->setText("");
                ui->txt_Max_Box_Sur_Z->setText("");

                ui->txt_P1_Cyl_Sur_X->setText("");
                ui->txt_P1_Cyl_Sur_Y->setText("");
                ui->txt_P1_Cyl_Sur_Z->setText("");

                ui->txt_P2_Cyl_Sur_X->setText("");
                ui->txt_P2_Cyl_Sur_Y->setText("");
                ui->txt_P2_Cyl_Sur_Z->setText("");

                ui->txt_Radius_Cyl_Sur->setText("");

                return;
            }
        }
    }
    if(flag_btn_MeshLayer_Click ==true)
    {
        for(int i=0;i< mesh->snappyd->add_Layers_Controls.layers.size(); i++)
        {
            if(mesh->snappyd->add_Layers_Controls.layers[i].name == ui->tb_boundary->currentItem()->text())
            {
                ui->txt_Layer_Bounding_2->setText(QString::number(mesh->snappyd->add_Layers_Controls.layers[i].nSurfaceLayers));
                return;
            }
        }
        ui->txt_Layer_Bounding_2->setText("0");
    }
}

void MainWindow::on_tb_MeshSurface_itemSelectionChanged()
{
    if(ui->tb_MeshSurface->selectedItems().size() == 1){
        ui->txt_BoundaryName->setText(ui->tb_MeshSurface->currentItem()->text());
    }
}
