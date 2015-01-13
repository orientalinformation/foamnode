#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loaded = true;
    licenseOK = true;
    emitflag = 0;
    mesh = new DMesh();
    this->isClose = false;
    this->keepBoundary = false;
    ui->layout_Mesh->addWidget(mesh);
//    ui->txt_Log->setMaximumBlockCount(300);
    checkMeshThread = new MyThread();
    this->comment = "";
    this->logPath = "";
    this->table.clear();
    LoadControlItems();
    LoadControlsVisible();
    LoadLocationInMesh();
    ui->actionCheck_mesh->setDisabled(true);
    ui->actionParaView->setDisabled(true);
    ui->actionSave->setDisabled(true);
    ui->checkBox_WriteLog->setChecked(true);
    ui->tb_MeshRefineAroundSurface->horizontalHeader()->setStretchLastSection(true);
    ui->tb_boundary->setRowCount(0);

    SetButtonDefault();
    CheckLicense();
    if(licenseOK){
        checkDFC();
    }
    QPushButton *btn = new QPushButton();
    connect(btn,SIGNAL(clicked()),this,SLOT(setColorBoundary()));
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
            QMessageBox a(tr("License"),tr("The license has expired...!"),QMessageBox::Warning,QMessageBox::Yes,QMessageBox::Ok,QMessageBox::Abort);
            a.setButtonText(QMessageBox::Ok,tr("Buy License"));
            a.setButtonText(QMessageBox::Abort,tr("Register"));
            a.setButtonText(QMessageBox::Yes,tr("Trial"));
            a.addButton(QMessageBox::Cancel);
            a.setWindowFlags( this->windowFlags() | Qt::WindowStaysOnTopHint);
            a.exec();
            if(a.result() == QMessageBox::Ok)
            {
                SendMail *sendMail;
                sendMail = new SendMail();
                sendMail->SetMode(true);
                sendMail->exec();
                licenseOK = false;
            }
            if(a.result() == QMessageBox::Abort)
            {
                RegisterDialog *rg;
                rg = new RegisterDialog();
                rg->exec();
                if(rg->result())
                {
//                    ui->actionOpen->setEnabled(true);
                    licenseOK = true;
                    SetButtonEnable(true);
                    SetButtonDisplayEnable(true);
                }
            }
            if(a.result() == QMessageBox::Yes)
            {
                SendMail *sendMail;
                sendMail = new SendMail();
                sendMail->SetMode(false);
                sendMail->exec();
                licenseOK = false;
            }
        }
        else
        {
            SetButtonEnable(true);
            SetButtonDisplayEnable(true);
            licenseOK = true;
        }
    }
    else
    {
        SetButtonEnable(false);
        SetButtonDisplayEnable(false);
        licenseOK = false;
        QMessageBox a(tr("License"),tr("Please contact DFM-Europe for license key...!"),QMessageBox::Warning,QMessageBox::Yes,QMessageBox::Ok,QMessageBox::Abort);
        a.setButtonText(QMessageBox::Ok,tr("Buy License"));
        a.setButtonText(QMessageBox::Yes,tr("Trial"));
        a.setButtonText(QMessageBox::Abort,tr("Register"));
        a.addButton(QMessageBox::Cancel);
        a.exec();
        if(a.result() == QMessageBox::Ok)
        {
            SendMail *sendMail;
            sendMail = new SendMail();
            sendMail->SetMode(true);
            sendMail->exec();
            licenseOK = false;
        }
        if(a.result() == QMessageBox::Abort)
        {
            RegisterDialog *rg;
            rg = new RegisterDialog();
            rg->exec();
            if(rg->result())
            {
//                ui->actionOpen->setEnabled(true);
                SetButtonEnable(true);
                SetButtonDisplayEnable(true);
                licenseOK = true;
            }
        }
        if(a.result() == QMessageBox::Yes)
        {
            SendMail *sendMail;
            sendMail = new SendMail();
            sendMail->SetMode(false);
            sendMail->exec();
            licenseOK = false;
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
    ui->cb_GeometryNewMesh->addItem("STL");
    ui->cb_GeometryNewMesh->addItem("Simple surface");

    ui->rbn_Surface->setChecked(true);
}
void MainWindow::LoadBoundaryControlItems()
{
    if(this->keepBoundary == false){
        ui->cb_BoundaryType->clear();
        ui->cb_BoundaryType->addItem("empty");
        ui->cb_BoundaryType->addItem("symmetryPlane");
        ui->cb_BoundaryType->addItem("wall");
        ui->cb_BoundaryType->setCurrentIndex(2);
    }

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
    ui->frame_MeshRefinement->hide();
    ui->frame_MeshLayer->hide();
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

void MainWindow::LoadColor()
{
//    ui->
}

bool MainWindow::AddFaceToList(QString name, int type)
{
    int i = listFaces.indexOf(name);
    if(i > -1)
    {
        ui->tb_boundary->showRow(i);
//        loadData();
        return false;
    }
    listFaces.append(name);
    ui->tb_boundary->setColumnCount(1);
    ui->tb_boundary->setColumnWidth(0,300);
    ui->tb_boundary->setRowCount(listFaces.size());
    QTableWidgetItem *temp = new QTableWidgetItem(name);
    if(type != 0){
        if(type == 1){
            temp->setBackgroundColor(QColor(255,255,0));
        }else if(type == 2){
            temp->setBackgroundColor(QColor(0,255,0));
        }else{
            temp->setBackgroundColor(QColor(0,0,255));
            temp->setTextColor(QColor(255,255,255));
        }
    }
    ui->tb_boundary->setItem(listFaces.size()-1,0,temp);
    ui->tb_boundary->setEditTriggers(QTableWidget::NoEditTriggers);
    //loadData();
    return true;
}
void MainWindow::Thread_Changed(QString value)
{
//    QString logvalue = value;
    value = value.remove("\n");
//    if(ui->checkBox_WriteLog->isChecked() && comment != "checkMesh" && comment != "paraFoam"){
//        QFile file(logPath);
//        file.open(QIODevice::Append);
//        file.write(logvalue.toAscii().data());
//        file.close();
//    }
    if(comment == "checkMesh"){
        FilterLogMesh(value);
    }else
        ui->txt_Log->append(value);
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
//        ui->btn_DeleteSurface->setEnabled(true);
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
//        ui->btn_DeleteSurface->setEnabled(true);
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
        if(mesh->snappyd->gBox.n !=0 || mesh->snappyd->gCylin.n !=0 || mesh->snappyd->gSphere.n !=0 || mesh->snappyd->gUserDefine.n !=0 ||
           mesh->snappyd->gBoxCellZone.n !=0 || mesh->snappyd->gCylinCellZone.n !=0 ||
                mesh->snappyd->gSphereCellZone.n !=0 || mesh->snappyd->gUserDefineCellZone.n !=0)
        {
            ui->btn_MeshSurface->setEnabled(true);
            ui->btn_MeshVolume->setEnabled(true);
        }
        else if(mesh->snappyd->gBoxRegion.n != 0 || mesh->snappyd->gCylinRegion.n != 0 || mesh->snappyd->gSphereRegion.n != 0)
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
        //Calculate delta in base mesh

        if(mesh->blockd->vertice.n > 5)
        {
            mesh->snappyd->deltaBaseMesh.x = (mesh->blockd->vertice.v[6].x - mesh->blockd->vertice.v[0].x)/mesh->blockd->gen_Bounding.element.x;
            mesh->snappyd->deltaBaseMesh.y = (mesh->blockd->vertice.v[6].y - mesh->blockd->vertice.v[0].y)/mesh->blockd->gen_Bounding.element.y;
            mesh->snappyd->deltaBaseMesh.z = (mesh->blockd->vertice.v[6].z - mesh->blockd->vertice.v[0].z)/mesh->blockd->gen_Bounding.element.z;
        }
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
//        ui->btn_DeleteSurface->setEnabled(true);
        ui->txt_X_Location->setText(QString::number(mesh->xAverage));
        ui->txt_Y_Location->setText(QString::number(mesh->yAverage));
        ui->txt_Z_Location->setText(QString::number(mesh->zAverage));
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
//        ui->btn_DeleteSurface->setEnabled(true);
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
        QMessageBox::critical(this,tr("Error"),tr("Please input a valid name!"));
        return false;
    }
    //check name valid
    for(int i=0; i< mesh->snappyd->gBox.n; i++)
    {
        if(value==  mesh->snappyd->gBox.boxes[i].name)
        {
            QMessageBox::critical(this,tr("Error"),tr("This name is already existed, this will be replaced by new values (Coming soon)...!"));
            return false;
        }
    }
    for(int i=0; i< mesh->snappyd->gBoxRegion.n; i++)
    {
        if(value==  mesh->snappyd->gBoxRegion.boxes[i].name)
        {
            QMessageBox::critical(this,tr("Error"),tr("This name is already existed, this will be replaced by new values (Coming soon)...!"));
            return false;
        }
    }
    for(int i=0; i< mesh->snappyd->gCylin.n; i++)
    {
        if(value==  mesh->snappyd->gCylin.cylins[i].name)
        {
            QMessageBox::critical(this,tr("Error"),tr("This name is already existed, this will be replaced by new values (Coming soon)...!"));
            return false;
        }
    }
    for(int i=0; i< mesh->snappyd->gCylinRegion.n; i++)
    {
        if(value==  mesh->snappyd->gCylinRegion.cylins[i].name)
        {
            QMessageBox::critical(this,tr("Error"),tr("This name is already existed, this will be replaced by new values (Coming soon)...!"));
            return false;
        }
    }
    for(int i=0; i< mesh->snappyd->gSphere.n; i++)
    {
        if(value==  mesh->snappyd->gSphere.sphere[i].name)
        {
            QMessageBox::critical(this,tr("Error"),tr("This name is already existed, this will be replaced by new values (Coming soon)...!"));
            return false;
        }
    }
    for(int i=0; i< mesh->snappyd->gSphereRegion.n; i++)
    {
        if(value==  mesh->snappyd->gSphereRegion.sphere[i].name)
        {
            QMessageBox::critical(this,tr("Error"),tr("This name is already existed, this will be replaced by new values (Coming soon)...!"));
            return false;
        }
    }
    for(int i=0; i< mesh->snappyd->gUserDefine.n; i++)
    {
        if(value==  mesh->snappyd->gUserDefine.user_Defines[i].name)
        {
            QMessageBox::critical(this,tr("Error"),tr("This name is already existed, this will be replaced by new values (Coming soon)...!"));
            return false;
        }
    }
    foreach(Boundary b,mesh->patchDict->boundaries)
    {
        if(b.name == value)
        {
            QMessageBox::critical(this,tr("Error"),tr("This name is already existed in boundaries...!"));
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
        QMessageBox::critical(this,tr("Error"),tr("Please input all values in this form...!"));
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
        QMessageBox::critical(this,tr("Error"),tr("Please input all values in this form...!"));
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
        QMessageBox::critical(this,tr("Error"),tr("Please input all values in this form...!"));
        return false;
    }    
    return true;
}

bool MainWindow::AddSurfaceRegionBox(QString surfaceName,int min, int max)
{
    RefinementSurfaces *refi_Sur;
    //snappy box
    for(int i = 0; i< mesh->snappyd->gBox.refi_Sur.n; i++)
    {
        refi_Sur = &mesh->snappyd->gBox.refi_Sur;
        //update if exist
        if(refi_Sur->surfaces[i].name == surfaceName)
        {
            refi_Sur->surfaces[i].lv1= min;
            refi_Sur->surfaces[i].lv2= max;
            return true;
        }
    }
    //snappy cylinder
    for(int i = 0; i< mesh->snappyd->gCylin.refi_Sur.n; i++)
    {
        refi_Sur = &mesh->snappyd->gCylin.refi_Sur;
        //update if exist
        if(refi_Sur->surfaces[i].name == surfaceName)
        {
            refi_Sur->surfaces[i].lv1= min;
            refi_Sur->surfaces[i].lv2= max;
            return true;
        }
    }
    //snappy sphere
    for(int i = 0; i< mesh->snappyd->gSphere.refi_Sur.n; i++)
    {
        refi_Sur = &mesh->snappyd->gSphere.refi_Sur;
        //update if exist
        if(refi_Sur->surfaces[i].name == surfaceName)
        {
            refi_Sur->surfaces[i].lv1= min;
            refi_Sur->surfaces[i].lv2= max;
            return true;
        }
    }
    //snappy box cell zone
    for(int i = 0; i< mesh->snappyd->gBoxCellZone.refi_Sur.n; i++)
    {
        refi_Sur = &mesh->snappyd->gBoxCellZone.refi_Sur;
        //update if exist
        if(refi_Sur->surfaces[i].name == surfaceName)
        {
            refi_Sur->surfaces[i].lv1= min;
            refi_Sur->surfaces[i].lv2= max;
            return true;
        }
    }
    //snappy cylinder cell zone
    for(int i = 0; i< mesh->snappyd->gCylinCellZone.refi_Sur.n; i++)
    {
        refi_Sur = &mesh->snappyd->gCylinCellZone.refi_Sur;
        //update if exist
        if(refi_Sur->surfaces[i].name == surfaceName)
        {
            refi_Sur->surfaces[i].lv1= min;
            refi_Sur->surfaces[i].lv2= max;
            return true;
        }
    }
    //snappy sphere cell zone
    for(int i = 0; i< mesh->snappyd->gSphereCellZone.refi_Sur.n; i++)
    {
        refi_Sur = &mesh->snappyd->gSphereCellZone.refi_Sur;
        //update if exist
        if(refi_Sur->surfaces[i].name == surfaceName)
        {
            refi_Sur->surfaces[i].lv1= min;
            refi_Sur->surfaces[i].lv2= max;
            return true;
        }
    }
    return false;
}

bool MainWindow::AddUserDefine(QString surfaceName,int min, int max)
{
    GeomeUserDefine *gUserDefine = &mesh->snappyd->gUserDefine;
    for(int i = 0; i< gUserDefine->refi_Sur.n; i++)
    {
        QString currentSurface = gUserDefine->refi_Sur.surfaces[i].name;
        if(currentSurface == surfaceName)
        {
            gUserDefine->refi_Sur.surfaces[i].lv1 = min;
            gUserDefine->refi_Sur.surfaces[i].lv2 = max;
            for(int j = 0; j< gUserDefine->refi_Fea.n; j++)
            {
                QString currentSurface = gUserDefine->refi_Fea.feature[j].name;
                if(currentSurface == surfaceName)
                {
                    gUserDefine->refi_Fea.feature[j].lv = max;
                    return true;
                }
            }
        }
    }
    gUserDefine = &mesh->snappyd->gUserDefineCellZone;
    for(int i = 0; i< gUserDefine->refi_Sur.n; i++)
    {
        QString currentSurface = gUserDefine->refi_Sur.surfaces[i].name;
        if(currentSurface == surfaceName)
        {
            gUserDefine->refi_Sur.surfaces[i].lv1 = min;
            gUserDefine->refi_Sur.surfaces[i].lv2 = max;
            for(int j = 0; j< gUserDefine->refi_Fea.n; j++)
            {
                QString currentSurface = gUserDefine->refi_Fea.feature[j].name;
                if(currentSurface == surfaceName)
                {
                    gUserDefine->refi_Fea.feature[j].lv = max;
                    return true;
                }
            }
        }
    }
    return false;
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
                    distance->setTextAlignment(Qt::AlignCenter);
                    QTableWidgetItem *level = new QTableWidgetItem(QString::number(gBox->refi_Reg.region[i].distances[j].lv2));
                    level->setTextAlignment(Qt::AlignCenter);
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
                    distance->setTextAlignment(Qt::AlignCenter);
                    QTableWidgetItem *level = new QTableWidgetItem(QString::number(gCylin->refi_Reg.region[i].distances[j].lv2));
                    level->setTextAlignment(Qt::AlignCenter);
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
                    distance->setTextAlignment(Qt::AlignCenter);
                    QTableWidgetItem *level = new QTableWidgetItem(QString::number(gUserDefine->refi_Reg.region[i].distances[j].lv2));
                    level->setTextAlignment(Qt::AlignCenter);
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
                    distance->setTextAlignment(Qt::AlignCenter);
                    QTableWidgetItem *level = new QTableWidgetItem(QString::number(gSphere->refi_Reg.region[i].distances[j].lv2));
                    level->setTextAlignment(Qt::AlignCenter);
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
    //Find in box cell zone
    if(type == 4)
    {
        GeomeBoxSurface *gBox = &mesh->snappyd->gBoxCellZone;
        for(int i = 0; i< gBox->refi_Reg.n; i++)
        {
            if(gBox->refi_Reg.region[i].name == currentSurface)
            {
                int rSize = gBox->refi_Reg.region[i].distances.size();
                ui->tb_MeshRefineAroundSurface->setRowCount(rSize + 1);
                for(int j = 0; j < rSize; j++)
                {
                    QTableWidgetItem *distance = new QTableWidgetItem(QString::number(gBox->refi_Reg.region[i].distances[j].lv1));
                    distance->setTextAlignment(Qt::AlignCenter);
                    QTableWidgetItem *level = new QTableWidgetItem(QString::number(gBox->refi_Reg.region[i].distances[j].lv2));
                    level->setTextAlignment(Qt::AlignCenter);
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
    //Find in cylinder cell zone
    else if(type == 5)
    {
        GeomeCylinderSurface *gCylin = &mesh->snappyd->gCylinCellZone;
        for(int i = 0; i< gCylin->refi_Reg.n; i++)
        {
            if(gCylin->refi_Reg.region[i].name == currentSurface)
            {
                int rSize = gCylin->refi_Reg.region[i].distances.size();
                ui->tb_MeshRefineAroundSurface->setRowCount(rSize + 1);
                for(int j = 0; j < rSize; j++)
                {
                    QTableWidgetItem *distance = new QTableWidgetItem(QString::number(gCylin->refi_Reg.region[i].distances[j].lv1));
                    distance->setTextAlignment(Qt::AlignCenter);
                    QTableWidgetItem *level = new QTableWidgetItem(QString::number(gCylin->refi_Reg.region[i].distances[j].lv2));
                    level->setTextAlignment(Qt::AlignCenter);
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
    //Find in sphere cell zone
    else if(type == 6)
    {
        GeomeSphereSurface *gSphere = &mesh->snappyd->gSphereCellZone;
        for(int i = 0; i< gSphere->refi_Reg.n; i++)
        {
            if(gSphere->refi_Reg.region[i].name == currentSurface)
            {
                int rSize = gSphere->refi_Reg.region[i].distances.size();
                ui->tb_MeshRefineAroundSurface->setRowCount(rSize + 1);
                for(int j = 0; j < rSize; j++)
                {
                    QTableWidgetItem *distance = new QTableWidgetItem(QString::number(gSphere->refi_Reg.region[i].distances[j].lv1));
                    distance->setTextAlignment(Qt::AlignCenter);
                    QTableWidgetItem *level = new QTableWidgetItem(QString::number(gSphere->refi_Reg.region[i].distances[j].lv2));
                    level->setTextAlignment(Qt::AlignCenter);
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
    //Find in STL cell zone
    else if(type == 7)
    {
        GeomeUserDefine *gUserDefine = &mesh->snappyd->gUserDefineCellZone;
        for(int i = 0;i< gUserDefine->refi_Reg.n; i++)
        {
            if(gUserDefine->refi_Reg.region[i].name == currentSurface)
            {

                int rSize = gUserDefine->refi_Reg.region[i].distances.size();
                ui->tb_MeshRefineAroundSurface->setRowCount(rSize + 1);
                for(int j = 0; j < rSize; j++)
                {
                    QTableWidgetItem *distance = new QTableWidgetItem(QString::number(gUserDefine->refi_Reg.region[i].distances[j].lv1));
                    distance->setTextAlignment(Qt::AlignCenter);
                    QTableWidgetItem *level = new QTableWidgetItem(QString::number(gUserDefine->refi_Reg.region[i].distances[j].lv2));
                    level->setTextAlignment(Qt::AlignCenter);
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

void MainWindow::ReloadRefineDistanceSurface(RefinementRegion *refi_Reg)
{
    int rSize = refi_Reg->distances.size();
    ui->tb_MeshRefineAroundSurface->setRowCount(rSize + 1);
    for(int j = 0; j < rSize; j++)
    {
        QTableWidgetItem *distance = new QTableWidgetItem(QString::number(refi_Reg->distances[j].lv1));
        distance->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *level = new QTableWidgetItem(QString::number(refi_Reg->distances[j].lv2));
        level->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *action = new QTableWidgetItem(QIcon(":/resource/images/delete-icon.png"),"");
        ui->tb_MeshRefineAroundSurface->setItem(j,0,distance);
        ui->tb_MeshRefineAroundSurface->setItem(j,1,level);
        ui->tb_MeshRefineAroundSurface->setItem(j,2,action);
    }
    QTableWidgetItem *action = new QTableWidgetItem(QIcon(":/resource/images/tick.png"),"");
    ui->tb_MeshRefineAroundSurface->setItem(ui->tb_MeshRefineAroundSurface->rowCount() - 1,2,action);
    ui->tb_MeshRefineAroundSurface->setItem(ui->tb_MeshRefineAroundSurface->rowCount() - 1,0,new QTableWidgetItem(""));
    ui->tb_MeshRefineAroundSurface->setItem(ui->tb_MeshRefineAroundSurface->rowCount() - 1,1,new QTableWidgetItem(""));
}

void MainWindow::loadData()
{
    if(ui->tb_boundary->rowCount() <= 0){
        return;
    }
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
        refi_Reg = &mesh->snappyd->gBoxCellZone.refi_Reg;
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
        refi_Reg = &mesh->snappyd->gCylinCellZone.refi_Reg;
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
        refi_Reg = &mesh->snappyd->gSphereCellZone.refi_Reg;
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
        refi_Reg = &mesh->snappyd->gUserDefineCellZone.refi_Reg;
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
    }
    if(flag_btnSurGeometry_Click ==true)
    {
        GeomeBoxSurface *gBox = &mesh->snappyd->gBox;
        for(int i=0; i< gBox->n; i++)
        {
            if(currentSurface==  gBox->boxes[i].name)
            {
                ui->txt_GeometrySurfaceFileStl->setText("");

                if(gBox->boxes[i].type =="searchableBox")
                    ui->cb_SurfaceType->setCurrentIndex(0);

                ui->txt_GeometrySurfaceName->setText(gBox->boxes[i].name);
                ui->txt_Min_Box_Sur_X->setText(QString::number(gBox->boxes[i].min.x));
                ui->txt_Min_Box_Sur_Y->setText(QString::number(gBox->boxes[i].min.y));
                ui->txt_Min_Box_Sur_Z->setText(QString::number(gBox->boxes[i].min.z));

                ui->txt_Max_Box_Sur_X->setText(QString::number(gBox->boxes[i].max.x));
                ui->txt_Max_Box_Sur_Y->setText(QString::number(gBox->boxes[i].max.y));
                ui->txt_Max_Box_Sur_Z->setText(QString::number(gBox->boxes[i].max.z));
                break;
            }
        }
        gBox = &mesh->snappyd->gBoxCellZone;
        for(int i=0; i< mesh->snappyd->gBoxCellZone.n; i++)
        {
            if(currentSurface==  gBox->boxes[i].name)
            {
                ui->txt_GeometrySurfaceFileStl->setText("");

                if(gBox->boxes[i].type =="searchableBox")
                    ui->cb_SurfaceType->setCurrentIndex(0);

                ui->txt_GeometrySurfaceName->setText(gBox->boxes[i].name);
                ui->txt_Min_Box_Sur_X->setText(QString::number(gBox->boxes[i].min.x));
                ui->txt_Min_Box_Sur_Y->setText(QString::number(gBox->boxes[i].min.y));
                ui->txt_Min_Box_Sur_Z->setText(QString::number(gBox->boxes[i].min.z));

                ui->txt_Max_Box_Sur_X->setText(QString::number(gBox->boxes[i].max.x));
                ui->txt_Max_Box_Sur_Y->setText(QString::number(gBox->boxes[i].max.y));
                ui->txt_Max_Box_Sur_Z->setText(QString::number(gBox->boxes[i].max.z));
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

void MainWindow::checkDFC()
{
    if(!QFile("DOFICore").exists()){
        QMessageBox *messDFC = new QMessageBox(tr("DMesh"),tr("DMesh needs OpenFOAM solvers for running. Please install it!"),QMessageBox::Question,QMessageBox::Yes,QMessageBox::No,QMessageBox::NoButton);
        messDFC->setWindowFlags( this->windowFlags() | Qt::WindowStaysOnTopHint);
        if(messDFC->exec() == QMessageBox::Yes){
            #if defined(Q_OS_WIN)
                QDesktopServices::openUrl(QUrl("http://dofilink.com/files/DOFICore/OpenFOAM_2.1.0_Win_64bits.exe"));
    //                    this->close();
            #else
                QDesktopServices::openUrl(QUrl("http://dofilink.com/files/DOFICore/OpenFOAM.tar.gz"));
    //                    this->close();
            #endif
            loaded = false;
        }else{
            loaded = true;
        }
        CheckDFC = false;
    }else {
        CheckDFC = true;
    }
}

void MainWindow::FilterLogMesh(QString value)
{
    StrLogCheckMesh = value;
    StrLogCheckMesh = StrLogCheckMesh.trimmed();
    int padding = 40;
    bool ok;
    if(emitflag == 0){
        ui->txt_Log->append(value);
    }
    if(StrLogCheckMesh == "Mesh stats" || StrLogCheckMesh == "Overall number of cells of each type:"){
        if(StrLogCheckMesh == "Overall number of cells of each type:"){
            padding = 80;
        }
        emitflag = 1;
    }
    if(StrLogCheckMesh == "Checking topology..."){
        emitflag = 2;
    }
    if(StrLogCheckMesh == "Checking patch topology for multiply connected surfaces ..."){
        emitflag = 3;
    }
    if(StrLogCheckMesh == "Checking geometry..."){
        StrLogCheckMesh+="\n";
        emitflag = 0;
    }
    if(emitflag == 1){
        QList<QString> tablename;
        QList<QString> tablevalue;
        QStringList listname;
        QStringList listvalue;
        QString str_value;
        if(StrLogCheckMesh != ""){
            QStringList list_str = StrLogCheckMesh.split(QRegExp("\\s+"));
            table.append(list_str);
        }else{
            table.removeFirst();
            for(int i = 0; i< table.length(); i++){
                for(int j = 0; j < table[i].length(); j++){
                    table[i][j].toFloat(&ok);
                    if(ok == false){
                        if(j == 0){
                            listname.append(table[i][j]);
                        }else{
                            listname.append(" "+table[i][j]);
                        }
                    }else{
                        listvalue.append("\t"+table[i][j]);
                    }
                }
                if(listname.length() == 0){
                    tablename.append("\t");
                }else{
                    int j = 0;
                    while(j+1<listname.length()){
                        listname[0]+=listname[j+1];
                        j++;
                    }
                    tablename.append(listname[0]);
                }
                listname.clear();
                if(listvalue.length() == 0){
                    //listvalue.append();
                    tablevalue.append(" ");
                }else{
                    tablevalue.append(listvalue);
                }
                listvalue.clear();
            }
            table.clear();
            str_value = "<table>";
            for(int i = 0; i< tablename.length(); i++){
                if(tablename[i] == "" && tablevalue[i] == ""){
                    tablename.removeAt(i);
                    tablevalue.removeAt(i);
                }
                if(tablename[i] != "" && tablevalue[i] == ""){
                    str_value += "<tr><td colspan = \"4\" >"+tablename[i]+"</td></tr>";
                }
                if(tablename[i] != "" && tablevalue[i] != ""){
                    str_value += "<tr><td style = \"padding-left: "+QString::number(padding) +";\">"+tablename[i]+"</td><td>\t</td><td style = \"padding-left: 40px;\">"+tablevalue[i]+"</td</tr>";
                }
            }
            str_value += "</table>";
            StrLogCheckMesh.append(str_value);
            ui->txt_Log->append(StrLogCheckMesh);
            ui->txt_Log->toHtml();
            emitflag = 0;

        }
    }

    if(emitflag == 2){
        QList<QString> tablename;
        QList<QStringList> tablevalue;
        QStringList listname;
        QStringList listvalue;
        QString str_value;
        if(StrLogCheckMesh != ""){
            QStringList list_str = StrLogCheckMesh.split(QRegExp("\\s+"));
            table.append(list_str);
        }else{
            table.removeFirst();
            for(int i = 0; i< table.length(); i++){
                for(int j = 0; j < table[i].length(); j++){
                    table[i][j].toFloat(&ok);
                    if(ok == false && table[i][j].contains("OK") == false){
                        if(j == 0){
                            listname.append(table[i][j]);
                        }else{
                            listname.append(" "+table[i][j]);
                        }
                    }else{
                        if(ok == true){
                            listvalue.append(table[i][j]);
                        }else{
                            listvalue.append("\t"+table[i][j]);
                        }
                    }
                }
                if(listname.length() == 0){
                    tablename.append("\t");
                }else{
                    int j = 0;
                    while(j+1<listname.length()){
                        listname[0]+=listname[j+1];
                        j++;
                    }
                    tablename.append(listname[0]);
                }
                listname.clear();
                if(listvalue.length() == 0){
                    listvalue.append(" ");
                }
                tablevalue.append(listvalue);
                listvalue.clear();
            }
            table.clear();
            int t = 0;
            int td = 0;
            while(t < tablevalue.length()){
                if(td < tablevalue[t].length()){
                    td = tablevalue[t].length();
                }
                t++;
            }
            QStringList a;
            QString b;
            int k = 0;
            while(k < tablevalue.length()){
                int tt = td - tablevalue[k].length();
                b = "";
                if(tt > 0){
                    for(int i = 0; i< tt ; i++){
                        b+= "<td style = \"padding-left: 40px;\"></td>";
                    }
                }
                for(int i = 0; i< tablevalue[k].length(); i++){
                    b+= "<td style = \"padding-left: 40px;\">"+tablevalue[k][i]+"</td>";
                }
                a.append(b);
                k++;
            }
            str_value = "<table>";
            for(int i = 0; i< tablename.length(); i++){
                if(tablename[i] == "" && a[i] == ""){
                    tablename.removeAt(i);
                    tablevalue.removeAt(i);
                }
                if(tablename[i] != "" && a[i] == ""){
                    str_value += "<tr><td colspan = \"4\" >"+tablename[i]+"</td></tr>";
                }
                if(tablename[i] != "" && a[i] != ""){
                    str_value += "<tr><td style = \"padding-left: 40px;\">"+tablename[i]+"</td><td>\t</td>"+a[i]+"</tr>";
                }
            }
            str_value += "</table>";
            StrLogCheckMesh.append(str_value);
            ui->txt_Log->append(StrLogCheckMesh);
            ui->txt_Log->toHtml();
            emitflag = 0;

        }
    }

    if(emitflag == 3){
        QList<QString> tablename;
        QList<QString> tablevalue;
        QStringList listname;
        QStringList listvalue;
        QStringList tmp;
        int max;
        if(StrLogCheckMesh != ""){
            QString end_str;
            if(StrLogCheckMesh.contains(" ok "))
            {
                end_str = StrLogCheckMesh.right(StrLogCheckMesh.length() - StrLogCheckMesh.indexOf(" ok "));
                StrLogCheckMesh = StrLogCheckMesh.left(StrLogCheckMesh.indexOf(" ok ") -1);
            }
            QStringList list_str = StrLogCheckMesh.split(QRegExp("\\s+"));
            list_str.append(end_str);
            table.append(list_str);
        }else{
            table.removeFirst();
            for(int i = 0; i< table.length(); i++){
                int j = 0;
                //bool end = false;
                while(j < table[i].length()){
                    table[i][j].toFloat(&ok);
                    if(ok == false && table[i][j].contains(" ok ") == false){
                        if(j == 0){
                            listname.append(table[i][j]);
                        }else{
                            listname.append(" "+table[i][j]);
                        }
                        j++;
                    }else{
                        listvalue.append("\t"+table[i][j]);
                        j++;
                    }
                }
                if(listname.length() == 0){
                    tablename.append("\t");
                }else{
                    if(i == 0){
                        tmp = listname;
                        listname[0] = " ";
                    }else{
                        int j = 0;
                        while(j+1<listname.length()){
                            listname[0]+=listname[j+1];
                            j++;
                        }
                    }
                    tablename.append(listname[0]);
                }
                listname.clear();
                if(listvalue.length() == 0){
                    tablevalue.append(" ");
                }else{
                    int j = 1;
                    while(j<listvalue.length()){
                        listvalue[0]+=listvalue[j];
                        j++;
                    }
                    tablevalue.append(listvalue[0]);
                }
                listvalue.clear();
            }
            table.clear();
            tablename[0] = tablename.value(0).trimmed();
            max = tablename.value(0).length();
            int i = 1;
            while (i<tablename.length()) {
                tablename.value(i) = tablename.value(i).trimmed();
                if(max < tablename.value(i).length()){
                    max = tablename.value(i).length();
                }
                i++;
            }
            if(max < 11){
                max = 1;
            }else{
                if((max%11) != 0){
                    max = max/11+1;
                }else{
                    max = max/11;
                }
            }
            QString str_value;
            tmp[0] = tmp.value(0).trimmed();
            int lt = tmp.value(0).length();
            if(lt < 11){
                lt = 0;
            }else{
                lt = lt/11;
            }
            int t = max - lt;
            if(t > 0){
                int k = 0;
                while(k < t){
                    tmp[0] += "\t";
                    k++;
                }
            }
            int tt =1;
            while(tt<tmp.length()){
                if((tt) == (tmp.length() -2)){
                    tmp[0]+=tmp[tt];
                }else{
                    tmp[0]+=("\t"+tmp[tt]);
                }
                tt++;
            }
            str_value = "\n\t"+tmp.value(0)+"\n";
            for(int j = 0; j < tablename.length(); j++){
                int l_i= tablename.value(j).length();
                if(l_i < 11){
                    l_i = 0;
                }else{
                    l_i = l_i/11;
                }
                int l = max - l_i;
                if(l > 0){
                    int k = 0;
                    while(k < l){
                        tablename[j]+="\t";
                        k++;
                    }
                }
                str_value+="\t"+tablename[j]+tablevalue[j]+"\n";
            }
            max=0;
            StrLogCheckMesh.append(str_value);
            ui->txt_Log->append(StrLogCheckMesh);
            emitflag = 0;

        }
    }

    if(emitflag == 4){
        ui->txt_Log->append("\t"+StrLogCheckMesh);
        emitflag = 0;

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
//    ui->btn_DeleteSurface->setEnabled(true);
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
    for(int i=0; i< mesh->snappyd->gBoxCellZone.n; i++)
    {
        AddFaceToList(mesh->snappyd->gBoxCellZone.boxes[i].name);
    }
    for(int i=0; i< mesh->snappyd->gCylinCellZone.n; i++)
    {
        AddFaceToList(mesh->snappyd->gCylinCellZone.cylins[i].name);
    }
    for(int i=0; i< mesh->snappyd->gSphereCellZone.n; i++)
    {
        AddFaceToList(mesh->snappyd->gSphereCellZone.sphere[i].name);
    }
    for(int i=0; i< mesh->snappyd->gUserDefineCellZone.n; i++)
    {
        AddFaceToList(mesh->snappyd->gUserDefineCellZone.user_Defines[i].name);
    }
}
void MainWindow::on_btn_Bounding_clicked()
{
    ui->frame_GeometrySurface->hide();
    ui->frame_BoundingGeometryType->hide();
    ui->frame_GeometryBounding->show();
//    ui->btn_DeleteSurface->setEnabled(false);
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
    file_name_STLs = QFileDialog::getOpenFileNames(this,"Import file stl",lastFileSTL,tr("Slt (*.stl)"));
    if(file_name_STLs.size() > 0) {
        lastFileSTL = file_name_STLs[0];
        QString files = "";
        foreach (QString sTL, file_name_STLs) {
            files.append(QFileInfo(sTL).fileName() + ",");
        }
        ui->txt_GeometrySurfaceFileStl->setText(files);
    }
}
void MainWindow::ImportSTLSurface()
{
    if(ui->txt_GeometrySurfaceFileStl->text() == "")
    {
        QMessageBox::information(this,tr("Error"),tr("Please select a STL file!"));
        return;
    }
    else
    {
        foreach(QString file_name_STL,file_name_STLs) {
            QFile file(file_name_STL);
            //Get filename and name of folder containds file
            QString name = QFileInfo(file).fileName().append(".stl");
            QString _name = QFileInfo(file).fileName();
    //        QString folder_STl = file_name_STL.split(file_name[file_name.length()-1])[0];
            //check name exists
            GeomeUserDefine *gUserDefine = &mesh->snappyd->gUserDefine;
            for(int i=0; i< gUserDefine->n; i++)
            {
                if(name ==  gUserDefine->user_Defines[i].name_file)
                {
                    QMessageBox::information(this,tr("Error"),tr("This name is already exists"));
                    return;
                }
            }

            //read file
            QString file1;
            if(file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                file1= file.readAll();
            }
            if(file1 == "")
            {
                QMessageBox::information(this,tr("Error"),tr("File is empty!"));
                return;
            }


            gUserDefine->n= gUserDefine->n+1;
            gUserDefine->user_Defines.resize(gUserDefine->n);
            gUserDefine->user_Defines.last().direction= file_name_STL;
            gUserDefine->user_Defines.last().name_file=name;
            gUserDefine->user_Defines.last().type="triSurfaceMesh";
            gUserDefine->user_Defines.last().name=_name;
            AddFaceToList(_name,1);

            //add surface
            gUserDefine->refi_Sur.n=gUserDefine->refi_Sur.n+1;
            gUserDefine->refi_Sur.surfaces.resize(gUserDefine->refi_Sur.n);
            gUserDefine->refi_Sur.surfaces.last().lv1=0;
            gUserDefine->refi_Sur.surfaces.last().lv2=0;
            gUserDefine->refi_Sur.surfaces.last().name=_name;
            //add reg
            gUserDefine->refi_Reg.n=gUserDefine->refi_Reg.n+1;
            gUserDefine->refi_Reg.region.resize(gUserDefine->refi_Reg.n);
            gUserDefine->refi_Reg.region.last().mode = "distance";
            gUserDefine->refi_Reg.region.last().name=_name;
            gUserDefine->refi_Reg.region.last().lv1 = 0;
            gUserDefine->refi_Reg.region.last().lv2 = 0;
            gUserDefine->refi_Reg.region.last().n = 0;
            //feature
            gUserDefine->refi_Fea.n = gUserDefine->refi_Fea.n + 1;
            gUserDefine->refi_Fea.feature.resize(gUserDefine->refi_Fea.n);
            gUserDefine->refi_Fea.feature.last().name = _name;
            gUserDefine->refi_Fea.feature.last().angle = 150;
            gUserDefine->refi_Fea.feature.last().lv = 0;
            //chua co region nao
            gUserDefine->refi_Sur.surfaces.last().n=0;
            int index =0;
            QStringList lines = file1.split("\n",QString::SkipEmptyParts);
            for(int i=0; i< lines.length(); i++)
            {
                if(lines[i].left(5)=="solid")
                {
                    QString name_re = lines[i].split(" ")[1];
                    gUserDefine->refi_Sur.surfaces.last().regionSTLs.resize(index +1);
                    gUserDefine->refi_Sur.surfaces.last().regionSTLs[index].name=name_re;
                    gUserDefine->refi_Sur.surfaces.last().regionSTLs[index].lv1=0;
                    gUserDefine->refi_Sur.surfaces.last().regionSTLs[index].lv2=0;
                    index++;
                }
            }
            gUserDefine->refi_Sur.surfaces.last().n=index;

            mesh->snappyd->min_Max.max.x=-1000000.0;
            mesh->snappyd->min_Max.max.y=-1000000.0;
            mesh->snappyd->min_Max.max.z=-1000000.0;

            mesh->snappyd->min_Max.min.x=1000000.0;
            mesh->snappyd->min_Max.min.y=1000000.0;
            mesh->snappyd->min_Max.min.z=1000000.0;
            //Read file STL
            if(mesh->snappyd->ReadSTLFile(file_name_STL,1))
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
        file_name_STLs.clear();
    }
}
void MainWindow::ImportSTLCellzone()
{
    if(ui->txt_GeometrySurfaceFileStl->text()=="")
    {
        QMessageBox::information(this,tr("Error"),tr("Please select a STL file!"));
        return;
    }
    else
    {
        foreach(QString file_name_STL,file_name_STLs) {
            //Get filename and name of folder containds file
            QFile file(file_name_STL);
            //Get filename and name of folder containds file
            QString name = QFileInfo(file).fileName().append(".stl");
            QString _name = QFileInfo(file).fileName();
    //        QString folder_STl = file_name_STL.split(file_name[file_name.length()-1])[0];
            //check name exists
            GeomeUserDefine *gUserDefine = &mesh->snappyd->gUserDefineCellZone;
            for(int i=0; i< gUserDefine->n; i++)
            {
                if(name==  gUserDefine->user_Defines[i].name_file)
                {
                    QMessageBox::information(this,tr("Error"),tr("This name is already exists"));
                    return;
                }
            }

            //read file
            QString file1;
            if(file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                 file1= file.readAll();
            }
            if(file1 == "")
            {
                QMessageBox::information(this,tr("Error"),tr("File is empty!"));
                return;
            }

            gUserDefine->n= gUserDefine->n+1;
            gUserDefine->user_Defines.resize(gUserDefine->n);
            gUserDefine->user_Defines.last().direction= file_name_STL;
            gUserDefine->user_Defines.last().name_file=name;
            gUserDefine->user_Defines.last().type="triSurfaceMesh";
            gUserDefine->user_Defines.last().name=_name;
            AddFaceToList(_name,3);

            //add surface
            gUserDefine->refi_Sur.n = gUserDefine->refi_Sur.n + 1;
            gUserDefine->refi_Sur.surfaces.resize(gUserDefine->refi_Sur.n);
            gUserDefine->refi_Sur.surfaces.last().lv1=0;
            gUserDefine->refi_Sur.surfaces.last().lv2=0;
            gUserDefine->refi_Sur.surfaces.last().name=_name;
            //add reg
            gUserDefine->refi_Reg.n=gUserDefine->refi_Reg.n+1;
            gUserDefine->refi_Reg.region.resize(gUserDefine->refi_Reg.n);
            gUserDefine->refi_Reg.region.last().mode = "inside";
            gUserDefine->refi_Reg.region.last().name=_name;
            gUserDefine->refi_Reg.region.last().lv1 = 0;
            gUserDefine->refi_Reg.region.last().lv2 = 0;
            gUserDefine->refi_Reg.region.last().n = 0;
            //feature
            gUserDefine->refi_Fea.n = gUserDefine->refi_Fea.n + 1;
            gUserDefine->refi_Fea.feature.resize(gUserDefine->refi_Fea.n);
            gUserDefine->refi_Fea.feature.last().name = _name;
            gUserDefine->refi_Fea.feature.last().angle = 150;
            gUserDefine->refi_Fea.feature.last().lv = 0;
            //chua co region nao
            gUserDefine->refi_Sur.surfaces.last().n=0;
            int index =0;
            QStringList lines = file1.split("\n",QString::SkipEmptyParts);
            for(int i=0; i< lines.length(); i++)
            {
                if(lines[i].left(5)=="solid")
                {
                    QString name_re = lines[i].split(" ")[1];
                    gUserDefine->refi_Sur.surfaces.last().regionSTLs.resize(index +1);
                    gUserDefine->refi_Sur.surfaces.last().regionSTLs[index].name=name_re;
                    gUserDefine->refi_Sur.surfaces.last().regionSTLs[index].lv1=0;
                    gUserDefine->refi_Sur.surfaces.last().regionSTLs[index].lv2=0;
                    index++;
                }
            }
            gUserDefine->refi_Sur.surfaces.last().n=index;

            mesh->snappyd->min_Max.max.x=-1000000.0;
            mesh->snappyd->min_Max.max.y=-1000000.0;
            mesh->snappyd->min_Max.max.z=-1000000.0;

            mesh->snappyd->min_Max.min.x=1000000.0;
            mesh->snappyd->min_Max.min.y=1000000.0;
            mesh->snappyd->min_Max.min.z=1000000.0;
            //Read file STL
            if(mesh->snappyd->ReadSTLFile(file_name_STL,0))
            {
                QStringList views = mesh->GetViewList();
                views.append(_name);
                mesh->SetViewList(views);

                SetBoundingDistance();
                ui->cb_BoundingType->setCurrentIndex(0);
            }
        }
        file_name_STLs.clear();
    }
}

void MainWindow::DefineSimpleSurface()
{
    QString surfaceName = ui->txt_GeometrySurfaceName->text();
    if(!CheckNameValid(surfaceName))
        return;
    //Save value for each type
    Snappy_Dmesh *snappy = mesh->snappyd;
    if(ui->cb_SurfaceType->currentText() == "Cylinder")
    {
       //define cylinder type
        if(ui->txt_P1_Cyl_Sur_X->text()=="" || ui->txt_P1_Cyl_Sur_Y->text()=="" || ui->txt_P1_Cyl_Sur_Z->text()=="" || ui->txt_P2_Cyl_Sur_X->text()==""
           ||ui->txt_P2_Cyl_Sur_Y->text()=="" || ui->txt_P2_Cyl_Sur_Z->text()=="" || ui->txt_Radius_Cyl_Sur->text()=="")
        {
            QMessageBox::information(this,tr("Error"),tr("Values are incorrect!"));
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
                QMessageBox::information(this,tr("Error"),tr("Values are incorrect!"));
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

            //snappy->FindMinMaxDefaultBounding(minx, miny, minz);
            //snappy->FindMinMaxDefaultBounding(maxx, maxy, maxz);

            snappy->min_Max.min.x = minx;
            snappy->min_Max.min.y = miny;
            snappy->min_Max.min.z = minz;

            snappy->min_Max.max.x = maxx;
            snappy->min_Max.max.y = maxy;
            snappy->min_Max.max.z = maxz;

            //add min_Max temp to list min max;
            snappy->min_Max.name_Surface = surfaceName;
            snappy->list_Surface_Min_Max.append(snappy->min_Max);
            snappy->min_Max.name_Surface="";
            snappy->FindMinMax(snappy->list_Surface_Min_Max);

            //add geometry values to mesh
            snappy->gCylin.n=snappy->gCylin.n+1;
            snappy->gCylin.cylins.resize(snappy->gCylin.n);
            snappy->gCylin.cylins.last().name=surfaceName;
            snappy->gCylin.cylins.last().type = "searchableCylinder";
            snappy->gCylin.cylins.last().point2.x=x2;
            snappy->gCylin.cylins.last().point2.y=y2;
            snappy->gCylin.cylins.last().point2.z=z2;
            snappy->gCylin.cylins.last().point1.x=x1;
            snappy->gCylin.cylins.last().point1.y=y1;
            snappy->gCylin.cylins.last().point1.z=z1;
            snappy->gCylin.cylins.last().radius= radius_cyl;

            //add surface and region default
            snappy->gCylin.refi_Sur.n =snappy->gCylin.refi_Sur.n+1;
            snappy->gCylin.refi_Sur.surfaces.resize(snappy->gCylin.refi_Sur.n);
            snappy->gCylin.refi_Sur.surfaces.last().lv1= 0;
            snappy->gCylin.refi_Sur.surfaces.last().lv2= 0;
            snappy->gCylin.refi_Sur.surfaces.last().name=surfaceName;

            snappy->gCylin.refi_Reg.n = snappy->gCylin.refi_Reg.n+1;
            snappy->gCylin.refi_Reg.region.resize(snappy->gCylin.refi_Reg.n);
            snappy->gCylin.refi_Reg.region.last().name = surfaceName;
            snappy->gCylin.refi_Reg.region.last().mode = "distance";
            snappy->gCylin.refi_Reg.region.last().lv1 = 0;
            snappy->gCylin.refi_Reg.region.last().lv2 = 0;
            snappy->gCylin.refi_Reg.region.last().n = 0;

            AddFaceToList(surfaceName,1);
            listSurfaces.append(surfaceName);

            QStringList views = mesh->GetViewList();
            views.append(surfaceName);
            mesh->SetViewList(views);
            SetBoundingDistance();
//            snappy->facename.append(surfaceName);
            mesh->updateGL();
            ui->txt_Log->append("Defining "+ surfaceName +" cylinder has been done");
        }
    }
    else if(ui->cb_SurfaceType->currentText() == "Box")
    {
       //define box type
        if(ui->txt_Max_Box_Sur_X->text()=="" || ui->txt_Max_Box_Sur_Y->text()=="" || ui->txt_Max_Box_Sur_Z->text()=="" ||
           ui->txt_Min_Box_Sur_X->text()=="" || ui->txt_Min_Box_Sur_Y->text()=="" || ui->txt_Min_Box_Sur_Z->text()=="")
        {
            QMessageBox::information(this,tr("Error"),tr("Values are incorrect!"));
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
                QMessageBox::information(this,tr("Error"),tr("Values are incorrect!"));
                return;
            }
            //find min max default bouding
            //snappy->FindMinMaxDefaultBounding(x1, y1, z1);
            //snappy->FindMinMaxDefaultBounding(x2, y2, z2);

            snappy->min_Max.min.x = x1;
            snappy->min_Max.min.y = y1;
            snappy->min_Max.min.z = z1;

            snappy->min_Max.max.x = x2;
            snappy->min_Max.max.y = y2;
            snappy->min_Max.max.z = z2;

            //add min_Max temp to list min max;
            snappy->min_Max.name_Surface = surfaceName;
            snappy->list_Surface_Min_Max.append(snappy->min_Max);
            snappy->min_Max.name_Surface="";

            snappy->FindMinMax(snappy->list_Surface_Min_Max);

            snappy->gBox.n=snappy->gBox.n+1;
            snappy->gBox.boxes.resize(snappy->gBox.n);
            snappy->gBox.boxes.last().name=surfaceName;
            snappy->gBox.boxes.last().type="searchableBox";
            snappy->gBox.boxes.last().max.x=x2;
            snappy->gBox.boxes.last().max.y=y2;
            snappy->gBox.boxes.last().max.z=z2;
            snappy->gBox.boxes.last().min.x=x1;
            snappy->gBox.boxes.last().min.y=y1;
            snappy->gBox.boxes.last().min.z=z1;

            //add surface and region default
            snappy->gBox.refi_Sur.n =snappy->gBox.refi_Sur.n+1;
            snappy->gBox.refi_Sur.surfaces.resize(snappy->gBox.refi_Sur.n);
            snappy->gBox.refi_Sur.surfaces.last().lv1= 0;
            snappy->gBox.refi_Sur.surfaces.last().lv2= 0;
            snappy->gBox.refi_Sur.surfaces.last().name=surfaceName;

            snappy->gBox.refi_Reg.n = snappy->gBox.refi_Reg.n+1;
            snappy->gBox.refi_Reg.region.resize(snappy->gBox.refi_Reg.n);
            snappy->gBox.refi_Reg.region.last().name = surfaceName;
            snappy->gBox.refi_Reg.region.last().mode = "distance";
            snappy->gBox.refi_Reg.region.last().lv1 = 0;
            snappy->gBox.refi_Reg.region.last().lv2 = 0;
            snappy->gBox.refi_Reg.region.last().n = 0;

            //draw box

            int plus = snappy->points.size();
            snappy->points.resize(8+plus);
            snappy->points[0+plus] = new  float[3];
            snappy->points[0+plus][0] = x1;
            snappy->points[0+plus][1] = y1;
            snappy->points[0+plus][2] = z1;


            snappy->points[1+plus] = new  float[3];
            snappy->points[1+plus][0] = x2;
            snappy->points[1+plus][1] = y1;
            snappy->points[1+plus][2] = z1;

            snappy->points[2+plus] = new  float[3];
            snappy->points[2+plus][0] = x2;
            snappy->points[2+plus][1] = y2;
            snappy->points[2+plus][2] = z1;

            snappy->points[3+plus] = new  float[3];
            snappy->points[3+plus][0] = x1;
            snappy->points[3+plus][1] = y2;
            snappy->points[3+plus][2] = z1;

            snappy->points[4+plus] = new  float[3];
            snappy->points[4+plus][0] = x1;
            snappy->points[4+plus][1] = y1;
            snappy->points[4+plus][2] = z2;

            snappy->points[5+plus] = new  float[3];
            snappy->points[5+plus][0] = x2;
            snappy->points[5+plus][1] = y1;
            snappy->points[5+plus][2] = z2;

            snappy->points[6+plus] = new  float[3];
            snappy->points[6+plus][0] = x2;
            snappy->points[6+plus][1] = y2;
            snappy->points[6+plus][2] = z2;

            snappy->points[7+plus] = new  float[3];
            snappy->points[7+plus][0] = x1;
            snappy->points[7+plus][1] = y2;
            snappy->points[7+plus][2] = z2;

            int starend[2];
            starend[0] = snappy->faces.size();

            snappy->faces.resize(6+starend[0]);
            snappy->faces[0+starend[0]] = new int[4];
            snappy->faces[0+starend[0]][0] = 3 + plus;
            snappy->faces[0+starend[0]][1] = 7 + plus;
            snappy->faces[0+starend[0]][2] = 6 + plus;
            snappy->faces[0+starend[0]][3] = 2 + plus;

            snappy->faces[1+starend[0]] = new int[4];
            snappy->faces[1+starend[0]][0] = 1 + plus;
            snappy->faces[1+starend[0]][1] = 5 + plus;
            snappy->faces[1+starend[0]][2] = 4 + plus;
            snappy->faces[1+starend[0]][3] = 0 + plus;

            snappy->faces[2+starend[0]] = new int[4];
            snappy->faces[2+starend[0]][0] = 4 + plus;
            snappy->faces[2+starend[0]][1] = 5 + plus;
            snappy->faces[2+starend[0]][2] = 6 + plus;
            snappy->faces[2+starend[0]][3] = 7 + plus;

            snappy->faces[3+starend[0]] = new int[4];
            snappy->faces[3+starend[0]][0] = 0 + plus;
            snappy->faces[3+starend[0]][1] = 3 + plus;
            snappy->faces[3+starend[0]][2] = 2 + plus;
            snappy->faces[3+starend[0]][3] = 1 + plus;

            snappy->faces[4+starend[0]] = new int[4];
            snappy->faces[4+starend[0]][0] = 0 + plus;
            snappy->faces[4+starend[0]][1] = 4 + plus;
            snappy->faces[4+starend[0]][2] = 7 + plus;
            snappy->faces[4+starend[0]][3] = 3 + plus;

            snappy->faces[5+starend[0]] = new int[4];
            snappy->faces[5+starend[0]][0] = 2 + plus;
            snappy->faces[5+starend[0]][1] = 6 + plus;
            snappy->faces[5+starend[0]][2] = 5 + plus;
            snappy->faces[5+starend[0]][3] = 1 + plus;

            starend[1] = snappy->faces.size();

            snappy->facezones.resize( snappy->facezones.size()+1);
            snappy->facezones[snappy->facezones.size()-1]= new int[2];
            snappy->facezones[snappy->facezones.size()-1][0]= starend[0];
            snappy->facezones[snappy->facezones.size()-1][1]= starend[1];

            snappy->facetype.resize(snappy->facetype.size() + 1);
            snappy->facetype[snappy->facetype.size() - 1] = 1;

            snappy->facename.append(surfaceName);

            QStringList views = mesh->GetViewList();
            views.append(surfaceName);
            mesh->SetViewList(views);

            SetBoundingDistance();

            AddFaceToList(surfaceName,1);
            listSurfaces.append(surfaceName);
            ui->txt_Log->append("Successfully create "+ surfaceName +" box");
        }

    }
    else if(ui->cb_SurfaceType->currentText() == "Sphere")
    {
        //define cylinder type
         if(ui->txt_P_Sphe_Sur_X->text()=="" || ui->txt_P_Sphe_Sur_Y->text()=="" || ui->txt_P_Sphe_Sur_Z->text()=="" || ui->txt_Radius_Sphe_Sur->text()=="")
         {
             QMessageBox::information(this,tr("Error"),tr("Values are incorrect!"));
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
                 QMessageBox::information(this,tr("Error"),tr("Values are incorrect!"));
                 return;
             }
             //find min max default bouding
             //snappy->FindMinMaxDefaultBounding(x + radius_sph, y + radius_sph, z + radius_sph);
             //snappy->FindMinMaxDefaultBounding(x - radius_sph, y - radius_sph, z - radius_sph);
             snappy->min_Max.min.x = x + radius_sph;
             snappy->min_Max.min.y = y + radius_sph;
             snappy->min_Max.min.z = z + radius_sph;

             snappy->min_Max.max.x = x - radius_sph;
             snappy->min_Max.max.y = y - radius_sph;
             snappy->min_Max.max.z = z - radius_sph;

             //add min_Max temp to list min max;
             snappy->min_Max.name_Surface = surfaceName;
             snappy->list_Surface_Min_Max.append(snappy->min_Max);
             snappy->min_Max.name_Surface = "";
             snappy->FindMinMax(snappy->list_Surface_Min_Max);

             //add geometry values to mesh
             snappy->gSphere.n = snappy->gSphere.n + 1;
             snappy->gSphere.sphere.resize(snappy->gSphere.n);
             snappy->gSphere.sphere.last().name = surfaceName;
             snappy->gSphere.sphere.last().type = "searchableSphere";
             snappy->gSphere.sphere.last().centre.x = x;
             snappy->gSphere.sphere.last().centre.y = y;
             snappy->gSphere.sphere.last().centre.z = z;
             snappy->gSphere.sphere.last().radius = radius_sph;

             //add surface and region default
             snappy->gSphere.refi_Sur.n =snappy->gSphere.refi_Sur.n+1;
             snappy->gSphere.refi_Sur.surfaces.resize(snappy->gSphere.refi_Sur.n);
             snappy->gSphere.refi_Sur.surfaces.last().lv1= 0;
             snappy->gSphere.refi_Sur.surfaces.last().lv2= 0;
             snappy->gSphere.refi_Sur.surfaces.last().name=surfaceName;

             snappy->gSphere.refi_Reg.n = snappy->gSphere.refi_Reg.n+1;
             snappy->gSphere.refi_Reg.region.resize(snappy->gSphere.refi_Reg.n);
             snappy->gSphere.refi_Reg.region.last().name = surfaceName;
             snappy->gSphere.refi_Reg.region.last().mode = "distance";
             snappy->gSphere.refi_Reg.region.last().lv1 = 0;
             snappy->gSphere.refi_Reg.region.last().lv2 = 0;
             snappy->gSphere.refi_Reg.region.last().n = 0;

             AddFaceToList(surfaceName,1);
             listSurfaces.append(surfaceName);

             QStringList views = mesh->GetViewList();
             views.append(surfaceName);
             mesh->SetViewList(views);

             SetBoundingDistance();
 //            snappy->facename.append(surfaceName);
             mesh->updateGL();
             ui->txt_Log->append("Defining "+ surfaceName +" sphere has been done");
         }
    }
    ui->cb_BoundingType->setCurrentIndex(0);
}

void MainWindow::DefineSimpleCellZone()
{
    QString surfaceName = ui->txt_GeometrySurfaceName->text();
    if(!CheckNameValid(surfaceName))
        return;
    //Save value for each type
    Snappy_Dmesh *snappy = mesh->snappyd;
    if(ui->cb_SurfaceType->currentText() == "Cylinder")
    {
       //define cylinder type
        if(ui->txt_P1_Cyl_Sur_X->text()=="" || ui->txt_P1_Cyl_Sur_Y->text()=="" || ui->txt_P1_Cyl_Sur_Z->text()=="" || ui->txt_P2_Cyl_Sur_X->text()==""
           ||ui->txt_P2_Cyl_Sur_Y->text()=="" || ui->txt_P2_Cyl_Sur_Z->text()=="" || ui->txt_Radius_Cyl_Sur->text()=="")
        {
            QMessageBox::information(this,tr("Error"),tr("Values are incorrect!"));
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
                QMessageBox::information(this,tr("Error"),tr("Values are incorrect!"));
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

            //add min_Max temp to list min max;
            snappy->min_Max.name_Surface = surfaceName;
            snappy->min_Max.min.x = minx;
            snappy->min_Max.min.y = miny;
            snappy->min_Max.min.z = minz;

            snappy->min_Max.max.x = maxx;
            snappy->min_Max.max.y = maxy;
            snappy->min_Max.max.z = maxz;
            snappy->list_Surface_Min_Max.append(snappy->min_Max);
            snappy->min_Max.name_Surface="";
            snappy->FindMinMax(snappy->list_Surface_Min_Max);

            //add geometry values to mesh
            GeomeCylinderSurface *gCylin = &snappy->gCylinCellZone;
            gCylin->n=gCylin->n+1;
            gCylin->cylins.resize(gCylin->n);
            gCylin->cylins.last().name=surfaceName;
            gCylin->cylins.last().type = "searchableCylinder";
            gCylin->cylins.last().point2.x=x2;
            gCylin->cylins.last().point2.y=y2;
            gCylin->cylins.last().point2.z=z2;
            gCylin->cylins.last().point1.x=x1;
            gCylin->cylins.last().point1.y=y1;
            gCylin->cylins.last().point1.z=z1;
            gCylin->cylins.last().radius= radius_cyl;

            //add surface and region default
            gCylin->refi_Sur.n =gCylin->refi_Sur.n+1;
            gCylin->refi_Sur.surfaces.resize(gCylin->refi_Sur.n);
            gCylin->refi_Sur.surfaces.last().lv1= 0;
            gCylin->refi_Sur.surfaces.last().lv2= 0;
            gCylin->refi_Sur.surfaces.last().name=surfaceName;

            gCylin->refi_Reg.n = gCylin->refi_Reg.n+1;
            gCylin->refi_Reg.region.resize(gCylin->refi_Reg.n);
            gCylin->refi_Reg.region.last().name = surfaceName;
            gCylin->refi_Reg.region.last().mode = "inside";
            gCylin->refi_Reg.region.last().lv1 = 0;
            gCylin->refi_Reg.region.last().lv2 = 0;
            gCylin->refi_Reg.region.last().n = 0;

            AddFaceToList(surfaceName,3);

            QStringList views = mesh->GetViewList();
            views.append(surfaceName);
            mesh->SetViewList(views);

            SetBoundingDistance();
            mesh->updateGL();
            ui->txt_Log->append("Defining "+ surfaceName +" cylinder cell zone has been done");
        }
    }
    else if(ui->cb_SurfaceType->currentText() == "Box")
    {
       //define box type
        if(ui->txt_Max_Box_Sur_X->text()=="" || ui->txt_Max_Box_Sur_Y->text()=="" || ui->txt_Max_Box_Sur_Z->text()=="" ||
           ui->txt_Min_Box_Sur_X->text()=="" || ui->txt_Min_Box_Sur_Y->text()=="" || ui->txt_Min_Box_Sur_Z->text()=="")
        {
            QMessageBox::information(this,tr("Error"),tr("Values are incorrect!"));
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
                QMessageBox::information(this,tr("Error"),tr("Values are incorrect!"));
                return;
            }

            //add min_Max temp to list min max;
            snappy->min_Max.name_Surface = surfaceName;

            snappy->min_Max.min.x = x1;
            snappy->min_Max.min.y = y1;
            snappy->min_Max.min.z = z1;

            snappy->min_Max.max.x = x2;
            snappy->min_Max.max.y = y2;
            snappy->min_Max.max.z = z2;

            snappy->list_Surface_Min_Max.append(snappy->min_Max);
            snappy->min_Max.name_Surface="";
            snappy->FindMinMax(snappy->list_Surface_Min_Max);

            //add geometry values to mesh
            GeomeBoxSurface *gBox = &snappy->gBoxCellZone;
            gBox->n=gBox->n+1;
            gBox->boxes.resize(gBox->n);
            gBox->boxes.last().name=surfaceName;
            gBox->boxes.last().type="searchableBox";
            gBox->boxes.last().max.x=x2;
            gBox->boxes.last().max.y=y2;
            gBox->boxes.last().max.z=z2;
            gBox->boxes.last().min.x=x1;
            gBox->boxes.last().min.y=y1;
            gBox->boxes.last().min.z=z1;

            //add surface and region default
            gBox->refi_Sur.n = gBox->refi_Sur.n + 1;
            gBox->refi_Sur.surfaces.resize(gBox->refi_Sur.n);
            gBox->refi_Sur.surfaces.last().name = surfaceName;
            gBox->refi_Sur.surfaces.last().lv1 = 0;
            gBox->refi_Sur.surfaces.last().lv2 = 0;

            gBox->refi_Reg.n = gBox->refi_Reg.n + 1;
            gBox->refi_Reg.region.resize(gBox->refi_Reg.n);
            gBox->refi_Reg.region.last().name = surfaceName;
            gBox->refi_Reg.region.last().mode = "inside";
            gBox->refi_Reg.region.last().lv1 = 0;
            gBox->refi_Reg.region.last().lv2 = 0;
            gBox->refi_Reg.region.last().n = 0;


            //draw box

            int plus = snappy->points.size();
            snappy->points.resize(8+plus);
            snappy->points[0+plus] = new  float[3];
            snappy->points[0+plus][0] = x1;
            snappy->points[0+plus][1] = y1;
            snappy->points[0+plus][2] = z1;


            snappy->points[1+plus] = new  float[3];
            snappy->points[1+plus][0] = x2;
            snappy->points[1+plus][1] = y1;
            snappy->points[1+plus][2] = z1;

            snappy->points[2+plus] = new  float[3];
            snappy->points[2+plus][0] = x2;
            snappy->points[2+plus][1] = y2;
            snappy->points[2+plus][2] = z1;

            snappy->points[3+plus] = new  float[3];
            snappy->points[3+plus][0] = x1;
            snappy->points[3+plus][1] = y2;
            snappy->points[3+plus][2] = z1;

            snappy->points[4+plus] = new  float[3];
            snappy->points[4+plus][0] = x1;
            snappy->points[4+plus][1] = y1;
            snappy->points[4+plus][2] = z2;

            snappy->points[5+plus] = new  float[3];
            snappy->points[5+plus][0] = x2;
            snappy->points[5+plus][1] = y1;
            snappy->points[5+plus][2] = z2;

            snappy->points[6+plus] = new  float[3];
            snappy->points[6+plus][0] = x2;
            snappy->points[6+plus][1] = y2;
            snappy->points[6+plus][2] = z2;

            snappy->points[7+plus] = new  float[3];
            snappy->points[7+plus][0] = x1;
            snappy->points[7+plus][1] = y2;
            snappy->points[7+plus][2] = z2;

            int starend[2];
            starend[0] = snappy->faces.size();

            snappy->faces.resize(6+starend[0]);
            snappy->faces[0+starend[0]] = new int[4];
            snappy->faces[0+starend[0]][0] = 3 + plus;
            snappy->faces[0+starend[0]][1] = 7 + plus;
            snappy->faces[0+starend[0]][2] = 6 + plus;
            snappy->faces[0+starend[0]][3] = 2 + plus;

            snappy->faces[1+starend[0]] = new int[4];
            snappy->faces[1+starend[0]][0] = 1 + plus;
            snappy->faces[1+starend[0]][1] = 5 + plus;
            snappy->faces[1+starend[0]][2] = 4 + plus;
            snappy->faces[1+starend[0]][3] = 0 + plus;

            snappy->faces[2+starend[0]] = new int[4];
            snappy->faces[2+starend[0]][0] = 4 + plus;
            snappy->faces[2+starend[0]][1] = 5 + plus;
            snappy->faces[2+starend[0]][2] = 6 + plus;
            snappy->faces[2+starend[0]][3] = 7 + plus;

            snappy->faces[3+starend[0]] = new int[4];
            snappy->faces[3+starend[0]][0] = 0 + plus;
            snappy->faces[3+starend[0]][1] = 3 + plus;
            snappy->faces[3+starend[0]][2] = 2 + plus;
            snappy->faces[3+starend[0]][3] = 1 + plus;

            snappy->faces[4+starend[0]] = new int[4];
            snappy->faces[4+starend[0]][0] = 0 + plus;
            snappy->faces[4+starend[0]][1] = 4 + plus;
            snappy->faces[4+starend[0]][2] = 7 + plus;
            snappy->faces[4+starend[0]][3] = 3 + plus;

            snappy->faces[5+starend[0]] = new int[4];
            snappy->faces[5+starend[0]][0] = 2 + plus;
            snappy->faces[5+starend[0]][1] = 6 + plus;
            snappy->faces[5+starend[0]][2] = 5 + plus;
            snappy->faces[5+starend[0]][3] = 1 + plus;

            starend[1] = snappy->faces.size();

            snappy->facezones.resize( snappy->facezones.size()+1);
            snappy->facezones[snappy->facezones.size()-1]= new int[2];
            snappy->facezones[snappy->facezones.size()-1][0]= starend[0];
            snappy->facezones[snappy->facezones.size()-1][1]= starend[1];

            snappy->facetype.resize(snappy->facetype.size() + 1);
            snappy->facetype[snappy->facetype.size() - 1] = 0;

            snappy->facename.append(surfaceName);

            QStringList views = mesh->GetViewList();
            views.append(surfaceName);
            mesh->SetViewList(views);

            SetBoundingDistance();

            AddFaceToList(surfaceName,3);
            ui->txt_Log->append("Successfully create "+ surfaceName +" box cell zone");
        }

    }
    else if(ui->cb_SurfaceType->currentText() == "Sphere")
    {
        //define cylinder type
         if(ui->txt_P_Sphe_Sur_X->text()=="" || ui->txt_P_Sphe_Sur_Y->text()=="" || ui->txt_P_Sphe_Sur_Z->text()=="" || ui->txt_Radius_Sphe_Sur->text()=="")
         {
             QMessageBox::information(this,tr("Error"),tr("Values are incorrect!"));
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
                 QMessageBox::information(this,tr("Error"),tr("Values are incorrect!"));
                 return;
             }
             //find min max default bouding
             //snappy->FindMinMaxDefaultBounding(x + radius_sph, y + radius_sph, z + radius_sph);
             //snappy->FindMinMaxDefaultBounding(x - radius_sph, y - radius_sph, z - radius_sph);

             //add min_Max temp to list min max;
             snappy->min_Max.name_Surface = surfaceName;
             snappy->min_Max.min.x = x + radius_sph;
             snappy->min_Max.min.y = y + radius_sph;
             snappy->min_Max.min.z = z + radius_sph;

             snappy->min_Max.max.x = x - radius_sph;
             snappy->min_Max.max.y = y - radius_sph;
             snappy->min_Max.max.z = z - radius_sph;

             snappy->list_Surface_Min_Max.append(snappy->min_Max);
             snappy->min_Max.name_Surface = "";
             snappy->FindMinMax(snappy->list_Surface_Min_Max);

             //add geometry values to mesh

             GeomeSphereSurface *gSphere = &snappy->gSphereCellZone;
             gSphere->n = gSphere->n + 1;
             gSphere->sphere.resize(gSphere->n);
             gSphere->sphere[gSphere->n-1].name = surfaceName;
             gSphere->sphere[gSphere->n-1].type = "searchableSphere";
             gSphere->sphere[gSphere->n-1].centre.x = x;
             gSphere->sphere[gSphere->n-1].centre.y = y;
             gSphere->sphere[gSphere->n-1].centre.z = z;
             gSphere->sphere[gSphere->n-1].radius = radius_sph;

             //add surface and region default
             gSphere->refi_Sur.n =gSphere->refi_Sur.n+1;
             gSphere->refi_Sur.surfaces.resize(gSphere->refi_Sur.n);
             gSphere->refi_Sur.surfaces.last().lv1= 0;
             gSphere->refi_Sur.surfaces.last().lv2= 0;
             gSphere->refi_Sur.surfaces.last().name=surfaceName;

             gSphere->refi_Reg.n = gSphere->refi_Reg.n+1;
             gSphere->refi_Reg.region.resize(gSphere->refi_Reg.n);
             gSphere->refi_Reg.region.last().name = surfaceName;
             gSphere->refi_Reg.region.last().mode = "inside";
             gSphere->refi_Reg.region.last().lv1 = 0;
             gSphere->refi_Reg.region.last().lv2 = 0;
             gSphere->refi_Reg.region.last().n = 0;

             AddFaceToList(surfaceName,3);

             QStringList views = mesh->GetViewList();
             views.append(surfaceName);
             mesh->SetViewList(views);

             SetBoundingDistance();
             mesh->updateGL();
             ui->txt_Log->append("Defining "+ surfaceName +" sphere cell zone has been done");
         }
    }
    ui->cb_BoundingType->setCurrentIndex(0);
}

void MainWindow::DefineSimpleVolume()
{
    QString volumeName = ui->txt_GeometrySurfaceName->text();
    if(!CheckNameValid(volumeName))
        return;
    //Save value for each type
    if(ui->cb_SurfaceType->currentText() == "Box")
    {
       //define box type
        if(ui->txt_Max_Box_Sur_X->text()=="" || ui->txt_Max_Box_Sur_Y->text()=="" || ui->txt_Max_Box_Sur_Z->text()=="" ||
           ui->txt_Min_Box_Sur_X->text()=="" || ui->txt_Min_Box_Sur_Y->text()=="" || ui->txt_Min_Box_Sur_Z->text()=="")
        {
            QMessageBox::information(this,tr("Error"),tr("Values are incorrect!"));
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
                QMessageBox::information(this,tr("Error"),tr("Values are incorrect!"));
                return;
            }

            GeomeBoxRegion *gBoxRegion = &mesh->snappyd->gBoxRegion;
            int index = gBoxRegion->n;
            gBoxRegion->n = index + 1;
            gBoxRegion->boxes.resize(gBoxRegion->n);
            gBoxRegion->boxes[index].name = volumeName;
            gBoxRegion->boxes[index].type = "searchableBox";
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
            refi_Reg->region[index].mode = "inside";
            refi_Reg->region[index].lv1 = 0;
            refi_Reg->region[index].lv2 = 0;
            refi_Reg->region[index].n = 0;


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
            mesh->snappyd->facetype[mesh->snappyd->facetype.size() - 1] = 2;

            mesh->snappyd->facename.append(volumeName);

            QStringList views = mesh->GetViewList();
            views.append(volumeName);
            mesh->SetViewList(views);

            AddFaceToList(volumeName,2);

            mesh->updateGL();
            ui->txt_Log->append("Defining of "+ volumeName +" box has been done");
        }

    }
    else if(ui->cb_SurfaceType->currentText() == "Cylinder")
    {
       //define cylinder type
        if(ui->txt_P1_Cyl_Sur_X->text()=="" || ui->txt_P1_Cyl_Sur_Y->text()=="" || ui->txt_P1_Cyl_Sur_Z->text()=="" || ui->txt_P2_Cyl_Sur_X->text()==""
           ||ui->txt_P2_Cyl_Sur_Y->text()=="" || ui->txt_P2_Cyl_Sur_Z->text()=="" || ui->txt_Radius_Cyl_Sur->text()=="")
        {
            QMessageBox::information(this,tr("Error"),tr("Values are invalid!"));
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
                QMessageBox::information(this,tr("Error"),tr("Values are invalid!"));
                return;
            }
            GeomeCylinderRegion *gCylinRegion = &mesh->snappyd->gCylinRegion;

            int index = gCylinRegion->n;
            gCylinRegion->n = index + 1;
            gCylinRegion->cylins.resize(gCylinRegion->n);
            gCylinRegion->cylins[index].name=volumeName;
            gCylinRegion->cylins[index].type = "searchableCylinder";
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
            refi_Reg->region[index].mode = "inside";
            refi_Reg->region[index].lv1 = 0;
            refi_Reg->region[index].lv2 = 0;
            refi_Reg->region[index].n = 0;

            AddFaceToList(volumeName,2);

            QStringList views = mesh->GetViewList();
            views.append(volumeName);
            mesh->SetViewList(views);

            mesh->updateGL();
            ui->txt_Log->append("Defining  of "+ volumeName +" cylinder has been done");
        }

    }
    else if(ui->cb_SurfaceType->currentText() == "Sphere")
    {
       //define sphere type
        if(ui->txt_P_Sphe_Sur_X->text()=="" || ui->txt_P_Sphe_Sur_Y->text()=="" || ui->txt_P_Sphe_Sur_Z->text()=="" || ui->txt_Radius_Sphe_Sur->text()=="")
        {
            QMessageBox::information(this,tr("Error"),tr("Values are invalid!"));
            return;
        }
        else
        {
            bool a,b,c,d;
            float x = ui->txt_P_Sphe_Sur_X->text().toFloat(&a);
            float y = ui->txt_P_Sphe_Sur_Y->text().toFloat(&b);
            float z = ui->txt_P_Sphe_Sur_Z->text().toFloat(&c);

            float radius_sph = ui->txt_Radius_Sphe_Sur->text().toFloat(&d);
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
            gSphereRegion->sphere[index].type = "searchableSphere";
            gSphereRegion->sphere[index].centre.x=x;
            gSphereRegion->sphere[index].centre.y=y;
            gSphereRegion->sphere[index].centre.z=z;
            gSphereRegion->sphere[index].radius= radius_sph;

            RefinementRegions *refi_Reg = &gSphereRegion->refi_Reg;
            index = refi_Reg->n;
            refi_Reg->n = index + 1;
            refi_Reg->region.resize(refi_Reg->n);
            refi_Reg->region[index].name = volumeName;
            refi_Reg->region[index].mode = "inside";
            refi_Reg->region[index].lv1 = 0;
            refi_Reg->region[index].lv2 = 0;
            refi_Reg->region[index].n = 0;

            AddFaceToList(volumeName,2);

            QStringList views = mesh->GetViewList();
            views.append(volumeName);
            mesh->SetViewList(views);

            mesh->updateGL();
            ui->txt_Log->append("Defining of "+ volumeName +" sphere has been done");
        }
    }
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
            ui->txt_Log->append("Settings for bounding distance have been save");
    }
    else
    {
        if(CheckAndSaveBoundingBoxInput())
            ui->txt_Log->append("Values of bounding have been save");
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
        QMessageBox::critical(this,tr("Error"),tr("Please input a valid value!"));
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
        QMessageBox::critical(this,tr("Error"),tr("Please input a valid value!"));
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
        QMessageBox::critical(this,tr("Error"),tr("Please input a valid value!"));
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
    mesh->snappyd->deltaBaseMesh.x = ui->txt_DeltaX->text().toFloat();
    mesh->snappyd->deltaBaseMesh.y = ui->txt_DeltaY->text().toFloat();
    mesh->snappyd->deltaBaseMesh.z = ui->txt_DeltaZ->text().toFloat();
    ui->txt_Log->append("Settings for base mesh have been save");
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

    if(!mesh->blockd->isAutomatic){
        //add list faces
        for(int i =0; i< mesh->blockd->boundMesh.n; i++)
        {
            AddFaceToList(mesh->blockd->boundMesh.bounsType[i].name);
        }
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
    for(int i=0; i< mesh->snappyd->gBoxCellZone.n; i++)
    {
        AddFaceToList(mesh->snappyd->gBoxCellZone.boxes[i].name);
    }
    for(int i=0; i< mesh->snappyd->gCylinCellZone.n; i++)
    {
        AddFaceToList(mesh->snappyd->gCylinCellZone.cylins[i].name);
    }
    for(int i=0; i< mesh->snappyd->gSphereCellZone.n; i++)
    {
        AddFaceToList(mesh->snappyd->gSphereCellZone.sphere[i].name);
    }
    for(int i=0; i< mesh->snappyd->gUserDefineCellZone.n; i++)
    {
        AddFaceToList(mesh->snappyd->gUserDefineCellZone.user_Defines[i].name);
    }
    ui->cb_MeshVolumeMode->setCurrentIndex(0);
}

void MainWindow::on_btn_MeshRefineSurface_clicked()
{
    if(ui->tb_boundary->currentRow() == -1 || flag_Item_Face_Click ==false){
        QMessageBox::warning(this,"Error","Please select a surface!");
        return;
    }
    bool a,b;
    int min = ui->txt_Level_Min_Surface_Refine->text().toInt(&a);
    int max = ui->txt_Level_Max_Surface_Refine->text().toInt(&b);

    if(a== false || b==false || min > max)
    {
        QMessageBox::critical(this,tr("Error"),tr("Please input all values in this form...!"));
        return;
    }    
    foreach (QTableWidgetItem *item, ui->tb_boundary->selectedItems()) {
        if(AddUserDefine(item->text(),min,max) || AddSurfaceRegionBox(item->text(),min,max))
            ui->txt_Log->append("Defining of "+ item->text() +" surface has been done");
    }
}


bool MainWindow::AddRefineVolume(RefinementRegions *refi_Reg, QString currentSurface,QString mode, float lv)
{
    for(int i = 0; i< refi_Reg->n; i++)
    {
        if(refi_Reg->region[i].name == currentSurface)
        {
            refi_Reg->region[i].mode = mode;
            refi_Reg->region[i].lv2 = lv;
            ui->txt_Log->append("Settings of refining volume for " + currentSurface + " cell zone have been done");
            return true;
        }
    }
    return false;
}

void MainWindow::on_btn_MeshRefineVolume_clicked()
{
    if(ui->tb_boundary->currentRow() == -1)
        return;
    bool a;
    float lv = ui->txt_Level_Volume->text().toFloat(&a);
    QString currentSurface = ui->tb_boundary->currentItem()->text();
    QString mode = ui->cb_MeshVolumeMode->currentText();
    if(AddRefineVolume(&mesh->snappyd->gBoxCellZone.refi_Reg,currentSurface,mode,lv))
        return;
    if(AddRefineVolume(&mesh->snappyd->gCylinCellZone.refi_Reg,currentSurface,mode,lv))
        return;
    if(AddRefineVolume(&mesh->snappyd->gSphereCellZone.refi_Reg,currentSurface,mode,lv))
        return;
    if(AddRefineVolume(&mesh->snappyd->gUserDefineCellZone.refi_Reg,currentSurface,mode,lv))
        return;
    if(AddRefineVolume(&mesh->snappyd->gBoxRegion.refi_Reg,currentSurface,mode,lv))
        return;
    if(AddRefineVolume(&mesh->snappyd->gCylinRegion.refi_Reg,currentSurface,mode,lv))
        return;
    if(AddRefineVolume(&mesh->snappyd->gSphereRegion.refi_Reg,currentSurface,mode,lv))
        return;
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
//        AddFaceToList(mesh->snappyd->gUserDefine.user_Defines[i].name);
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
        QMessageBox::information(this,tr("Error"),tr("Values are incorrect!"));
        return;
    }
    foreach(QTableWidgetItem *item,ui->tb_boundary->selectedItems()) {
        int n = mesh->snappyd->add_Layers_Controls.nLayer;
        //if exists, update
        int i;
        for(i=0;i< n; i++)
        {
            if(mesh->snappyd->add_Layers_Controls.layers[i].name ==item->text())
            {
                mesh->snappyd->add_Layers_Controls.layers[i].nSurfaceLayers =layer;
                ui->txt_Log->append("Settings for layer of " + item->text() + " have been done");
                break;
            }
        }
        if(i == n) {
            //add new
            mesh->snappyd->add_Layers_Controls.nLayer = n + 1;
            mesh->snappyd->add_Layers_Controls.layers.resize(n+1);
            mesh->snappyd->add_Layers_Controls.layers[n].name = item->text();
            mesh->snappyd->add_Layers_Controls.layers[n].nSurfaceLayers = layer;
            ui->txt_Log->append("Settings for layer of " + item->text() + " have been done");
        }
    }
}

void MainWindow::on_btn_MeshSurfaceGeneral_clicked()
{
    W_MeshSurfaceRefineGeneral *w_general;
    w_general = new W_MeshSurfaceRefineGeneral(mesh);
    w_general->exec();
    if(w_general->result())
        ui->txt_Log->append("Settings for general value of layers have been done");
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
    if(ui->btn_MeshSurface->isEnabled())
        on_btn_MeshSurface_clicked();
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
            QMessageBox::critical(this,tr("Error"),tr("Please select type of boundary!"));
            return;
        }
        foreach(Boundary b,mesh->patchDict->boundaries)
        {
            if(b.name == ui->txt_BoundaryName->text())
            {
                QMessageBox::critical(this,tr("Error"),tr("This name is already existed...!"));
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
        this->keepBoundary = true;
        LoadBoundaryControlItems();
        this->keepBoundary = false;
        ui->txt_Log->append("These surfaces have been merged to boundary " + b.name);
    }
    else
        QMessageBox::critical(this,tr("Error"),tr("Please select as least as once surface!"));
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
        this->keepBoundary = true;
        LoadBoundaryControlItems();
        this->keepBoundary = false;
        if(ui->tb_MeshBoundary->selectedItems().count() > 1)
            ui->txt_Log->append("These boundaries have been deleted");
        else
            ui->txt_Log->append("This boundary have been deleted");
    }
    else
        QMessageBox::critical(this,tr("Error"),tr("Please select as least as once boundary!"));
}

////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////  Create Mesh  //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_btn_CreateMesh_clicked()
{
    checkDFC();
    if(!CheckDFC){
        return;
    }
    bool flag_YesNo = false;
    if(path_Open != "")
    {
        switch(QMessageBox::question(this,"DMesh",tr("Do you want to ovewrite this case?"),QMessageBox::Yes |
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
    bool writeLog = false;
    if(flag_YesNo == false)
    {
        QString saveCase = QFileDialog::getSaveFileName(this);
        if(saveCase != "")
        {
            if(ui->checkBox_WriteLog->isChecked()){
                QString filelog = "Log_" + QDate::currentDate().toString("yyyyMMdd") + "_" + QTime::currentTime().toString("hhmm") + ".txt";
                logPath = saveCase + "/" + filelog;
                QFile *f = new QFile(logPath);
                if(f->exists())
                    f->remove();
                f->open(QIODevice::WriteOnly);
                f->close();
                writeLog = true;
            }
            if(!OpenFoam::CopyDir("Data/OpenFoamDefault",saveCase))
            {
                ui->txt_Log->append("Can't copy default value to " + saveCase);
                return;
            }
            QFile(saveCase + "/constant/polyMesh/blockMeshDict").remove();
            OpenFoam::RemoveDir(saveCase + QDir::separator() + "0");
            QDir a;
            a.mkdir(saveCase + QDir::separator() + "0");

            mesh->blockd->Write_Block_Dmesh(saveCase + "/constant/polyMesh/");
            mesh->snappyd->Write_Snappy(saveCase + "/system/");
            mesh->snappyd->Write_Topodict(saveCase + "/system/");
            if(!QDir(saveCase + "/constant/triSurface").exists())
                QDir().mkdir(saveCase + "/constant/triSurface");
            for(int i=0; i<mesh->snappyd->gUserDefine.n; i++)
            {
                QFile(mesh->snappyd->gUserDefine.user_Defines[i].direction).copy(saveCase + "/constant/triSurface/" + mesh->snappyd->gUserDefine.user_Defines[i].name_file);
            }
            for(int i=0; i<mesh->snappyd->gUserDefineCellZone.n; i++)
            {
                QFile(mesh->snappyd->gUserDefineCellZone.user_Defines[i].direction).copy(saveCase + "/constant/triSurface/" + mesh->snappyd->gUserDefineCellZone.user_Defines[i].name_file);
            }
            if(mesh->patchDict->boundaries.size() > 0)
            {
                mesh->patchDict->WritePatchDict(saveCase + "/system");
            }
            ui->txt_Log->append("Creating mesh...!\n\n");
            createrThread = new MyThread();
            createrThread->logPath = this->logPath;
            createrThread->writeLog = writeLog;
            connect(createrThread,SIGNAL(changed(QString)),this,SLOT(Thread_Changed(QString)));
            OpenFoam::SetOpenFOAMPath(saveCase);
            createrThread->SetCommand("blockMesh");
            this->comment = "blockMesh";
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
                this->comment = "blockMesh";
                createrThread->ThreadName("blockMesh");
                createrThread->start();
                while(createrThread->isRunning())
                {
                    QApplication::processEvents();
                }
            }
            for(int i = 0; i < mesh->snappyd->gUserDefineCellZone.refi_Fea.n; i++)
            {
                createrThread->SetCommand("surfaceFeatureExtract -includedAngle " + QString::number(mesh->snappyd->gUserDefineCellZone.refi_Fea.feature[i].angle) +
                                          " constant/triSurface/" + mesh->snappyd->gUserDefineCellZone.refi_Fea.feature[i].name + ".stl " + mesh->snappyd->gUserDefineCellZone.refi_Fea.feature[i].name);
                this->comment = "blockMesh";
                createrThread->ThreadName("blockMesh");
                createrThread->start();
                while(createrThread->isRunning())
                {
                    QApplication::processEvents();
                }
            }
            createrThread->SetSubCommand("-overwrite",2);
            createrThread->SetCommand("snappyHexMesh");
            this->comment = "snappyHexMesh";
            createrThread->ThreadName("snappyHexMesh");
            createrThread->start();
            while(createrThread->isRunning())
            {
                QApplication::processEvents();
            }
            createrThread->SetSubCommand("-overwrite",2);
            createrThread->SetCommand("createPatch");
            this->comment = "createPatch";
            createrThread->ThreadName("createPatch");
            createrThread->start();
            while(createrThread->isRunning())
            {
                QApplication::processEvents();
            }
            createrThread->SetSubCommand("-blockOrder -orderPoints -overwrite",2);
            createrThread->SetCommand("renumberMesh");
            this->comment = "renumberMesh";
            createrThread->ThreadName("renumberMesh");
            createrThread->start();
            while(createrThread->isRunning())
            {
                QApplication::processEvents();
            }
            if(mesh->snappyd->gBoxCellZone.boxes.size() > 0 || mesh->snappyd->gCylinCellZone.cylins.size() > 0 ||
                    mesh->snappyd->gSphereCellZone.sphere.size() > 0 || mesh->snappyd->gUserDefineCellZone.user_Defines.size() > 0) {
                createrThread->SetSubCommand("",2);
                createrThread->SetCommand("topoSetDict -dict system/DMESH.topoSetDict");
                createrThread->ThreadName("topoSetDict");
                createrThread->start();
                while(createrThread->isRunning())
                {
                    QApplication::processEvents();
                }
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
            OpenFoam::SetOpenFOAMPath(saveCase);

        }
    }
    else
    {
        if(ui->checkBox_WriteLog->isChecked()){
            QString filelog = "Log_" + QDate::currentDate().toString("yyyyMMdd") + "_" + QTime::currentTime().toString("hhmm") + ".txt";
            logPath = path_Open + "/" + filelog;
            QFile *f = new QFile(logPath);
            if(f->exists())
                f->remove();
            f->open(QIODevice::WriteOnly);
            f->close();
            writeLog = true;
        }
        //use current folder
        QFile(path_Open + "/constant/polyMesh/blockMeshDict").remove();

        mesh->blockd->Write_Block_Dmesh(path_Open + "/constant/polyMesh/");

        QFile(path_Open + "/system/snappyHexMeshDict").remove();
        mesh->snappyd->Write_Snappy(path_Open + "/system/");
        mesh->snappyd->Write_Topodict(path_Open + "/system/");
        if(!QDir(path_Open + "/constant/triSurface").exists())
            QDir().mkdir(path_Open + "/constant/triSurface");
        for(int i=0; i<mesh->snappyd->gUserDefine.n; i++)
        {
            QFile(mesh->snappyd->gUserDefine.user_Defines[i].direction).copy(path_Open + "/constant/triSurface/" + mesh->snappyd->gUserDefine.user_Defines[i].name_file);
        }
        for(int i=0; i<mesh->snappyd->gUserDefineCellZone.n; i++)
        {
            QFile(mesh->snappyd->gUserDefineCellZone.user_Defines[i].direction).copy(path_Open + "/constant/triSurface/" + mesh->snappyd->gUserDefineCellZone.user_Defines[i].name_file);
        }
        if(mesh->patchDict->boundaries.size() > 0)
        {
            QFile(path_Open + "/system/createPatchDict").remove();
            mesh->patchDict->WritePatchDict(path_Open + "/system");
        }
        ui->txt_Log->append("Creating mesh...!\n\n");
        createrThread = new MyThread();
        createrThread->logPath = this->logPath;
        createrThread->writeLog = writeLog;
        connect(createrThread,SIGNAL(changed(QString)),this,SLOT(Thread_Changed(QString)));
        OpenFoam::SetOpenFOAMPath(path_Open);
        createrThread->SetCommand("blockMesh");
        this->comment = "blockMesh";
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
            this->comment = "blockMesh";
            createrThread->start();
            while(createrThread->isRunning())
            {
                QApplication::processEvents();
            }
        }
        createrThread->SetSubCommand("-overwrite",2);
        createrThread->SetCommand("snappyHexMesh");
        this->comment = "snappyHexMesh";
        createrThread->ThreadName("snappyHexMesh");
        createrThread->start();
        while(createrThread->isRunning())
        {
            QApplication::processEvents();
        }
        createrThread->SetSubCommand("-overwrite",2);
        createrThread->SetCommand("createPatch");
        this->comment = "createPatch";
        createrThread->ThreadName("createPatch");
        createrThread->start();
        while(createrThread->isRunning())
        {
            QApplication::processEvents();
        }
        createrThread->SetSubCommand("-blockOrder -orderPoints -overwrite",2);
        createrThread->SetCommand("renumberMesh");
        this->comment = "renumberMesh";
        createrThread->ThreadName("renumberMesh");
        createrThread->start();
        while(createrThread->isRunning())
        {
            QApplication::processEvents();
        }
        createrThread->SetCommand("topoSetDict -dict system/DMESH.topoSetDict");
        this->comment = "topoSetDict";
        createrThread->ThreadName("topoSetDict");
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
        ui->txt_Log->append("Settings for location vetices have been done");
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
                ui->txt_Log->append("Successfully delete "+ name +" box");
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
                ui->txt_Log->append("Successfully delete "+ name +" box");
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
                ui->txt_Log->append("Successfully delete "+ name +" box");
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
                ui->txt_Log->append("Successfully delete "+ name +" STL");
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
        mesh->snappyd->Write_Topodict(path_Open + "/system/");
        if(!QDir(path_Open + "/constant/triSurface").exists())
            QDir().mkdir(path_Open + "/constant/triSurface");
        for(int i=0; i<mesh->snappyd->gUserDefine.n; i++)
        {
            QFile(mesh->snappyd->gUserDefine.user_Defines[i].direction).copy(path_Open + "/constant/triSurface/" + mesh->snappyd->gUserDefine.user_Defines[i].name_file);
        }
        for(int i=0; i<mesh->snappyd->gUserDefineCellZone.n; i++)
        {
            QFile(mesh->snappyd->gUserDefineCellZone.user_Defines[i].direction).copy(path_Open + "/constant/triSurface/" + mesh->snappyd->gUserDefineCellZone.user_Defines[i].name_file);
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
    QString saveCase = QFileDialog::getSaveFileName(this,"",lastFileSTL);
    if(saveCase != "")
    {
        if(!OpenFoam::CopyDir("Data/OpenFoamDefault",saveCase))
        {
            ui->txt_Log->append("Can't copy default value to " + saveCase);
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
        mesh->snappyd->Write_Topodict(saveCase + "/system/");
        if(!QDir(saveCase + "/constant/triSurface").exists())
            QDir().mkdir(saveCase + "/constant/triSurface");
        for(int i=0; i<mesh->snappyd->gUserDefine.n; i++)
        {
            QFile(mesh->snappyd->gUserDefine.user_Defines[i].direction).copy(saveCase + "/constant/triSurface/" + mesh->snappyd->gUserDefine.user_Defines[i].name_file);
        }
        for(int i=0; i<mesh->snappyd->gUserDefineCellZone.n; i++)
        {
            QFile(mesh->snappyd->gUserDefineCellZone.user_Defines[i].direction).copy(path_Open + "/constant/triSurface/" + mesh->snappyd->gUserDefineCellZone.user_Defines[i].name_file);
        }
        if(mesh->patchDict->boundaries.size() > 0)
        {
            mesh->patchDict->WritePatchDict(saveCase + "/system");
        }
        OpenFoam::SetOpenFOAMPath(saveCase);
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
    RefinementFeaturesSTL *rFeature;
    for(int i = 0 ; i < mesh->snappyd->gUserDefine.refi_Sur.n; i++)
    {
        if(mesh->snappyd->gUserDefine.refi_Sur.surfaces[i].name == currentSurface){
            rSurface = &mesh->snappyd->gUserDefine.refi_Sur.surfaces[i];
            rFeature = &mesh->snappyd->gUserDefine.refi_Fea;
        }
    }
    for(int i = 0 ; i < mesh->snappyd->gUserDefineCellZone.refi_Sur.n; i++)
    {
        if(mesh->snappyd->gUserDefineCellZone.refi_Sur.surfaces[i].name == currentSurface){
            rSurface = &mesh->snappyd->gUserDefineCellZone.refi_Sur.surfaces[i];
            rFeature = &mesh->snappyd->gUserDefineCellZone.refi_Fea;
        }
    }


    W_MeshSTLRefineAdvance *w = new W_MeshSTLRefineAdvance(rSurface,rFeature,&mesh->snappyd->resolveFeatureAngle);
    if(w->exec())
    {
        ui->txt_Log->append("Defining of advance values for "+ ui->tb_boundary->currentItem()->text() +" surface has been done");
    }
}
bool MainWindow::RemoveRefineDistant(RefinementRegions *refi_Reg, QString currentSurface, float lv1, int lv2)
{
    for(int i = 0; i< refi_Reg->n; i++)
    {
        if(refi_Reg->region[i].name == currentSurface)
        {
            for(int j = 0; j < refi_Reg->region[i].distances.size(); j ++)
            {
                if(refi_Reg->region[i].distances[j].lv1 == lv1 && refi_Reg->region[i].distances[j].lv2 == lv2)
                {
                    refi_Reg->region[i].distances.remove(j);
                    refi_Reg->region[i].n = refi_Reg->region[i].n - 1;
                    break;
                }
            }
//            LoadRefineDistanceSurface(currentSurface,index);
            ReloadRefineDistanceSurface(&refi_Reg->region[i]);
            return true;
        }
    }
    return false;
}
bool MainWindow::AddRefineRegion(RefinementRegions *refi_Reg, QString currentSurface, float lv1, int lv2)
{
    for(int i = 0; i< refi_Reg->n; i++)
    {
        if(refi_Reg->region[i].name == currentSurface)
        {
            int j;
            RefinementDistance r;
            r.lv1 = lv1;
            r.lv2 = lv2;
            for(j = 0; j < refi_Reg->region[i].distances.size(); j ++)
            {
                if(refi_Reg->region[i].distances[j].lv1 > lv1)
                {
                    refi_Reg->region[i].distances.insert(j,r);
                    break;
                }
            }
            if(j == refi_Reg->region[i].distances.size())
                refi_Reg->region[i].distances.append(r);
            refi_Reg->region[i].n = refi_Reg->region[i].n + 1;
//            LoadRefineDistanceSurface(currentSurface,0);
            ReloadRefineDistanceSurface(&refi_Reg->region[i]);
            ui->txt_Log->append("Settings of refining distance for " + currentSurface + " have been done");
            return true;
        }
    }
    return false;
}

void MainWindow::on_tb_MeshRefineAroundSurface_cellClicked(int row, int column)
{
    if(ui->tb_boundary->currentRow() < 0)
        return;
    foreach(QTableWidgetItem *item,ui->tb_boundary->selectedItems()){
        QString currentSurface = item->text();
        if(column == 2)
        {
            if(row < ui->tb_MeshRefineAroundSurface->rowCount() - 1)
            {
                if(QMessageBox::warning(this,tr("Warning"),tr("Do you want to delete this field")))
                {
                    float lv1 = ui->tb_MeshRefineAroundSurface->item(row,0)->text().toFloat();
                    int lv2 = ui->tb_MeshRefineAroundSurface->item(row,1)->text().toInt();
                    //Foreach Box
                    if(RemoveRefineDistant(&mesh->snappyd->gBox.refi_Reg,currentSurface,lv1,lv2))
                        return;
                    //Foreach Cilynder
                    if(RemoveRefineDistant(&mesh->snappyd->gCylin.refi_Reg,currentSurface,lv1,lv2))
                        return;
                    //Foreach Sphere
                    if(RemoveRefineDistant(&mesh->snappyd->gSphere.refi_Reg,currentSurface,lv1,lv2))
                        return;
                    //Foreach STL
                    if(RemoveRefineDistant(&mesh->snappyd->gUserDefine.refi_Reg,currentSurface,lv1,lv2))
                        return;
                    //Foreach Box Cell zone
                    if(RemoveRefineDistant(&mesh->snappyd->gBoxCellZone.refi_Reg,currentSurface,lv1,lv2))
                        return;
                    //Foreach Cilynder Cell zone
                    if(RemoveRefineDistant(&mesh->snappyd->gCylinCellZone.refi_Reg,currentSurface,lv1,lv2))
                        return;
                    //Foreach Sphere Cell zone
                    if(RemoveRefineDistant(&mesh->snappyd->gSphereCellZone.refi_Reg,currentSurface,lv1,lv2))
                        return;
                    //Foreach STL Cell zone
                    if(RemoveRefineDistant(&mesh->snappyd->gUserDefineCellZone.refi_Reg,currentSurface,lv1,lv2))
                        return;
                }
            }  else {
                RefinementDistance r;
                QString c = ui->tb_MeshRefineAroundSurface->item(row,0)->text();
                QString d = ui->tb_MeshRefineAroundSurface->item(row,1)->text();
                if(c == "" || d == ""){
                    QMessageBox::warning(this,tr("Error"),tr("Please input all field...!"));
                    return;
                }
                bool a,b;
                float lv1 = c.toFloat(&a);
                int lv2 = d.toInt(&b);
                if(!a || !b)
                {
                    QMessageBox::warning(this,tr("Error"),tr("Please input all field...!"));
                    return;
                }
                //Foreach Box
                if(AddRefineRegion(&mesh->snappyd->gBox.refi_Reg,currentSurface,lv1,lv2))
                    return;
                //Foreach Cilynder
                if(AddRefineRegion(&mesh->snappyd->gCylin.refi_Reg,currentSurface,lv1,lv2))
                    return;
                //Foreach Sphere
                if(AddRefineRegion(&mesh->snappyd->gSphere.refi_Reg,currentSurface,lv1,lv2))
                    return;
                //Foreach STL
                if(AddRefineRegion(&mesh->snappyd->gUserDefine.refi_Reg,currentSurface,lv1,lv2))
                    return;
                //Foreach Box Cell zone
                if(AddRefineRegion(&mesh->snappyd->gBoxCellZone.refi_Reg,currentSurface,lv1,lv2))
                    return;
                //Foreach Cilynder Cell zone
                if(AddRefineRegion(&mesh->snappyd->gCylinCellZone.refi_Reg,currentSurface,lv1,lv2))
                    return;
                //Foreach Sphere Cell zone
                if(AddRefineRegion(&mesh->snappyd->gSphereCellZone.refi_Reg,currentSurface,lv1,lv2))
                    return;
                //Foreach STL Cell zone
                if(AddRefineRegion(&mesh->snappyd->gUserDefineCellZone.refi_Reg,currentSurface,lv1,lv2))
                    return;
            }
        }
    }
}

void MainWindow::on_txt_Level_Min_Surface_Refine_textChanged(const QString &arg1)
{
    if(arg1=="")
    {
        return;
    }
    bool a;
    int temp = arg1.toInt(&a);
    if(a ==false)
    {
        QMessageBox::warning(this,tr("Error"),tr("Please enter a valid number!"));
        ui->txt_Level_Min_Surface_Refine->undo();
    }
    else
    {
        if(temp<0)
        {
            QMessageBox::warning(this,tr("Error"),tr("Please enter a number greater than or equal 0!"));
            ui->txt_Level_Min_Surface_Refine->undo();
        } else {
            float delta = mesh->snappyd->MaxIn(mesh->snappyd->deltaBaseMesh.x,mesh->snappyd->deltaBaseMesh.y,mesh->snappyd->deltaBaseMesh.y)/pow(2.0,temp);
            ui->txt_Delta_Min_Surface_Refine->setText(QString::number(delta));
        }
    }
}

void MainWindow::on_txt_Level_Max_Surface_Refine_textChanged(const QString &arg1)
{
    if(arg1=="")
    {
        return;
    }
    bool a;
    int temp = arg1.toInt(&a);
    if(a ==false)
    {
        QMessageBox::warning(this,tr("Error"),tr("Please enter a valid number!"));
        ui->txt_Level_Max_Surface_Refine->undo();
    }
    else
    {
        if(temp<0)
        {
            QMessageBox::warning(this,tr("Error"),tr("Please enter a number greater than or equal 0!"));
            ui->txt_Level_Max_Surface_Refine->undo();
        } else {
            float delta = mesh->snappyd->MinIn(mesh->snappyd->deltaBaseMesh.x,mesh->snappyd->deltaBaseMesh.y,mesh->snappyd->deltaBaseMesh.y)/pow(2.0,temp);
            ui->txt_Delta_Max_Surface_Refine->setText(QString::number(delta));
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
        QMessageBox::warning(this,tr("Error"),tr("Please enter a number greater than or equal 0!"));
        ui->txt_Level_Volume->undo();
    }
    else
    {
        if(temp<0)
        {
            QMessageBox::warning(this,tr("Error"),tr("Please enter a number greater than or equal 0"));
            ui->txt_Level_Volume->undo();
        }
    }
}

void MainWindow::on_actionOpen_triggered()
{
    if(lastFolderCase == ""){
        lastFolderCase == QDir::currentPath();
    }
    QString dir;
        dir= QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                               lastFolderCase,
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
        if(dir!="")
        {
            lastFolderCase = dir;
            OpenFoam::SetOpenFOAMPath(dir);
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
            if(mesh->snappyd->ReadSTLFile(path,-1))
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
            AddFaceToList(mesh->snappyd->gBox.boxes[i].name,1);
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
            AddFaceToList(mesh->snappyd->gCylin.cylins[i].name,1);
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
            AddFaceToList(mesh->snappyd->gSphere.sphere[i].name,1);
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
            AddFaceToList(mesh->snappyd->gBoxRegion.boxes[i].name,2);
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
            AddFaceToList(mesh->snappyd->gCylinRegion.cylins[i].name,2);
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
            AddFaceToList(mesh->snappyd->gSphereRegion.sphere[i].name,2);
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
        ui->actionClose->setEnabled(true);
        this->isClose = false;
        ui->actionCheck_mesh->setDisabled(false);
        ui->actionParaView->setDisabled(false);
        ui->actionSave->setDisabled(false);
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
    ui->actionClose->setEnabled(false);
}

void MainWindow::SetButtonEnable(bool value)
{
    ui->actionOpen->setEnabled(value);
//    ui->actionSave->setEnabled(value);

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
    if(isClose){
        return;
    }
    flag_Item_Face_Click = true;
    QString currentSurface = ui->tb_boundary->currentItem()->text();
    if(flag_btnSurface_Click == true)
    {
        //snappy box
        RefinementSurfaces *refi_Sur = &mesh->snappyd->gBox.refi_Sur;
        for(int i = 0; i< refi_Sur->n; i++)
        {
            if(refi_Sur->surfaces[i].name == currentSurface)
            {
                ui->txt_Level_Min_Surface_Refine->setText(QString::number
                                                          (refi_Sur->surfaces[i].lv1));
                ui->txt_Level_Max_Surface_Refine->setText(QString::number
                                                 (refi_Sur->surfaces[i].lv2));
                LoadRefineDistanceSurface(currentSurface,0);
                ui->btn_MeshRefineAdvance->hide();
                ui->groupBox_MeshSurfaceAround->show();
                return;
            }
        }
        //snappy cylinder
        refi_Sur = &mesh->snappyd->gCylin.refi_Sur;
        for(int i = 0; i< refi_Sur->n; i++)
        {
            if(refi_Sur->surfaces[i].name == currentSurface)
            {
                ui->txt_Level_Min_Surface_Refine->setText(QString::number
                                                          (refi_Sur->surfaces[i].lv1));
                ui->txt_Level_Max_Surface_Refine->setText(QString::number
                                                 (refi_Sur->surfaces[i].lv2));
                LoadRefineDistanceSurface(currentSurface,1);
                ui->btn_MeshRefineAdvance->hide();
                ui->groupBox_MeshSurfaceAround->show();
                return;
            }
        }
        //snappy sphere
        refi_Sur = &mesh->snappyd->gSphere.refi_Sur;
        for(int i = 0; i< refi_Sur->n; i++)
        {
            if(refi_Sur->surfaces[i].name == currentSurface)
            {
                ui->txt_Level_Min_Surface_Refine->setText(QString::number
                                                          (refi_Sur->surfaces[i].lv1));
                ui->txt_Level_Max_Surface_Refine->setText(QString::number
                                                 (refi_Sur->surfaces[i].lv2));
                LoadRefineDistanceSurface(currentSurface,3);
                ui->btn_MeshRefineAdvance->hide();
                ui->groupBox_MeshSurfaceAround->show();
                return;
            }
        }
        //snappy userdefine        
        RefinementSurfacesSTL *refi_SurSTL = &mesh->snappyd->gUserDefine.refi_Sur;
        for(int i = 0; i< refi_SurSTL->n; i++)
        {
            if(refi_SurSTL->surfaces[i].name == currentSurface)
            {
                ui->txt_Level_Min_Surface_Refine->setText(QString::number
                                                          (refi_SurSTL->surfaces[i].lv1));
                ui->txt_Level_Max_Surface_Refine->setText(QString::number
                                                 (refi_SurSTL->surfaces[i].lv2));
                //Region
                LoadRefineDistanceSurface(currentSurface,2);

                ui->btn_MeshRefineAdvance->show();
                ui->groupBox_MeshSurfaceAround->show();
                return;
            }
        }
        //snappy box cell zone
        refi_Sur = &mesh->snappyd->gBoxCellZone.refi_Sur;
        for(int i = 0; i< refi_Sur->n; i++)
        {
            if(refi_Sur->surfaces[i].name == currentSurface)
            {
                ui->txt_Level_Min_Surface_Refine->setText(QString::number
                                                          (refi_Sur->surfaces[i].lv1));
                ui->txt_Level_Max_Surface_Refine->setText(QString::number
                                                 (refi_Sur->surfaces[i].lv2));
                LoadRefineDistanceSurface(currentSurface,4);
                ui->btn_MeshRefineAdvance->hide();
                ui->groupBox_MeshSurfaceAround->show();
                return;
            }
        }
        //snappy cylinder cell zone
        refi_Sur = &mesh->snappyd->gCylinCellZone.refi_Sur;
        for(int i = 0; i< refi_Sur->n; i++)
        {
            if(refi_Sur->surfaces[i].name == currentSurface)
            {
                ui->txt_Level_Min_Surface_Refine->setText(QString::number
                                                          (refi_Sur->surfaces[i].lv1));
                ui->txt_Level_Max_Surface_Refine->setText(QString::number
                                                 (refi_Sur->surfaces[i].lv2));
                LoadRefineDistanceSurface(currentSurface,5);
                ui->btn_MeshRefineAdvance->hide();
                ui->groupBox_MeshSurfaceAround->show();
                return;
            }
        }
        //snappy sphere cell zone
        refi_Sur = &mesh->snappyd->gSphereCellZone.refi_Sur;
        for(int i = 0; i< refi_Sur->n; i++)
        {
            if(refi_Sur->surfaces[i].name == currentSurface)
            {
                ui->txt_Level_Min_Surface_Refine->setText(QString::number
                                                          (refi_Sur->surfaces[i].lv1));
                ui->txt_Level_Max_Surface_Refine->setText(QString::number
                                                 (refi_Sur->surfaces[i].lv2));
                LoadRefineDistanceSurface(currentSurface,6);
                ui->btn_MeshRefineAdvance->hide();
                ui->groupBox_MeshSurfaceAround->show();
                return;
            }
        }
        //snappy userdefine cell zone
        refi_SurSTL = &mesh->snappyd->gUserDefineCellZone.refi_Sur;
        for(int i = 0; i< refi_SurSTL->n; i++)
        {
            if(refi_SurSTL->surfaces[i].name == currentSurface)
            {
                ui->txt_Level_Min_Surface_Refine->setText(QString::number
                                                          (refi_SurSTL->surfaces[i].lv1));
                ui->txt_Level_Max_Surface_Refine->setText(QString::number
                                                 (refi_SurSTL->surfaces[i].lv2));
                //Region
                LoadRefineDistanceSurface(currentSurface,7);

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
                return;
            }
        }refi_Reg = &mesh->snappyd->gBoxCellZone.refi_Reg;
        for(int i = 0; i < refi_Reg->region.size(); i++)
        {
            if(refi_Reg->region[i].name == currentSurface)
            {
                if(refi_Reg->region[i].mode =="inside" )
                    ui->cb_MeshVolumeMode->setCurrentIndex(1);
                else if(refi_Reg->region[i].mode =="outside" )
                    ui->cb_MeshVolumeMode->setCurrentIndex(2);
                ui->txt_Level_Volume->setText(QString::number(refi_Reg->region[i].lv2));
                return;
            }
        }
        refi_Reg = &mesh->snappyd->gCylinCellZone.refi_Reg;
        for(int i = 0; i < refi_Reg->region.size(); i++)
        {
            if(refi_Reg->region[i].name == currentSurface)
            {
                if(refi_Reg->region[i].mode =="inside" )
                    ui->cb_MeshVolumeMode->setCurrentIndex(1);
                else if(refi_Reg->region[i].mode =="outside" )
                    ui->cb_MeshVolumeMode->setCurrentIndex(2);
                ui->txt_Level_Volume->setText(QString::number(refi_Reg->region[i].lv2));
                return;
            }
        }
        refi_Reg = &mesh->snappyd->gSphereCellZone.refi_Reg;
        for(int i = 0; i < refi_Reg->region.size(); i++)
        {
            if(refi_Reg->region[i].name == currentSurface)
            {
                if(refi_Reg->region[i].mode =="inside" )
                    ui->cb_MeshVolumeMode->setCurrentIndex(1);
                else if(refi_Reg->region[i].mode =="outside" )
                    ui->cb_MeshVolumeMode->setCurrentIndex(2);
                ui->txt_Level_Volume->setText(QString::number(refi_Reg->region[i].lv2));
                return;
            }
        }
        refi_Reg = &mesh->snappyd->gUserDefineCellZone.refi_Reg;
        for(int i = 0; i < refi_Reg->region.size(); i++)
        {
            if(refi_Reg->region[i].name == currentSurface)
            {
                if(refi_Reg->region[i].mode =="inside" )
                    ui->cb_MeshVolumeMode->setCurrentIndex(1);
                else if(refi_Reg->region[i].mode =="outside" )
                    ui->cb_MeshVolumeMode->setCurrentIndex(2);
                ui->txt_Level_Volume->setText(QString::number(refi_Reg->region[i].lv2));
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
                ui->cb_GeometryNewMesh->setCurrentIndex(1);
                ui->rbn_Surface->setChecked(true);
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
                ui->cb_GeometryNewMesh->setCurrentIndex(1);
                ui->rbn_Surface->setChecked(true);
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
                ui->cb_GeometryNewMesh->setCurrentIndex(1);
                ui->rbn_Surface->setChecked(true);
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
                ui->cb_GeometryNewMesh->setCurrentIndex(0);
                ui->rbn_Surface->setChecked(true);
                ui->txt_GeometrySurfaceFileStl->setText(mesh->snappyd->gUserDefine.user_Defines[i].direction);
                return;
            }
        }
        for(int i=0; i< mesh->snappyd->gBoxCellZone.n; i++)
        {
            if(currentSurface==  mesh->snappyd->gBoxCellZone.boxes[i].name)
            {
                ui->cb_GeometryNewMesh->setCurrentIndex(1);
                ui->rbn_Cellzone->setChecked(true);
                if(mesh->snappyd->gBoxCellZone.boxes[i].type =="searchableBox")
                    ui->cb_SurfaceType->setCurrentIndex(0);

                ui->txt_GeometrySurfaceName->setText(mesh->snappyd->gBoxCellZone.boxes[i].name);
                ui->txt_Min_Box_Sur_X->setText(QString::number(mesh->snappyd->gBoxCellZone.boxes[i].min.x));
                ui->txt_Min_Box_Sur_Y->setText(QString::number(mesh->snappyd->gBoxCellZone.boxes[i].min.y));
                ui->txt_Min_Box_Sur_Z->setText(QString::number(mesh->snappyd->gBoxCellZone.boxes[i].min.z));

                ui->txt_Max_Box_Sur_X->setText(QString::number(mesh->snappyd->gBoxCellZone.boxes[i].max.x));
                ui->txt_Max_Box_Sur_Y->setText(QString::number(mesh->snappyd->gBoxCellZone.boxes[i].max.y));
                ui->txt_Max_Box_Sur_Z->setText(QString::number(mesh->snappyd->gBoxCellZone.boxes[i].max.z));
                return;
            }
        }
        for(int i=0; i< mesh->snappyd->gCylinCellZone.n; i++)
        {
            if(currentSurface==  mesh->snappyd->gCylinCellZone.cylins[i].name)
            {
                ui->cb_GeometryNewMesh->setCurrentIndex(1);
                ui->rbn_Cellzone->setChecked(true);
                if(mesh->snappyd->gCylinCellZone.cylins[i].type =="searchableCylinder")
                    ui->cb_SurfaceType->setCurrentIndex(1);

                ui->txt_GeometrySurfaceName->setText(mesh->snappyd->gCylinCellZone.cylins[i].name);
                ui->txt_P1_Cyl_Sur_X->setText(QString::number(mesh->snappyd->gCylinCellZone.cylins[i].point1.x));
                ui->txt_P1_Cyl_Sur_Y->setText(QString::number(mesh->snappyd->gCylinCellZone.cylins[i].point1.y));
                ui->txt_P1_Cyl_Sur_Z->setText(QString::number(mesh->snappyd->gCylinCellZone.cylins[i].point1.z));

                ui->txt_P2_Cyl_Sur_X->setText(QString::number(mesh->snappyd->gCylinCellZone.cylins[i].point2.x));
                ui->txt_P2_Cyl_Sur_Y->setText(QString::number(mesh->snappyd->gCylinCellZone.cylins[i].point2.y));
                ui->txt_P2_Cyl_Sur_Z->setText(QString::number(mesh->snappyd->gCylinCellZone.cylins[i].point2.z));

                ui->txt_Radius_Cyl_Sur->setText(QString::number(mesh->snappyd->gCylinCellZone.cylins[i].radius));
                return;
            }
        }
        for(int i=0; i< mesh->snappyd->gSphereCellZone.n; i++)
        {
            if(currentSurface==  mesh->snappyd->gSphereCellZone.sphere[i].name)
            {
                ui->cb_GeometryNewMesh->setCurrentIndex(1);
                ui->rbn_Cellzone->setChecked(true);
                if(mesh->snappyd->gSphereCellZone.sphere[i].type =="searchableSphere")
                    ui->cb_SurfaceType->setCurrentIndex(2);
                ui->txt_GeometrySurfaceName->setText(mesh->snappyd->gSphereCellZone.sphere[i].name);
                ui->txt_P_Sphe_Sur_X->setText(QString::number(mesh->snappyd->gSphereCellZone.sphere[i].centre.x));
                ui->txt_P_Sphe_Sur_Y->setText(QString::number(mesh->snappyd->gSphereCellZone.sphere[i].centre.y));
                ui->txt_P_Sphe_Sur_Z->setText(QString::number(mesh->snappyd->gSphereCellZone.sphere[i].centre.z));

                ui->txt_Radius_Sphe_Sur->setText(QString::number(mesh->snappyd->gSphereCellZone.sphere[i].radius));
                return;
            }
        }
        for(int i=0; i< mesh->snappyd->gUserDefineCellZone.n; i++)
        {
            if(currentSurface==  mesh->snappyd->gUserDefineCellZone.user_Defines[i].name)
            {
                ui->cb_GeometryNewMesh->setCurrentIndex(0);
                ui->rbn_Cellzone->setChecked(true);
                ui->txt_GeometrySurfaceFileStl->setText(mesh->snappyd->gUserDefineCellZone.user_Defines[i].direction);

                return;
            }
        }
        for(int i=0; i< mesh->snappyd->gBoxRegion.n; i++)
        {
            if(currentSurface==  mesh->snappyd->gBoxRegion.boxes[i].name)
            {
                ui->cb_GeometryNewMesh->setCurrentIndex(1);
                ui->rbn_Volume->setChecked(true);
                if(mesh->snappyd->gBoxRegion.boxes[i].type =="searchableBox")
                    ui->cb_SurfaceType->setCurrentIndex(0);

                ui->txt_GeometrySurfaceName->setText(mesh->snappyd->gBoxRegion.boxes[i].name);
                ui->txt_Min_Box_Sur_X->setText(QString::number(mesh->snappyd->gBoxRegion.boxes[i].min.x));
                ui->txt_Min_Box_Sur_Y->setText(QString::number(mesh->snappyd->gBoxRegion.boxes[i].min.y));
                ui->txt_Min_Box_Sur_Z->setText(QString::number(mesh->snappyd->gBoxRegion.boxes[i].min.z));

                ui->txt_Max_Box_Sur_X->setText(QString::number(mesh->snappyd->gBoxRegion.boxes[i].max.x));
                ui->txt_Max_Box_Sur_Y->setText(QString::number(mesh->snappyd->gBoxRegion.boxes[i].max.y));
                ui->txt_Max_Box_Sur_Z->setText(QString::number(mesh->snappyd->gBoxRegion.boxes[i].max.z));
                return;
            }
        }
        for(int i=0; i< mesh->snappyd->gCylinRegion.n; i++)
        {
            if(currentSurface==  mesh->snappyd->gCylinRegion.cylins[i].name)
            {
                ui->cb_GeometryNewMesh->setCurrentIndex(1);
                ui->rbn_Volume->setChecked(true);
                if(mesh->snappyd->gCylinRegion.cylins[i].type =="searchableCylinder")
                    ui->cb_SurfaceType->setCurrentIndex(1);

                ui->txt_GeometrySurfaceName->setText(mesh->snappyd->gCylinRegion.cylins[i].name);
                ui->txt_P1_Cyl_Sur_X->setText(QString::number(mesh->snappyd->gCylinRegion.cylins[i].point1.x));
                ui->txt_P1_Cyl_Sur_Y->setText(QString::number(mesh->snappyd->gCylinRegion.cylins[i].point1.y));
                ui->txt_P1_Cyl_Sur_Z->setText(QString::number(mesh->snappyd->gCylinRegion.cylins[i].point1.z));

                ui->txt_P2_Cyl_Sur_X->setText(QString::number(mesh->snappyd->gCylinRegion.cylins[i].point2.x));
                ui->txt_P2_Cyl_Sur_Y->setText(QString::number(mesh->snappyd->gCylinRegion.cylins[i].point2.y));
                ui->txt_P2_Cyl_Sur_Z->setText(QString::number(mesh->snappyd->gCylinRegion.cylins[i].point2.z));

                ui->txt_Radius_Cyl_Sur->setText(QString::number(mesh->snappyd->gCylinRegion.cylins[i].radius));
                return;
            }
        }
        for(int i=0; i< mesh->snappyd->gSphereRegion.n; i++)
        {
            if(currentSurface==  mesh->snappyd->gSphereRegion.sphere[i].name)
            {
                ui->cb_GeometryNewMesh->setCurrentIndex(1);
                ui->rbn_Volume->setChecked(true);
                ui->txt_GeometrySurfaceFileStl->setText("");

                if(mesh->snappyd->gSphereRegion.sphere[i].type =="searchableSphere")
                    ui->cb_SurfaceType->setCurrentIndex(2);
                ui->txt_GeometrySurfaceName->setText(mesh->snappyd->gSphereRegion.sphere[i].name);
                ui->txt_P_Sphe_Sur_X->setText(QString::number(mesh->snappyd->gSphereRegion.sphere[i].centre.x));
                ui->txt_P_Sphe_Sur_Y->setText(QString::number(mesh->snappyd->gSphereRegion.sphere[i].centre.y));
                ui->txt_P_Sphe_Sur_Z->setText(QString::number(mesh->snappyd->gSphereRegion.sphere[i].centre.z));

                ui->txt_Radius_Sphe_Sur->setText(QString::number(mesh->snappyd->gSphereRegion.sphere[i].radius));
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

void MainWindow::on_actionClose_triggered()
{
//    on_actionSave_triggered();
    if(ui->tb_boundary->rowCount() > 0){
        this->isClose = true;
        for(int i = 0; i < ui->tb_boundary->rowCount(); i++){
            ui->tb_boundary->removeRow(i);
        }
        ui->tb_boundary->clearSelection();
        ui->tb_boundary->clear();
        Remove_All_Face();
        ui->tb_boundary->setRowCount(0);
    }
    ui->layout_Mesh->removeWidget(mesh);
    listSurfaces.clear();

    mesh = new DMesh();
    ui->layout_Mesh->addWidget(mesh);
    LoadControlsVisible();
//    ui->tb_MeshSurface->clear();
//    ui->tb_boundary->viewport()->update();
//    ui->tb_MeshSurface->viewport()->update();
//    ui->cb_BoundingType->clear();
//    ui->cb_MeshVolumeMode->clear();
//    ui->cb_VolumeType->clear();
//    LoadControlItems();
    LoadLocationInMesh();
    ui->tb_MeshRefineAroundSurface->horizontalHeader()->setStretchLastSection(true);
    ui->actionClose->setEnabled(false);
    ui->actionParaView->setDisabled(true);
    ui->actionParaView->setDisabled(true);
    ui->actionSave->setDisabled(true);
    ui->checkBox_WriteLog->setChecked(true);
}

void MainWindow::on_actionCapture_triggered()
{
    W_Capture *w_cap = new W_Capture();
    QString CapName;
    if(w_cap->exec()){
        CapName = w_cap->saveFileName;
    }
    if(w_cap->result() == 0)
        return;
    QString iPath = w_cap->GetFilePath();
    QString iFormat = w_cap->GetFormat();
    QString image = iPath+iFormat;
    QPixmap *pixmap;
    if (mesh != NULL && mesh->isVisible())
    {
        pixmap = new QPixmap(mesh->renderPixmap(mesh->width(),mesh->height()));
//        vtkMesh->render(pixmap);
        image = CapName + iFormat;
    }
//    if(isCreated == true && frm_ChartResidual->isVisible())
//    {
//        pixmap = new QPixmap(frm_ChartResidual->size());
//        frm_ChartResidual->render(pixmap);
//        image = iPath + "_Residual_" + iFormat;
//    }
    if(pixmap->save(image,0,100))
        QMessageBox::information(this,tr("Save picture"),tr("The picture %1 has been saved.").arg(image),QMessageBox::Ok,QMessageBox::NoButton);
}

void MainWindow::on_cb_GeometryNewMesh_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "STL") {
        ui->groupBox_GeometryImportSTL->show();
        ui->groupBox_GeometrySimpleSurface->hide();
    } else {
        ui->groupBox_GeometryImportSTL->hide();
        ui->groupBox_GeometrySimpleSurface->show();
    }
}

void MainWindow::on_btn_GeoDefineNew_clicked()
{
    if(ui->rbn_Surface->isChecked()) {
        if(ui->cb_GeometryNewMesh->currentText() == "STL")
            ImportSTLSurface();
        if(ui->cb_GeometryNewMesh->currentText() == "Simple surface")
            DefineSimpleSurface();
    } else if(ui->rbn_Cellzone->isChecked()) {
        if(ui->cb_GeometryNewMesh->currentText() == "STL")
            ImportSTLCellzone();
        if(ui->cb_GeometryNewMesh->currentText() == "Simple surface")
            DefineSimpleCellZone();
    } else if(ui->rbn_Volume->isChecked()) {
        if(ui->cb_GeometryNewMesh->currentText() == "Simple surface")
            DefineSimpleVolume();
    }
    this->isClose = false;
    ui->actionClose->setEnabled(true);
    ui->actionCheck_mesh->setEnabled(true);
    ui->actionParaView->setEnabled(true);
    ui->actionSave->setDisabled(false);
}

void MainWindow::on_rbn_Volume_clicked(bool checked)
{
    if(checked) {
        ui->cb_GeometryNewMesh->setCurrentIndex(1);
        ui->cb_GeometryNewMesh->setEnabled(false);
    } else {
        ui->cb_GeometryNewMesh->setEnabled(true);
    }
}

void MainWindow::on_rbn_Cellzone_clicked(bool checked)
{
    if(checked){
        ui->cb_GeometryNewMesh->setEnabled(true);
        ui->cb_GeometryNewMesh->setCurrentIndex(0);
    }
}

void MainWindow::on_rbn_Surface_clicked(bool checked)
{
    if(checked) {
        ui->cb_GeometryNewMesh->setEnabled(true);
        ui->cb_GeometryNewMesh->setCurrentIndex(0);
    }
}

void MainWindow::on_actionCheck_mesh_triggered()
{
    if(!CheckDFC){
        return;
    }
    if(checkMeshThread->isRunning() && comment == "checkMesh"){
        return;
    }
    checkMeshThread = new MyThread();
    checkMeshThread->SetCommand("checkMesh");
    this->comment = "checkMesh";
    checkMeshThread->ThreadName("check mesh");
    connect(checkMeshThread,SIGNAL(changed(QString)),this,SLOT(Thread_Changed(QString)));
    checkMeshThread->start();
}

void MainWindow::on_actionParaView_triggered()
{

    if(checkMeshThread->isRunning() && comment == "paraFoam"){
        return;
    }
    checkMeshThread = new MyThread();
    checkMeshThread->SetCommand("paraFoam");
    this->comment = "paraFoam";
    checkMeshThread->ThreadName("paraFoam");
//    connect(checkMeshThread,SIGNAL(changed(QString)),this,SLOT(Thread_Changed(QString)));
    ui->txt_Log->append("ParaView run...");
    checkMeshThread->start();
}

void MainWindow::on_tb_boundary_itemSelectionChanged()
{
    if(ui->tb_boundary->selectedItems().count() > 0){
        ui->btn_DeleteSurface->setEnabled(true);
        ui->btn_ViewMesh->setEnabled(true);
    }else{
        ui->btn_DeleteSurface->setDisabled(true);
        ui->btn_ViewMesh->setDisabled(true);
    }
}
