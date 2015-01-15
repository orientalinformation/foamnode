#include "dmesh.h"


DMesh::DMesh(QGLWidget *parent) : QGLWidget(parent)
{
    is_DrawCenterCoordinate = true;
    is_DrawNoteCoordinate = true;
    f = -1;
    zOut =0;
    zIn = 0;
    xAverage = 0;
    yAverage = 0;
    zAverage = 0;
    flagFirstStart = false;
    zoomScale =100;
    scalePoint = 0;
    //declare dmesh
    blockd = new Block_Dmesh();
    blockd->boundMesh.n=0;
    blockd->vertice.n=0;
    //creator defaut general
    blockd->gen_Bounding.Layers=0;
    blockd->gen_Bounding.element.x = 10;
    blockd->gen_Bounding.element.y = 10;
    blockd->gen_Bounding.element.z = 10;

    snappyd = new Snappy_Dmesh();

    snappyd->resolveFeatureAngle = 30;

    //creator defaut general and layers
    snappyd->add_Layers_Controls.nLayer =0;
    snappyd->add_Layers_Controls.relativeSizes= true;
    snappyd->add_Layers_Controls.finalLayerThickness = 0.5;
    snappyd->add_Layers_Controls.minThickness = 0.1;
    snappyd->add_Layers_Controls.expansionRatio= 1.2;
    snappyd->add_Layers_Controls.featureAngle = 180;
    snappyd->add_Layers_Controls.slipFeatureAngle = 30;

    snappyd->gBox.n =0;
    snappyd->gBox.refi_Sur.n=0;
    snappyd->gBox.refi_Reg.n=0;

    snappyd->gCylin.n =0;
    snappyd->gCylin.refi_Sur.n=0;
    snappyd->gCylin.refi_Reg.n=0;

    snappyd->gSphere.n =0;
    snappyd->gSphere.refi_Sur.n=0;
    snappyd->gSphere.refi_Reg.n=0;

    snappyd->gUserDefine.n =0;
    snappyd->gUserDefine.refi_Sur.n=0;
    snappyd->gUserDefine.refi_Reg.n=0;
    snappyd->gUserDefine.refi_Fea.n=0;

    snappyd->gBoxCellZone.n =0;
    snappyd->gBoxCellZone.refi_Sur.n=0;
    snappyd->gBoxCellZone.refi_Reg.n=0;

    snappyd->gCylinCellZone.n =0;
    snappyd->gCylinCellZone.refi_Sur.n=0;
    snappyd->gCylinCellZone.refi_Reg.n=0;

    snappyd->gSphereCellZone.n =0;
    snappyd->gSphereCellZone.refi_Sur.n=0;
    snappyd->gSphereCellZone.refi_Reg.n=0;

    snappyd->gUserDefineCellZone.n =0;
    snappyd->gUserDefineCellZone.refi_Sur.n=0;
    snappyd->gUserDefineCellZone.refi_Reg.n=0;
    snappyd->gUserDefineCellZone.refi_Fea.n=0;

    //Region for refine
    snappyd->gBoxRegion.n =0;
    snappyd->gBoxRegion.refi_Reg.n=0;

    snappyd->gCylinRegion.n =0;
    snappyd->gCylinRegion.refi_Reg.n=0;

    snappyd->gSphereRegion.n =0;
    snappyd->gSphereRegion.refi_Reg.n=0;


    //declare location in mesh
    snappyd->locationInMesh.x=0;
    snappyd->locationInMesh.y=0;
    snappyd->locationInMesh.z=0;

    //declare carfull
    //snappyd->sTL.n=0;

    //declare min max
    snappyd->defaultBounding.minBouDef.x = 1000000;
    snappyd->defaultBounding.minBouDef.y = 1000000;
    snappyd->defaultBounding.minBouDef.z = 1000000;

    snappyd->defaultBounding.maxBouDef.x = -1000000;
    snappyd->defaultBounding.maxBouDef.y = -1000000;
    snappyd->defaultBounding.maxBouDef.z = -1000000;    
	
    snappyd->min_Max.name_Surface="";
    snappyd->min_Max.min.x=1000000;
    snappyd->min_Max.min.y=1000000;
    snappyd->min_Max.min.z=1000000;
    snappyd->min_Max.max.x=-1000000;
    snappyd->min_Max.max.y=-1000000;
    snappyd->min_Max.max.z=-1000000;
    //Declare Patch Dict
    patchDict = new PatchDict();
    this->isCtrlPress=false;

}

void DMesh::SetHomeView()
{
    xRot = 0;
    yRot = 0;
    zRot = 0;
    xMove = 0;
    yMove = 0;
    zMove = 0;
    xCenter = 0;
    yCenter = 0;
    zCenter = 0;

    zoomScale = 500;
    updateGL();
}

void DMesh::SetRotate(float x, float y, float z)
{
    this->xRot = x;
    this->yRot = y;
    this->zRot = z;
    updateGL();
}
void DMesh::SetDrawCoordinate(bool note, bool center)
{
    is_DrawCenterCoordinate = center;
    is_DrawNoteCoordinate = note;
    updateGL();
}

void DMesh::SetViewList(QStringList views)
{
    this->viewList = views;
    updateGL();
}
void renderCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions,GLUquadricObj *quadric)
{
    float vx = x2-x1;
    float vy = y2-y1;
    float vz = z2-z1;

    //handle the degenerate case of z1 == z2 with an approximation
    if(vz == 0)
        vz = .0001;

    float v = sqrt( vx*vx + vy*vy + vz*vz );
    float ax = 57.2957795*acos( vz/v );
    if ( vz < 0.0 )
        ax = -ax;
    float rx = -vy*vz;
    float ry = vx*vz;
    glPushMatrix();

    //draw the cylinder body
    glTranslatef( x1,y1,z1 );
    glRotatef(ax, rx, ry, 0.0);
    gluQuadricOrientation(quadric,GLU_OUTSIDE);
    gluCylinder(quadric, radius, radius, v, subdivisions, 1);

    //draw the first cap
    gluQuadricOrientation(quadric,GLU_INSIDE);
    gluDisk( quadric, 0.0, radius, subdivisions, 1);
    glTranslatef( 0,0,v );

    //draw the second cap
    gluQuadricOrientation(quadric,GLU_OUTSIDE);
    gluDisk( quadric, 0.0, radius, subdivisions, 1);
    glPopMatrix();
}
void renderCylinder_convenient(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions)
{
    //the same quadric can be re-used for drawing many cylinders
    GLUquadricObj *quadric=gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluQuadricDrawStyle(quadric,GLU_LINE);
    renderCylinder(x1,y1,z1,x2,y2,z2,radius,subdivisions,quadric);
    gluDeleteQuadric(quadric);
}
void renderSphere(float x, float y, float z, float radius)
{
    glPushMatrix();
        glTranslatef(x,y,z);
        GLUquadric *myQuad;
        GLint slices, stacks;
        myQuad=gluNewQuadric();
        gluQuadricDrawStyle(myQuad,GLU_LINE);
        slices = stacks = 20;
        gluSphere( myQuad , radius , slices , stacks  );
    glPopMatrix();
}
QStringList DMesh::GetViewList()
{
    return this->viewList;
}

void DMesh::ResetLocation()
{
    snappyd->locationInMesh.x = xAverage;
    snappyd->locationInMesh.y = yAverage;
    snappyd->locationInMesh.z = zAverage;
}

void DMesh::initializeGL()
{
    qglClearColor(QColor(82,87,110));
    glClearDepth(1.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);   
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable (GL_LINE_SMOOTH);
    glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
//    glClearColor(0.82,0.87,0.11,0);
}
void DMesh::resizeGL(int w, int h)
{
    screenWidth = w;
    screenHeight = h;
    radian_Rotate = w/3;
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity () ;
    gluPerspective(45,(float)w/(float)h,0.001,100000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
float absf(float value)
{
    if(value > 0)
        return value;
    else
        return -value;
}
void DMesh::FindVMax()
{
//    //Center the object
//    xTranslate = (snappyd->defaultBounding.maxBouDef.x - snappyd->defaultBounding.minBouDef.x)/2.0;
//    yTranslate = (snappyd->defaultBounding.maxBouDef.y - snappyd->defaultBounding.minBouDef.y)/2.0;
//    zTranslate = (snappyd->defaultBounding.maxBouDef.z - snappyd->defaultBounding.minBouDef.z)/2.0;
//    //Find the value to scale the object fixable to screen
//    if(vMax == 2000)
//        vMax = 0;
//    if(vMax < absf(snappyd->defaultBounding.maxBouDef.x - xTranslate))
//        vMax = absf(snappyd->defaultBounding.maxBouDef.x - xTranslate);
//    if(vMax < absf(snappyd->defaultBounding.minBouDef.x - xTranslate))
//        vMax = absf(snappyd->defaultBounding.minBouDef.x - xTranslate);
//    if(vMax < absf(snappyd->defaultBounding.maxBouDef.y - yTranslate))
//        vMax = absf(snappyd->defaultBounding.maxBouDef.y - yTranslate);
//    if(vMax < absf(snappyd->defaultBounding.minBouDef.y - yTranslate))
//        absf(snappyd->defaultBounding.minBouDef.y - yTranslate);
//    if(vMax < absf(snappyd->defaultBounding.maxBouDef.z - zTranslate))
//        vMax = absf(snappyd->defaultBounding.maxBouDef.z - zTranslate);
//    if(vMax < absf(snappyd->defaultBounding.minBouDef.z - zTranslate))
//        vMax = absf(snappyd->defaultBounding.minBouDef.z - zTranslate);
//    scalePoint = tan(M_PI/8)/vMax;
    scalePoint = snappyd->MaxIn(snappyd->defaultBounding.maxBouDef.x,snappyd->defaultBounding.maxBouDef.y,snappyd->defaultBounding.maxBouDef.z)*9;
    if(scalePoint < 0)
        scalePoint = -1*scalePoint;
}

void DMesh::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
    glLoadIdentity();
    glTranslatef(0,0,-6);
    //glTranslatef(xTranslate,yTranslate,zTranslate);
    glPushMatrix();
        glViewport(xMove - screenWidth*1.5 ,
               yMove -screenHeight*1.5 ,
               4*screenWidth , 4*screenHeight);
        glTranslatef(0,0,zMove - scalePoint);
        glRotatef(xRot,1,0,0);
        glRotatef(yRot,0,1,0);
        glRotatef(zRot,0,0,1);

        glTranslatef(-xCenter/4,-yCenter/4,-zCenter/4);
        DrawLocationPoint();
        DrawEdges();

    glPopMatrix();

    ShowHeighWidthGreen();

    DrawCenterCoordinate();
    DrawNoteCoordinate();
}

void DMesh::ShowHeighWidthGreen()
{
    QString textshow;
    if(snappyd->list_Surface_Min_Max.length() !=0)
    {
        FindVMax();
        float x1, x2, y1, y2, z1, z2;
        x1 = 1000000;
        y1 = 1000000;
        z1 = 1000000;

        x2 = -1000000;
        y2 = -1000000;
        z2 = -1000000;
        QList<Surface_Min_Max> l = snappyd->list_Surface_Min_Max;
        this->radiusCaculationMax.clear();
        this->radiusCaculationMin.clear();
        for(int i= 0; i <l.length(); i++)
        {
                //x
                if(x1 > l[i].min.x)
                {
                    x1 = l[i].min.x;
                }
                if(x2 < l[i].max.x)
                {
                    x2 = l[i].max.x;
                }
                if(x1 > l[i].max.x)
                {
                    x1 = l[i].max.x;
                }
                if(x2 < l[i].min.x)
                {
                    x2 = l[i].min.x;
                }                                
                //y
                if(y1 > l[i].min.y)
                {
                    y1 = l[i].min.y;
                }
                if(y2 < l[i].max.y)
                {
                    y2 = l[i].max.y;
                }
                if(y1 > l[i].max.y)
                {
                    y1 = l[i].max.y;
                }
                if(y2 < l[i].min.y)
                {
                    y2 = l[i].min.y;
                }
                //z
                if(z1 > l[i].min.z)
                {
                    z1 = l[i].min.z;
                }
                if(z2 < l[i].max.z)
                {
                    z2 = l[i].max.z;
                }
                if(z1 > l[i].max.z)
                {
                    z1 = l[i].max.z;
                }
                if(z2 < l[i].min.z)
                {
                    z2 = l[i].min.z;
                }
                radiusCaculationMax.append(l[i].max.x);
                radiusCaculationMax.append(l[i].max.y);
                radiusCaculationMax.append(l[i].max.z);

                radiusCaculationMin.append(l[i].min.x);
                radiusCaculationMin.append(l[i].min.y);
                radiusCaculationMin.append(l[i].min.z);
        }
        textshow = "Min(" + QString::number(x1);
        textshow += ", " + QString::number(y1);
        textshow += ", " + QString::number(z1);
        textshow += ") Max(" + QString::number(x2);
        textshow += ", " + QString::number(y2);
        textshow += ", " + QString::number(z2);
        textshow += ")";
        xAverage = (x1+x2)/2;
        yAverage = (y1+y2)/2;
        zAverage = (z1+z2)/2;

    }
    else
    {
        textshow = "Min(0, 0, 0) Max(0, 0, 0)";
    }
    glPushMatrix();

        //glTranslatef(1,1,1);
        glClear(GL_DEPTH_BUFFER_BIT);
        glViewport( -screenWidth/2,0, screenWidth , screenHeight);
            glColor3f(1,1,1);
            QFont myFont("TypeWriter", 8);
            renderText(0, -2.4, 0, textshow, myFont);

    glPopMatrix();
}

void DMesh::DrawCenterCoordinate()
{
    if(!is_DrawCenterCoordinate)
        return;
    QVector <float*> coordinate;
    coordinate.resize(4);
    int length = scalePoint/12;
    if(length == 0){
        length = 1;
    }
    for (int i = 0; i < 4; i++)
    {
        coordinate[i] = new float[3];
        for (int j = 0; j < 3; j++)
        {
            if(i == j + 1)
                coordinate[i][j] = length;
            else
                coordinate[i][j] = 0;
        }
    }
    glPushMatrix();
    glLineWidth(1.0);

        //Draw the center coordinate
        glPushMatrix();
            glViewport(xMove - screenWidth*1.5 ,
                   yMove -screenHeight*1.5 ,
                   4*screenWidth , 4*screenHeight);
            glTranslatef(0,0,zMove - scalePoint);
            glRotatef(xRot,1,0,0);
            glRotatef(yRot,0,1,0);
            glRotatef(zRot,0,0,1);

            glBegin(GL_LINES);
            for (int i = 1; i < 4; i++)
            {
                if(i == 1)
                    glColor3f(1,0,0);
                else if(i == 2)
                    glColor3f(1,1,0);
                else
                    glColor3f(0,1,0);
                glVertex3f(-coordinate[i][0],-coordinate[i][1],-coordinate[i][2]);
                glVertex3f(coordinate[i][0],coordinate[i][1],coordinate[i][2]);

            }
            glEnd();
        glPopMatrix();
    glPopMatrix();
}

void DMesh::DrawNoteCoordinate()
{
    if(!is_DrawNoteCoordinate)
        return;
    QVector <float*> coordinate;
    coordinate.resize(4);
    for (int i = 0; i < 4; i++)
    {
        coordinate[i] = new float[3];
        for (int j = 0; j < 3; j++)
        {
            if(i == j + 1)
                coordinate[i][j] = 1.6;
            else
                coordinate[i][j] = 0;
        }
    }

    //Draw the note coordinate

    glPushMatrix();
        glLineWidth(1.0);
        glPushMatrix();

            glClear(GL_DEPTH_BUFFER_BIT);
            glViewport( -screenWidth/18,0, screenWidth/4 , screenHeight/4);
            glRotatef(xRot,1,0,0);
            glRotatef(yRot,0,1,0);
            glRotatef(zRot,0,0,1);

            GLUquadric * a;
            a = gluNewQuadric();
            for (int i = 1; i < 4; i++)
            {
                QString coor;
                if(i == 1)
                {
                    glColor3f(1,0,0);
                    coor = "X";
                }
                else if(i == 2)
                {
                    glColor3f(1,1,0);
                    coor = "Y";
                }
                else
                {
                    glColor3f(0,1,0);
                    coor = "Z";
                }
                glBegin(GL_LINES);
                glVertex3f(coordinate[0][0],coordinate[0][1],coordinate[0][2]);
                glVertex3f(coordinate[i][0],coordinate[i][1],coordinate[i][2]);
                glEnd();
                glPushMatrix();
                glTranslatef(coordinate[i][0],coordinate[i][1],coordinate[i][2]);
                if(i == 1)
                    glRotatef(90,0,1,0);
                else if(i == 2)
                    glRotatef(-90,1,0,0);
                gluCylinder(a,0.12,0.0,0.6,10,1);
                glPopMatrix();
                QFont myFont("TypeWriter", 10);
                renderText(coordinate[i][0]+0.2,coordinate[i][1]+0.2,coordinate[i][2]+0.2,coor,myFont);
            }            
            gluDeleteQuadric(a);

        glPopMatrix();
    glPopMatrix();
}

void DMesh::DrawLocationPoint()
{
    glPushMatrix();
        glColor3f(1.0,1.0,0.0);
        glPointSize(10);
        glBegin(GL_POINTS);
            glVertex3d(snappyd->locationInMesh.x,snappyd->locationInMesh.y,snappyd->locationInMesh.z);
        glEnd();
    glPopMatrix();
}
void DMesh::DrawEdges()
{
    glPushMatrix();
        glLineWidth(0.5);
        DrawSTL();
        DrawGeometry();
        DrawBounding();
    glPopMatrix();
}

void DMesh::DrawBounding()
{
    if(blockd->isAutomatic)
        return;
    int nBoundingFace  = blockd->boundMesh.n ;
    //For each face of bounding
//    glDisable(GL_DEPTH_TEST);
//    glColor3f(1,0.2,1);
//    for(int i = 0; i < nBoundingFace; i ++)
//    {
//        if(blockd->boundMesh.n > 0)
//        {
//            int nFace = blockd->boundMesh.bounsType[i].n;
//            //For each face of geometry
//            for(int j = 0; j < nFace; j ++)
//            {
//                int nPoint = blockd->boundMesh.bounsType[i].faces[j].n;
//                //For each points of face
//                glBegin(GL_POLYGON);
//                for(int k = 0; k < nPoint; k ++)
//                {
//                    int index = blockd->boundMesh.bounsType[i].faces[j].points[k];
//                    glVertex3f(blockd->vertice.v[index].x,blockd->vertice.v[index].y,blockd->vertice.v[index].z);
//                }
//                glEnd();
//            }
//        }
//    }
//    glEnable(GL_DEPTH_TEST);
    glColor3f(0,1,0);
    for(int i = 0; i < nBoundingFace; i ++)
    {
        if(blockd->boundMesh.n > 0)
        {
            int nFace = blockd->boundMesh.bounsType[i].n;
            //For each face of geometry
            for(int j = 0; j < nFace; j ++)
            {
                int nPoint = blockd->boundMesh.bounsType[i].faces[j].n;
                //For each points of face
                glBegin(GL_LINE_LOOP);
                for(int k = 0; k < nPoint; k ++)
                {
                    int index = blockd->boundMesh.bounsType[i].faces[j].points[k];
                    glVertex3f(blockd->vertice.v[index].x,blockd->vertice.v[index].y,blockd->vertice.v[index].z);
                }
                glEnd();
            }
        }
    }
}

void DMesh::DrawGeometry()
{
    int nGeometry  = snappyd->facename.size();
    if(viewList.size() == 0)
        return;
    //For each box
    for(int i = 0; i < nGeometry; i ++)
    {
//        if(snappyd->facename.size() <= i || snappyd->facetype.size() <= i)
//            return;
        if(this->viewList.contains(snappyd->facename[i]))
        {
            if(snappyd->facetype[i] == 1)
                glColor3f(1,1,1);
            else if(snappyd->facetype[i] == 0){
                glColor3f(0,1,0);
            }else
                glColor3f(1,0.666666667,0);

            int j = snappyd->facezones[i][0];
            int nFace = snappyd->facezones[i][1];
            //For each face of geometry
            for(; j < nFace; j ++)
            {
                int nPoint = 4;
                //For each points of face
                glBegin(GL_LINE_LOOP);
                for(int k = 0; k < nPoint; k ++)
                {
                    int index = snappyd->faces[j][k];
                    glVertex3fv(snappyd->points[index]);
                }
                glEnd();
            }
        }
    }
    //For each cylinder
    int nCylinder = snappyd->gCylin.n;
    for(int i = 0; i < nCylinder; i ++)
    {
        if(this->viewList.contains(snappyd->gCylin.cylins[i].name))
        {
            PointDmesh *p1 = &snappyd->gCylin.cylins[i].point1;
            PointDmesh *p2 = &snappyd->gCylin.cylins[i].point2;
            glColor3f(1,1,1);
            renderCylinder_convenient(p1->x,p1->y,p1->z,p2->x,p2->y,p2->z,snappyd->gCylin.cylins[i].radius,30);
        }
    }
    nCylinder = snappyd->gCylinCellZone.n;
    for(int i = 0; i < nCylinder; i ++)
    {
        if(this->viewList.contains(snappyd->gCylinCellZone.cylins[i].name))
        {
            PointDmesh *p1 = &snappyd->gCylinCellZone.cylins[i].point1;
            PointDmesh *p2 = &snappyd->gCylinCellZone.cylins[i].point2;
            glColor3f(0,1,0);
            renderCylinder_convenient(p1->x,p1->y,p1->z,p2->x,p2->y,p2->z,snappyd->gCylinCellZone.cylins[i].radius,30);
        }
    }
    nCylinder = snappyd->gCylinRegion.n;
    for(int i = 0; i < nCylinder; i ++)
    {
        if(this->viewList.contains(snappyd->gCylinRegion.cylins[i].name))
        {
            PointDmesh *p1 = &snappyd->gCylinRegion.cylins[i].point1;
            PointDmesh *p2 = &snappyd->gCylinRegion.cylins[i].point2;
            glColor3f(1,0.666666667,0);
            renderCylinder_convenient(p1->x,p1->y,p1->z,p2->x,p2->y,p2->z,snappyd->gCylinRegion.cylins[i].radius,30);
        }
    }
    //For each Sphere
    int nSphere = snappyd->gSphere.n;
    for(int i = 0; i < nSphere; i++)
    {
        if(this->viewList.contains(snappyd->gSphere.sphere[i].name))
        {
            PointDmesh *p = &snappyd->gSphere.sphere[i].centre;
            glColor3f(1,1,1);
            renderSphere(p->x,p->y,p->z,snappyd->gSphere.sphere[i].radius);
        }
    }
    nSphere = snappyd->gSphereCellZone.n;
        for(int i = 0; i < nSphere; i++)
        {
            if(this->viewList.contains(snappyd->gSphereCellZone.sphere[i].name))
            {
                PointDmesh *p = &snappyd->gSphereCellZone.sphere[i].centre;
                glColor3f(0,1,0);
                renderSphere(p->x,p->y,p->z,snappyd->gSphereCellZone.sphere[i].radius);
            }
        }
    nSphere = snappyd->gSphereRegion.n;
    for(int i = 0; i < nSphere; i++)
    {
        if(this->viewList.contains(snappyd->gSphereRegion.sphere[i].name))
        {
            PointDmesh *p = &snappyd->gSphereRegion.sphere[i].centre;
            glColor3f(1,0.666666667,0);
            renderSphere(p->x,p->y,p->z,snappyd->gSphereRegion.sphere[i].radius);
        }
    }
}

void DMesh::DrawSTL()
{
    int n_sTL = snappyd->sTL.size();
    if(viewList.size() == 0)
        return;
    for(int k=0; k< n_sTL; k++)
    {
        if(this->viewList.contains(snappyd->sTL[k].name))
        {
            int nSolid  = snappyd->sTL[k].n ;
            //For each STL geometry
            for(int i = 0; i < nSolid; i ++)
            {
                Solid *sTemp = &snappyd->sTL[k].solids[i];
                glColor3f(sTemp->color.x,sTemp->color.y,sTemp->color.z);
                int nFacet = sTemp->n;
                for(int j = 0; j < nFacet; j ++)
                {
                    glBegin(GL_LINE_LOOP);
                    for(int k = 0; k < sTemp->facets[j].n; k++)
                        glVertex3f(sTemp->facets[j].vertexs[k].x, sTemp->facets[j].vertexs[k].y,sTemp->facets[j].vertexs[k].z);
                    glEnd();
                }
            }
        }
        if(this->viewList.filter(snappyd->sTL[k].name).size() > 0)
        {
            int nSolid  = snappyd->sTL[k].n ;
            //For each STL geometry
            for(int i = 0; i < nSolid; i ++)
            {
                Solid *sTemp = &snappyd->sTL[k].solids[i];
                    if(this->viewList.contains(snappyd->sTL[k].name + "_" + sTemp->name)){
                    glColor3f(sTemp->color.x,sTemp->color.y,sTemp->color.z);
                    int nFacet = sTemp->n;
                    for(int j = 0; j < nFacet; j ++)
                    {
                        glBegin(GL_LINE_LOOP);
                        for(int k = 0; k < sTemp->facets[j].n; k++)
                            glVertex3f(sTemp->facets[j].vertexs[k].x, sTemp->facets[j].vertexs[k].y,sTemp->facets[j].vertexs[k].z);
                        glEnd();
                    }
                }
            }
        }
    }
}

void DMesh::DrawFaces()
{

}

void DMesh::DrawPoint()
{
    int n = blockd->vertice.n;
    //For each geometry
    glBegin(GL_POINTS);
    for(int i = 0; i < n; i ++)
    {
         glVertex3f(blockd->vertice.v[i].x,blockd->vertice.v[i].y,blockd->vertice.v[i].z);
    }
    glEnd();

}

void DMesh::DrawCells()
{

}

void DMesh::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    if(event->buttons() & Qt::MidButton)
    {
        xMove += (event->x() - xMove - screenWidth/2);
        yMove -= (event->y() + yMove - screenHeight/2);
        xCenter = xMove * 8.66 / screenWidth;
        yCenter = yMove * 4.94 / screenHeight;
    }
    updateGL();
}
void DMesh::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();
    int z = radian_Rotate*radian_Rotate - (event->x()*event->x() + event->y()*event->y());
    z = sqrt(z*1.0);
    int dz = z - lastPosZ;

    if (event->buttons() & Qt::LeftButton)
    {
        xRot += dy % 360;
        yRot += dx % 360;
        zRot += dz % 360;
        updateGL();
    }
    else if (event->buttons() & Qt::RightButton)
    {
        xMove +=  dx;
        yMove -=  dy;
        updateGL();
    }
    lastPos = event->pos();
    lastPosZ = radian_Rotate*radian_Rotate - (lastPos.x()*lastPos.x() + lastPos.y()*lastPos.y());
    lastPosZ = sqrt(lastPosZ*1.0);
}
void DMesh::wheelEvent(QWheelEvent * event)
{
    if(f * event->delta() < 0)
    {
        f = -f;
        zoomScale = 100;
    }
    if(zoomScale > 0)
    {
        if(this->isCtrlPress)//vMax/100 > 120
            zMove += f*12000.0/zoomScale;
        else
            zMove += f*120.0/zoomScale;
        zoomScale += zoomScale*0.025;
    }
    else
        zoomScale = 1;
    updateGL();
}


float DMesh::minValue(QVector<float> values)
{
    float min = 0.0;
    if(values.count()>0){
        min = values[0];
        for(int i=1;i<values.count();++i){
            if(values[i] <min){
                min = values[i];
            }
        }
    }
    return min;
}

float DMesh::maxValue(QVector<float> values)
{
    float max = 0.0;
    if(values.count()>0){
        max = values[0];
        for(int i=1;i<values.count();++i){
            if(values[i] > max){
                max = values[i];
            }
        }
    }
    return max;
}



