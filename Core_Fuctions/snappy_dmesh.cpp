#include "snappy_dmesh.h"

Snappy_Dmesh::Snappy_Dmesh()
{
}

void Snappy_Dmesh::FindMinMaxDefaultBounding(float x, float y, float z)
{
    if(min_Max.min.x > x)
    {
        min_Max.min.x = x;
    }
    if(min_Max.max.x < x)
    {
        min_Max.max.x = x;
    }
    //y
    if(min_Max.min.y > y)
    {
        min_Max.min.y = y;
    }
    if(min_Max.max.y < y)
    {
        min_Max.max.y = y;
    }
    //z
    if(min_Max.min.z > z)
    {
        min_Max.min.z = z;
    }
    if(min_Max.max.z < z)
    {
        min_Max.max.z = z;
    }
}

void Snappy_Dmesh::FindMinMax(QList<Surface_Min_Max> l)
{
    //declare min max
    defaultBounding.minBouDef.x = 1000000;
    defaultBounding.minBouDef.y = 1000000;
    defaultBounding.minBouDef.z = 1000000;

    defaultBounding.maxBouDef.x = -1000000;
    defaultBounding.maxBouDef.y = -1000000;
    defaultBounding.maxBouDef.z = -1000000;
    for(int i= 0; i <l.length(); i++)
    {
            //x
            if(defaultBounding.minBouDef.x > l[i].min.x)
            {
                defaultBounding.minBouDef.x = l[i].min.x;
            }
            if(defaultBounding.maxBouDef.x < l[i].max.x)
            {
                defaultBounding.maxBouDef.x = l[i].max.x;
            }
            if(defaultBounding.minBouDef.x > l[i].max.x)
            {
                defaultBounding.minBouDef.x = l[i].max.x;
            }
            if(defaultBounding.maxBouDef.x < l[i].min.x)
            {
                defaultBounding.maxBouDef.x = l[i].min.x;
            }
            //y
            if(defaultBounding.minBouDef.y > l[i].min.y)
            {
                defaultBounding.minBouDef.y = l[i].min.y;
            }
            if(defaultBounding.maxBouDef.y < l[i].max.y)
            {
                defaultBounding.maxBouDef.y = l[i].max.y;
            }
            if(defaultBounding.minBouDef.y > l[i].max.y)
            {
                defaultBounding.minBouDef.y = l[i].max.y;
            }
            if(defaultBounding.maxBouDef.y < l[i].min.y)
            {
                defaultBounding.maxBouDef.y = l[i].min.y;
            }
            //z
            if(defaultBounding.minBouDef.z > l[i].min.z)
            {
                defaultBounding.minBouDef.z = l[i].min.z;
            }
            if(defaultBounding.maxBouDef.z < l[i].max.z)
            {
                defaultBounding.maxBouDef.z = l[i].max.z;
            }
            if(defaultBounding.minBouDef.z > l[i].max.z)
            {
                defaultBounding.minBouDef.z = l[i].max.z;
            }
            if(defaultBounding.maxBouDef.z < l[i].min.z)
            {
                defaultBounding.maxBouDef.z = l[i].min.z;
            }            
    }
    float lx, ly, lz;
    lx = (defaultBounding.maxBouDef.x - defaultBounding.minBouDef.x)*0.01;
    ly = (defaultBounding.maxBouDef.y - defaultBounding.minBouDef.y)*0.01;
    lz = (defaultBounding.maxBouDef.z - defaultBounding.minBouDef.z)*0.01;

    defaultBounding.maxBouDef.x = defaultBounding.maxBouDef.x +lx;
    defaultBounding.minBouDef.x = defaultBounding.minBouDef.x -lx;

    defaultBounding.maxBouDef.y = defaultBounding.maxBouDef.y +ly;
    defaultBounding.minBouDef.y = defaultBounding.minBouDef.y -ly;

    defaultBounding.maxBouDef.z = defaultBounding.maxBouDef.z +lz;
    defaultBounding.minBouDef.z = defaultBounding.minBouDef.z -lz;
}

QString Snappy_Dmesh::CreateHeader()
{
    QString str_File_New  = "/*--------------------------------*- C++ -*----------------------------------*\\\n";
    str_File_New += "| =========                 |                                                 |\n";
    str_File_New += "| \\\\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |\n";
    str_File_New += "|  \\\\    /   O peration     | Version:  2.2.0                                 |\n";
    str_File_New += "|   \\\\  /    A nd           | Web:      www.OpenFOAM.org                      |\n";
    str_File_New += "|    \\\\/     M anipulation  |                                                 |\n";
    str_File_New += "\\*---------------------------------------------------------------------------*/\n";
    str_File_New += "FoamFile\n";

    str_File_New += "{\n";
    str_File_New += "version 2.0;\n";
    str_File_New += "format ascii;\n";
    str_File_New += "class dictionary;\n";
    str_File_New += "location system;\n";
    str_File_New += "object snappyHexMeshDict;\n";
    str_File_New += "}\n";
    str_File_New += "// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //\n";
    str_File_New += "castellatedMesh     true;\n";
    str_File_New += "snap        true;\n";
    str_File_New += "addLayers	true;\n";
    return str_File_New;
}
QString Snappy_Dmesh::CreateGeometry()
{
    QString str_File_New = "geometry\n";

    str_File_New += "{\n";
    for(int i=0; i< gBox.n; i++)
    {
        str_File_New += gBox.boxes[i].name + "\n";
        str_File_New += "{\n";
        str_File_New += "type searchableBox;\n";
        str_File_New += "tag  surface;\n";
        str_File_New += "min ("+QString::number(gBox.boxes[i].min.x)+" "+QString::number(gBox.boxes[i].min.y)
                                                +" "+QString::number(gBox.boxes[i].min.z)+");\n";
        str_File_New += "max ("+QString::number(gBox.boxes[i].max.x)+" "+QString::number(gBox.boxes[i].max.y)
                                                +" "+QString::number(gBox.boxes[i].max.z)+");\n";
        str_File_New += "}\n";
    }
    for(int i=0; i< gCylin.n; i++)
    {
        str_File_New += gCylin.cylins[i].name + "\n";
        str_File_New += "{\n";
        str_File_New += "type searchableCylinder;\n";
        str_File_New += "tag  surface;\n";
        str_File_New += "point1 ("+QString::number(gCylin.cylins[i].point1.x)+" "+QString::number(gCylin.cylins[i].point1.y)
                                                +" "+QString::number(gCylin.cylins[i].point1.z)+");\n";
        str_File_New += "point2 ("+QString::number(gCylin.cylins[i].point2.x)+" "+QString::number(gCylin.cylins[i].point2.y)
                                                +" "+QString::number(gCylin.cylins[i].point2.z)+");\n";
        str_File_New += "radius "+QString::number(gCylin.cylins[i].radius)+";\n";
        str_File_New += "}\n";
    }
    for(int i=0; i< gSphere.n; i++)
    {
        str_File_New += gSphere.sphere[i].name + "\n";
        str_File_New += "{\n";
        str_File_New += "type searchableSphere;\n";
        str_File_New += "tag  surface;\n";
        str_File_New += "centre ("+QString::number(gSphere.sphere[i].centre.x)+" "+QString::number(gSphere.sphere[i].centre.y)
                                                +" "+QString::number(gSphere.sphere[i].centre.z)+");\n";
        str_File_New += "radius "+QString::number(gSphere.sphere[i].radius)+";\n";
        str_File_New += "}\n";
    }
    for(int i=0; i< gUserDefine.n; i++)
    {
        str_File_New += gUserDefine.user_Defines[i].name_file + "\n";
        str_File_New += "{\n";
        str_File_New += "type triSurfaceMesh;\n";
        str_File_New += "tag  surface;\n";
        str_File_New += "name "+gUserDefine.user_Defines[i].name+";\n";
        str_File_New += "}\n";
    }
    for(int i=0; i< gBoxCellZone.n; i++)
    {
        str_File_New += gBoxCellZone.boxes[i].name + "\n";
        str_File_New += "{\n";
        str_File_New += "type searchableBox;\n";
        str_File_New += "tag  cellZone;\n";
        str_File_New += "min ("+QString::number(gBoxCellZone.boxes[i].min.x)+" "+QString::number(gBoxCellZone.boxes[i].min.y)
                                                +" "+QString::number(gBoxCellZone.boxes[i].min.z)+");\n";
        str_File_New += "max ("+QString::number(gBoxCellZone.boxes[i].max.x)+" "+QString::number(gBoxCellZone.boxes[i].max.y)
                                                +" "+QString::number(gBoxCellZone.boxes[i].max.z)+");\n";
        str_File_New += "}\n";
    }
    for(int i=0; i< gCylinCellZone.n; i++)
    {
        str_File_New += gCylinCellZone.cylins[i].name + "\n";
        str_File_New += "{\n";
        str_File_New += "type searchableCylinder;\n";
        str_File_New += "tag  cellZone;\n";
        str_File_New += "point1 ("+QString::number(gCylinCellZone.cylins[i].point1.x)+" "+QString::number(gCylinCellZone.cylins[i].point1.y)
                                                +" "+QString::number(gCylinCellZone.cylins[i].point1.z)+");\n";
        str_File_New += "point2 ("+QString::number(gCylinCellZone.cylins[i].point2.x)+" "+QString::number(gCylinCellZone.cylins[i].point2.y)
                                                +" "+QString::number(gCylinCellZone.cylins[i].point2.z)+");\n";
        str_File_New += "radius "+QString::number(gCylinCellZone.cylins[i].radius)+";\n";
        str_File_New += "}\n";
    }
    for(int i=0; i< gSphereCellZone.n; i++)
    {
        str_File_New += gSphereCellZone.sphere[i].name + "\n";
        str_File_New += "{\n";
        str_File_New += "type searchableSphere;\n";
        str_File_New += "tag  cellZone;\n";
        str_File_New += "centre ("+QString::number(gSphereCellZone.sphere[i].centre.x)+" "+QString::number(gSphereCellZone.sphere[i].centre.y)
                                                +" "+QString::number(gSphereCellZone.sphere[i].centre.z)+");\n";
        str_File_New += "radius "+QString::number(gSphereCellZone.sphere[i].radius)+";\n";
        str_File_New += "}\n";
    }
    for(int i=0; i< gUserDefineCellZone.n; i++)
    {
        str_File_New += gUserDefineCellZone.user_Defines[i].name_file + "\n";
        str_File_New += "{\n";
        str_File_New += "type triSurfaceMesh;\n";
        str_File_New += "tag  cellZone;\n";
        str_File_New += "name "+gUserDefineCellZone.user_Defines[i].name+";\n";
        str_File_New += "}\n";
    }
    for(int i=0; i< gBoxRegion.n; i++)
    {
        str_File_New += gBoxRegion.boxes[i].name + "\n";
        str_File_New += "{\n";
        str_File_New += "type searchableBox;\n";
        str_File_New += "tag  volume;\n";
        str_File_New += "min ("+QString::number(gBoxRegion.boxes[i].min.x)+" "+QString::number(gBoxRegion.boxes[i].min.y)
                                                +" "+QString::number(gBoxRegion.boxes[i].min.z)+");\n";
        str_File_New += "max ("+QString::number(gBoxRegion.boxes[i].max.x)+" "+QString::number(gBoxRegion.boxes[i].max.y)
                                                +" "+QString::number(gBoxRegion.boxes[i].max.z)+");\n";
        str_File_New += "}\n";
    }
    for(int i=0; i< gCylinRegion.n; i++)
    {
        str_File_New += gCylinRegion.cylins[i].name + "\n";
        str_File_New += "{\n";
        str_File_New += "type searchableCylinder;\n";
        str_File_New += "tag  volume;\n";
        str_File_New += "point1 ("+QString::number(gCylinRegion.cylins[i].point1.x)+" "+QString::number(gCylinRegion.cylins[i].point1.y)
                                                +" "+QString::number(gCylinRegion.cylins[i].point1.z)+");\n";
        str_File_New += "point2 ("+QString::number(gCylinRegion.cylins[i].point2.x)+" "+QString::number(gCylinRegion.cylins[i].point2.y)
                                                +" "+QString::number(gCylinRegion.cylins[i].point2.z)+");\n";
        str_File_New += "radius "+QString::number(gCylinRegion.cylins[i].radius)+";\n";
        str_File_New += "}\n";
    }
    for(int i=0; i< gSphereRegion.n; i++)
    {
        str_File_New += gSphereRegion.sphere[i].name + "\n";
        str_File_New += "{\n";
        str_File_New += "type searchableSphere;\n";
        str_File_New += "tag  volume;\n";
        str_File_New += "centre ("+QString::number(gSphereRegion.sphere[i].centre.x)+" "+QString::number(gSphereRegion.sphere[i].centre.y)
                                                +" "+QString::number(gSphereRegion.sphere[i].centre.z)+");\n";
        str_File_New += "radius "+QString::number(gSphereRegion.sphere[i].radius)+";\n";
        str_File_New += "}\n";
    }
    str_File_New += "}\n";
    str_File_New += "\n";
    return str_File_New;
}
QString Snappy_Dmesh::CreateCastellatedMeshControls()
{
    QString str_File_New = "castellatedMeshControls\n";
    str_File_New +="{\n";
    str_File_New +="maxLocalCells 10000000;\n";
    str_File_New +="maxGlobalCells 20000000;\n";
    str_File_New +="minRefinementCells 2;\n";
    str_File_New +="nCellsBetweenLevels 3;\n";
    str_File_New +="\n";
    str_File_New +="features\n";
    str_File_New +="(\n";
    for(int i=0; i<gUserDefine.refi_Fea.n; i++)
    {
        str_File_New +="{\n";
        str_File_New +="file \"" + gUserDefine.refi_Fea.feature[i].name + ".eMesh\";\n";
        str_File_New +="level " + QString::number(gUserDefine.refi_Fea.feature[i].lv) + ";\n";
        str_File_New +="}\n";
    }
    for(int i=0; i<gUserDefineCellZone.refi_Fea.n; i++)
    {
        str_File_New +="{\n";
        str_File_New +="file \"" + gUserDefineCellZone.refi_Fea.feature[i].name + ".eMesh\";\n";
        str_File_New +="level " + QString::number(gUserDefineCellZone.refi_Fea.feature[i].lv) + ";\n";
        str_File_New +="}\n";
    }
    str_File_New +=");\n";
    str_File_New +="refinementSurfaces\n";
    str_File_New +="{\n";

    //Add refinement surfaces
    for(int i=0; i< gBox.refi_Sur.n; i++)
    {
        if(gBox.refi_Sur.surfaces[i].lv2 > 0)
        {
            str_File_New +=gBox.refi_Sur.surfaces[i].name + "\n";
            str_File_New +="{\n";
            str_File_New +="level ("+QString::number(gBox.refi_Sur.surfaces[i].lv1)
                                                    +" "+QString::number(gBox.refi_Sur.surfaces[i].lv2)+");\n";
            str_File_New +="}\n";
        }
    }
    str_File_New.append(CreateMeshRefinementSurface(gBox.refi_Sur));
    str_File_New.append(CreateMeshRefinementSurface(gCylin.refi_Sur));
    str_File_New.append(CreateMeshRefinementSurface(gSphere.refi_Sur));
    str_File_New.append(CreateMeshRefinementSurfaceSTL(gUserDefine.refi_Sur));
    //Add refinement surfaces cell zone
    str_File_New.append(CreateMeshRefinementSurface(gBoxCellZone.refi_Sur,1));
    str_File_New.append(CreateMeshRefinementSurface(gCylinCellZone.refi_Sur,1));
    str_File_New.append(CreateMeshRefinementSurface(gSphereCellZone.refi_Sur,1));
    str_File_New.append(CreateMeshRefinementSurfaceSTL(gUserDefineCellZone.refi_Sur,1));

    str_File_New +="}\n";
    str_File_New +="resolveFeatureAngle " + QString::number(resolveFeatureAngle) + ";\n";
    str_File_New +="\n";
    str_File_New +="refinementRegions\n";
    str_File_New +="{\n";

    //add refinementRegions
    str_File_New.append(CreateMeshRefinementRegions(gBox.refi_Reg));
    str_File_New.append(CreateMeshRefinementRegions(gCylin.refi_Reg));
    str_File_New.append(CreateMeshRefinementRegions(gSphere.refi_Reg));
    str_File_New.append(CreateMeshRefinementRegions(gUserDefine.refi_Reg));
    str_File_New.append(CreateMeshRefinementRegions(gBoxCellZone.refi_Reg));
    str_File_New.append(CreateMeshRefinementRegions(gCylinCellZone.refi_Reg));
    str_File_New.append(CreateMeshRefinementRegions(gSphereCellZone.refi_Reg));
    str_File_New.append(CreateMeshRefinementRegions(gUserDefineCellZone.refi_Reg));
    str_File_New.append(CreateMeshRefinementRegions(gBoxRegion.refi_Reg));
    str_File_New.append(CreateMeshRefinementRegions(gCylinRegion.refi_Reg));
    str_File_New.append(CreateMeshRefinementRegions(gSphereRegion.refi_Reg));

    str_File_New +="}\n";
    str_File_New +="\n";
    str_File_New +="locationInMesh (";
    str_File_New += QString::number(locationInMesh.x) +" ";
    str_File_New += QString::number(locationInMesh.y) +" ";
    str_File_New += QString::number(locationInMesh.z);
    str_File_New +=");\n";
    str_File_New +="allowFreeStandingZoneFaces false;\n";
    str_File_New +="}\n";
    str_File_New +="\n";
    return str_File_New;
}
QString Snappy_Dmesh::CreateSnapControls()
{
    QString str_File_New ="snapControls\n";
    str_File_New +="{\n";
    str_File_New +="nSolveIter 30;\n";
    str_File_New +="nSmoothPatch 3;\n";
    str_File_New +="tolerance 4.0;\n";
    str_File_New +="nRelaxIter 5;\n";
    str_File_New +="nFeatureSnapIter 20;\n";
    str_File_New +="implicitFeatureSnap true;\n";
    str_File_New +="explicitFeatureSnap true;\n";
    str_File_New +="multiRegionFeatureSnap true;\n";
    str_File_New +="}\n";
    str_File_New +="\n";
    return str_File_New;
}
QString Snappy_Dmesh::CreateAddLayersControls()
{
    QString str_File_New ="addLayersControls\n";
    str_File_New +="{\n";
    str_File_New +="layers\n";
    str_File_New +="{\n";
//Add layer
    for (int i = 0; i < add_Layers_Controls.nLayer; i++)
    {
        if(add_Layers_Controls.layers[i].nSurfaceLayers > 0)
        {
            str_File_New +="\"" + add_Layers_Controls.layers[i].name + ".*\"\n";
            str_File_New +="{\n";
            str_File_New +="nSurfaceLayers " + QString::number(add_Layers_Controls.layers[i].nSurfaceLayers) + ";\n";
            str_File_New +="}\n";
        }
    }
    str_File_New +="}\n";
    str_File_New +="\n";
    if(add_Layers_Controls.relativeSizes == true)
        str_File_New +="relativeSizes true;\n";
    else
        str_File_New +="relativeSizes false;\n";
    str_File_New +="expansionRatio " + QString::number(add_Layers_Controls.expansionRatio) + ";\n";
    str_File_New +="finalLayerThickness " + QString::number(add_Layers_Controls.finalLayerThickness) + ";\n";
    str_File_New +="minThickness " + QString::number(add_Layers_Controls.minThickness) + ";\n";
    str_File_New +="nGrow 0;\n";
    str_File_New +="featureAngle " + QString::number(add_Layers_Controls.featureAngle) + ";\n";
    str_File_New +="slipFeatureAngle " + QString::number(add_Layers_Controls.slipFeatureAngle) + ";\n";
    str_File_New +="nRelaxIter 5;\n";
    str_File_New +="nSmoothSurfaceNormals 3;\n";
    str_File_New +="nSmoothNormals 5;\n";
    str_File_New +="nSmoothThickness 10;\n";
    str_File_New +="maxFaceThicknessRatio 0.9;\n";
    str_File_New +="maxThicknessToMedialRatio 0.9;\n";
    str_File_New +="minMedianAxisAngle 90;\n";
    str_File_New +="nBufferCellsNoExtrude 0;\n";
    str_File_New +="nLayerIter 50;\n";
    str_File_New +="nRelaxedIter 20;\n";
    str_File_New +="}\n";
    str_File_New +="\n";
    return str_File_New;
}
float Snappy_Dmesh::MinIn(float a, float b, float c)
{
    if(a < b && a < c)
        return a;
    if(b < a && b < c)
        return b;
    if(c < a && c < b)
        return c;
    return a;
}
float Snappy_Dmesh::MaxIn(float a, float b, float c)
{
    if(a > b && a > c)
        return a;
    if(b > a && b > c)
        return b;
    if(c > a && c > b)
        return c;
    return a;
}
int Snappy_Dmesh::FinMaxLevel()
{
    int result = 0;
    //Find in box surface
    foreach(RefinementSurface r,gBox.refi_Sur.surfaces){
        if(result < r.lv2)
            result = r.lv2;
    }
    //Find in box surface region
    foreach(RefinementRegion r,gBox.refi_Reg.region){
            foreach(RefinementDistance rD, r.distances)
                if(result < rD.lv2)
                    result = rD.lv2;
    }
    //Find in cylinder surface
    foreach(RefinementSurface r,gCylin.refi_Sur.surfaces){
        if(result < r.lv2)
            result = r.lv2;
    }
    //Find in cylinder surface region
    foreach(RefinementRegion r,gCylin.refi_Reg.region){
        foreach(RefinementDistance rD, r.distances)
            if(result < rD.lv2)
                result = rD.lv2;
    }
    //Find in sphere surface
    foreach(RefinementSurface r,gSphere.refi_Sur.surfaces){
        if(result < r.lv2)
            result = r.lv2;
    }
    //Find in sphere surface region
    foreach(RefinementRegion r,gSphere.refi_Reg.region){
        foreach(RefinementDistance rD, r.distances)
            if(result < rD.lv2)
                result = rD.lv2;
    }
    //Find in STL surface
    foreach(RefinementSurfaceSTL r,gUserDefine.refi_Sur.surfaces){
        if(result < r.lv2)
            result = r.lv2;
        foreach(regionSTL rS,r.regionSTLs)
            if(result < rS.lv2)
                result = rS.lv2;
    }
    //Find in STL surface region
    foreach(RefinementRegion r,gUserDefine.refi_Reg.region){
        foreach(RefinementDistance rD, r.distances)
            if(result < rD.lv2)
                result = rD.lv2;
    }
    //Find in STL surface feature
    foreach(FeatureSLT r,gUserDefine.refi_Fea.feature){
            if(result < r.lv)
                result = r.lv;
    }
    //Find in box surface cell zone
    foreach(RefinementSurface r,gBoxCellZone.refi_Sur.surfaces){
        if(result < r.lv2)
            result = r.lv2;
    }
    //Find in box surface region cell zone
    foreach(RefinementRegion r,gBoxCellZone.refi_Reg.region){
        if(result < r.lv2)
            result = r.lv2;
        foreach(RefinementDistance rD, r.distances)
            if(result < rD.lv2)
                result = rD.lv2;
    }
    //Find in cylinder surface cell zone
    foreach(RefinementSurface r,gCylinCellZone.refi_Sur.surfaces){
        if(result < r.lv2)
            result = r.lv2;
    }
    //Find in cylinder surface region cell zone
    foreach(RefinementRegion r,gCylinCellZone.refi_Reg.region){
        if(result < r.lv2)
            result = r.lv2;
        foreach(RefinementDistance rD, r.distances)
            if(result < rD.lv2)
                result = rD.lv2;
    }
    //Find in sphere surface cell zone
    foreach(RefinementSurface r,gSphereCellZone.refi_Sur.surfaces){
        if(result < r.lv2)
            result = r.lv2;
    }
    //Find in sphere surface region cell zone
    foreach(RefinementRegion r,gSphereCellZone.refi_Reg.region){
        if(result < r.lv2)
            result = r.lv2;
        foreach(RefinementDistance rD, r.distances)
            if(result < rD.lv2)
                result = rD.lv2;
    }

    //Find in STL surface cell zone
    foreach(RefinementSurfaceSTL r,gUserDefineCellZone.refi_Sur.surfaces){
        if(result < r.lv2)
            result = r.lv2;
        foreach(regionSTL rS,r.regionSTLs)
            if(result < rS.lv2)
                result = rS.lv2;
    }
    //Find in STL surface region cell zone
    foreach(RefinementRegion r,gUserDefineCellZone.refi_Reg.region){
        foreach(RefinementDistance rD, r.distances)
            if(result < rD.lv2)
                result = rD.lv2;
    }
    //Find in STL surface feature cell zone
    foreach(FeatureSLT r,gUserDefineCellZone.refi_Fea.feature){
        if(result < r.lv)
            result = r.lv;
    }

    //Find in box volume region
    foreach(RefinementRegion r,gBoxRegion.refi_Reg.region){
        if(result < r.lv2)
            result = r.lv2;
    }
    //Find in cylinder volume region
    foreach(RefinementRegion r,gCylinRegion.refi_Reg.region){
        if(result < r.lv2)
            result = r.lv2;
    }
    //Find in sphere volume region
    foreach(RefinementRegion r,gSphereRegion.refi_Reg.region){
        if(result < r.lv2)
            result = r.lv2;
    }
    return result;
}

QString Snappy_Dmesh::CreateMeshQualityControls()
{
    QString str_File_New = "meshQualityControls\n";
    str_File_New +="{\n";
    str_File_New +="maxNonOrtho 65;\n";
    str_File_New +="maxBoundarySkewness 20;\n";
    str_File_New +="maxInternalSkewness 4;\n";
    str_File_New +="maxConcave 80;\n";
    str_File_New +="minFlatness 0.5;\n";
    int maxLevel = FinMaxLevel();
    if (maxLevel == 0)
        maxLevel = 1;
    float minVol = pow(MinIn(deltaBaseMesh.x,deltaBaseMesh.y,deltaBaseMesh.z)/pow(2.0,maxLevel),3)/100;

    str_File_New +="minVol " + QString::number(minVol) + ";\n";
    str_File_New +="minTetQuality -1e30;\n";
    float minArea = pow(MinIn(deltaBaseMesh.x,deltaBaseMesh.y,deltaBaseMesh.z)/pow(2.0,maxLevel),2)/100;
    str_File_New +="minArea " + QString::number(minArea) + ";\n";
    str_File_New +="minTwist 0.02;\n";
    str_File_New +="minDeterminant 0.001;\n";
    str_File_New +="minFaceWeight 0.02;\n";
    str_File_New +="minVolRatio 0.01;\n";
    str_File_New +="minTriangleTwist -1;\n";
    str_File_New +="nSmoothScale 4;\n";
    str_File_New +="errorReduction 0.75;\n";
    str_File_New +="relaxed\n";
    str_File_New +="{\n";
    str_File_New +="maxNonOrtho 70;\n";
    str_File_New +="}\n";
    str_File_New +="}\n";
    str_File_New +="\n";
    return str_File_New;
}

QString Snappy_Dmesh::CreateMeshRefinementRegions(RefinementRegions ref)
{
    QString str_File_New = "";
    for(int i=0; i< ref.n; i++)
    {
        if(ref.region[i].n > 0)
        {
            str_File_New += ref.region[i].name + "\n";
            str_File_New += "{\n";
            str_File_New += "mode distance;\n";
            str_File_New += "levels (";
            foreach(RefinementDistance r, ref.region[i].distances) {
                str_File_New += " ("+ QString::number(r.lv1) + " "+ QString::number(r.lv2)+")";
            }
            str_File_New += " );\n";
            str_File_New +="}\n";
        }
        if(ref.region[i].lv1 > -1 && ref.region[i].lv2 > ref.region[i].lv1 && ref.region[i].mode != "distance") {
            str_File_New += ref.region[i].name + "\n";
            str_File_New += "{\n";
            str_File_New += "mode " + ref.region[i].mode + ";\n";
            str_File_New += "levels (("+ QString::number(ref.region[i].lv1)
                                                        +" "+ QString::number(ref.region[i].lv2)+"));\n";
            str_File_New +="}\n";

        }
    }
    return str_File_New;
}
QString Snappy_Dmesh::CreateMeshRefinementSurface(RefinementSurfaces ref,int isCellZone)
{
    QString str_File_New = "";
    for(int i=0; i< ref.n; i++)
    {
        if(ref.surfaces[i].lv2 > 0)
        {
            str_File_New +=ref.surfaces[i].name + "\n";
            str_File_New +="{\n";
            str_File_New +="level ("+QString::number(ref.surfaces[i].lv1)
                                                    +" "+QString::number(ref.surfaces[i].lv2)+");\n";
            if(isCellZone) {
                str_File_New +="cellZone " + ref.surfaces[i].name + ";\n";
                str_File_New +="faceZone " + ref.surfaces[i].name + ";\n";
                str_File_New +="celZoneInside inside;\n";
            }
            str_File_New +="}\n";
        }
    }
    return str_File_New;
}
QString Snappy_Dmesh::CreateMeshRefinementSurfaceSTL(RefinementSurfacesSTL ref,int isCellZone)
{
    QString str_File_New = "";
    for(int i=0; i< ref.n; i++)
    {
        QString str_File_STL = "";
        QString str_File_Regions = "";
//        bool isDefined = false;
        bool isDefinedRegion = false;

        str_File_STL +=ref.surfaces[i].name + "\n";
        str_File_STL +="{\n";
//        if(ref.surfaces[i].lv2 > 0)
//        {
            str_File_STL +="level ("+QString::number(ref.surfaces[i].lv1)
                                                    +" "+QString::number(ref.surfaces[i].lv2)+");\n";
            if(isCellZone) {
                str_File_STL +="cellZone " + ref.surfaces[i].name + ";\n";
                str_File_STL +="faceZone " + ref.surfaces[i].name + ";\n";
                str_File_STL +="celZoneInside inside;\n";
            }
//            isDefined = true;
//        }
        if(ref.surfaces[i].n > 0)
        {
            //add regions
            str_File_Regions +="regions\n";
            str_File_Regions +="{\n";
            for(int j=0; j< ref.surfaces[i].n; j++)
            {
                if(ref.surfaces[i].regionSTLs[j].lv2 > 0)
                {
                    str_File_Regions +=ref.surfaces[i].regionSTLs[j].name + "\n";
                    str_File_Regions +="{\n";
                    str_File_Regions +="level ("+QString::number(ref.surfaces[i].regionSTLs[j].lv1)
                                                            +" "+QString::number(ref.surfaces[i].regionSTLs[j].lv2)+");\n";
                    if(isCellZone) {
                        str_File_Regions +="cellZone " + ref.surfaces[i].name + ";\n";
                        str_File_Regions +="faceZone " + ref.surfaces[i].name + ";\n";
                        str_File_Regions +="celZoneInside inside;\n";
                    }
                    str_File_Regions +="}\n";
                    isDefinedRegion = true;
                }
            }
            if(isDefinedRegion)
                str_File_STL.append(str_File_Regions);
        }
        str_File_STL +="}\n";
//        if(isDefined)
        str_File_New.append(str_File_STL);
    }
    return str_File_New;
}

QString Snappy_Dmesh::FormatSnappyFile(QString value)
{
    QStringList lines = value.split("\n",QString::SkipEmptyParts);
    int i = 0;
    int lv = 0;
    int n = lines.size();
    while (i < n){
        if(lines[i].contains("}") && !lines[i].contains("{")) {
            if(i +1 < n & !lines[i + 1].contains("}"))
                lines[i].append("\n");
            lv -= 1;
        }
        for(int j = 0; j < lv; j++){
            lines[i] = "\t" + lines[i];
        }
        if(lines[i].contains("{") && !lines[i].contains("}"))
            lv += 1;
        i += 1;
    }
    return lines.join("\n");
}

bool Snappy_Dmesh::ReadSTLFile(QString path, int type)
{
    //read file

    QString file1;
    QFile file(path);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
         file1= file.readAll();
    }
    if(file1 == "")
    {
        //QMessageBox::information(this,"OpenFOAM","File is empty!");
        return false;
    }
    QStringList lines = file1.split("\n",QString::SkipEmptyParts);

    //create STL
    QString name = QFileInfo(file).fileName();

    int size_sTL =  sTL.size()+1;
    sTL.resize(size_sTL);
    sTL[size_sTL -1].n =0;
    sTL[size_sTL -1].name = name;    
    for(int i=0; i< lines.length(); i++)
    {
        if(lines[i].trimmed().left(5)=="solid")
        {            
            QString name_re = lines[i].split(" ")[1];
            sTL[size_sTL -1].n = sTL[size_sTL -1].n + 1;
            sTL[size_sTL -1].solids.resize(sTL[size_sTL -1].n);
            //Create temp Solid to clean CODE
            Solid *sTemp = &sTL[size_sTL -1].solids[sTL[size_sTL -1].n - 1];
            sTemp->n = 0;
            //Add name
            sTemp->name = name_re;
            //Add default color
            sTemp->type = type;
            if(type == 1){
                sTemp->color.x = 0.0;
                sTemp->color.y = 1.0;
                sTemp->color.z = 0.0;
            }else if(type == 2){
                sTemp->color.x = 1.0;
                sTemp->color.y = 0.862745098;
                sTemp->color.z = 0.0;
            }else{
                sTemp->color.x = 0.078431373;
                sTemp->color.y = 0.352941176;
                sTemp->color.z = 1.0;
            }
            //Loop for Normal vector and Vertexs

            while(lines[i].trimmed().left(8) != "endsolid")
            {               
                QStringList line = lines[i].split(" ",QString::SkipEmptyParts);
                if(line.size() > 0)
                {
                    //Add color if existed
                    if(line[0] == "color")
                    {
                        sTemp->color.x = line[1].toFloat();
                        sTemp->color.y = line[2].toFloat();
                        sTemp->color.z = line[3].toFloat();
                    }
                    //Add Normal vector
                    if(line[0] == "facet")
                    {
                        sTemp->n = sTemp->n+1;
                        sTemp->facets.resize(sTemp->n);
                        Facet *fTemp = &sTemp->facets[sTemp->n - 1];
                        fTemp->n = 0;
                        //Add normal vector
                        fTemp->normal.x = line[2].toFloat();
                        fTemp->normal.y = line[3].toFloat();
                        fTemp->normal.z = line[4].toFloat();
                        while(lines[i].trimmed().left(8) != "endfacet")
                        {
                            line = lines[i].split(" ",QString::SkipEmptyParts);
                            if(line[0] == "outer")
                            {
                                while(lines[i].trimmed().left(7) != "endloop")
                                {
                                    line = lines[i].split(" ",QString::SkipEmptyParts);
                                    if(line.size() > 0)
                                    {
                                        if(line[0] == "vertex")
                                        {
                                            PointDmesh vTemp;

                                            vTemp.x = line[1].toFloat();
                                            vTemp.y = line[2].toFloat();
                                            vTemp.z = line[3].toFloat();
                                            //find min max default bounding                                            
                                            FindMinMaxDefaultBounding(line[1].toFloat(), line[2].toFloat(), line[3].toFloat());

                                            fTemp->vertexs.append(vTemp);
                                            fTemp->n = fTemp->n + 1;
                                        }
                                    }
                                    i++;
                                }
                            }
                            i++;
                        }
                    }

                }
                i++;
            }
        }
    }

    //add min_Max temp to list min max;
    min_Max.name_Surface = name;
    list_Surface_Min_Max.append(min_Max);
    min_Max.name_Surface="";
    FindMinMax(list_Surface_Min_Max);
    return true;
}
void Snappy_Dmesh::Write_Snappy(QString path)
{
    path.append("/snappyHexMeshDict");

    QFile file(QDir::toNativeSeparators(path));
    file.open(QIODevice::WriteOnly);
    QString str_File_New;

    str_File_New.append(CreateHeader());
    str_File_New.append(CreateGeometry());
    str_File_New.append(CreateCastellatedMeshControls());
    str_File_New.append(CreateSnapControls());
    str_File_New.append(CreateAddLayersControls());
    str_File_New.append(CreateMeshQualityControls());

    str_File_New +="debug 0;\n";
    str_File_New +="mergeTolerance 1E-6;\n";
    str_File_New +="\n";

    str_File_New +="// ************************************************************************* //\n";

    str_File_New = FormatSnappyFile(str_File_New);
    file.write(str_File_New.toAscii().data());
    file.close();
}
void Snappy_Dmesh::Write_Topodict(QString path)
{
    path.append("/DMESH.topoSetDict");

    QFile file(QDir::toNativeSeparators(path));
    file.open(QIODevice::WriteOnly);
    QString str_File_New;

    str_File_New.append(CreateHeader());

    str_File_New.append("actions\n");
    str_File_New.append("(\n");
    foreach(GeomeBoxTypeDmesh g,gBoxCellZone.boxes) {
        str_File_New.append("{\n");
        str_File_New.append("name          " + g.name + ";\n");
        str_File_New.append("type          cellSet;\n");
        str_File_New.append("action        new;\n");
        str_File_New.append("source        zoneToCell;\n");
        str_File_New.append("sourceInfo\n");
        str_File_New.append("{\n");
        str_File_New.append("name    " + g.name + ";\n");
        str_File_New.append("}\n");
        str_File_New.append("}\n");
    }
    foreach(GeomeCylinTypeDmesh g,gCylinCellZone.cylins) {
        str_File_New.append("{\n");
        str_File_New.append("name          " + g.name + ";\n");
        str_File_New.append("type          cellSet;\n");
        str_File_New.append("action        new;\n");
        str_File_New.append("source        zoneToCell;\n");
        str_File_New.append("sourceInfo\n");
        str_File_New.append("{\n");
        str_File_New.append("name    " + g.name + ";\n");
        str_File_New.append("}\n");
        str_File_New.append("}\n");
    }
    foreach(GeomeSphereTypeDmesh g,gSphereCellZone.sphere) {
        str_File_New.append("{\n");
        str_File_New.append("name          " + g.name + ";\n");
        str_File_New.append("type          cellSet;\n");
        str_File_New.append("action        new;\n");
        str_File_New.append("source        zoneToCell;\n");
        str_File_New.append("sourceInfo\n");
        str_File_New.append("{\n");
        str_File_New.append("name    " + g.name + ";\n");
        str_File_New.append("}\n");
        str_File_New.append("}\n");
    }
    foreach(GeomeUserdefineTypeDmesh g,gUserDefineCellZone.user_Defines) {
        str_File_New.append("{\n");
        str_File_New.append("name          " + g.name + ";\n");
        str_File_New.append("type          cellSet;\n");
        str_File_New.append("action        new;\n");
        str_File_New.append("source        zoneToCell;\n");
        str_File_New.append("sourceInfo\n");
        str_File_New.append("{\n");
        str_File_New.append("name    " + g.name + ";\n");
        str_File_New.append("}\n");
        str_File_New.append("}\n");
    }
    str_File_New.append(");\n");
    str_File_New +="// ************************************************************************* //\n";

    str_File_New = FormatSnappyFile(str_File_New);
    file.write(str_File_New.toAscii().data());
    file.close();
}

bool Snappy_Dmesh::Read_Snappy(QString path)
{
    QString str_file = "";
    #if defined(Q_OS_WIN)
    {
        path = path.replace("/","\\");
    }
    #endif
    QFile file(path);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        str_file = file.readAll();

    if(str_file == "")
    {
        return false;
    }
    QStringList lines = str_file.split("\n",QString::SkipEmptyParts);

    RefinementSurfacesSTL list_SurfacesSTL;
    RefinementSurfaces list_Surfaces;
    RefinementRegions list_Regions;

    for(int i= 0; i< lines.length(); i++)
    {
        if(lines[i].contains("geometry"))
        {
            i = i +2;
            while(lines[i].trimmed() != "}")
            {

                QString type =  lines[i+2].trimmed().split(" ")[1];
                if(type == "searchableBox;")
                {
                    QString name =  lines[i].trimmed();

                    GeomeBoxTypeDmesh box;
                    box.name = name;
                    box.type = type.remove(";");

                    QString min = lines[i+3].remove("min").trimmed().remove(";");
                    min = min.remove("(");
                    min = min.remove(")");
                    QStringList list1 = min.split(" ", QString::SkipEmptyParts);
                    box.min.x = list1[0].trimmed().toFloat();
                    box.min.y = list1[1].trimmed().toFloat();
                    box.min.z = list1[2].trimmed().toFloat();

                    QString max = lines[i+4].remove("max").trimmed().remove(";");
                    max = max.remove("(");
                    max = max.remove(")");
                    QStringList list2 = max.split(" ");
                    box.max.x = list2[0].trimmed().toFloat();
                    box.max.y = list2[1].trimmed().toFloat();
                    box.max.z = list2[2].trimmed().toFloat();

                    gBox.boxes.append(box);
                    gBox.n =  gBox.boxes.size();

                    i = i + 6;
                }
                else if(type == "searchableCylinder;")
                {
                    QString name =  lines[i].trimmed();

                    GeomeCylinTypeDmesh  cyl;
                    cyl.name = name;
                    cyl.type = type.remove(";");

                    QString min = lines[i+3].remove("point1").trimmed().remove(";");
                    min = min.remove("(");
                    min = min.remove(")");
                    QStringList list1 = min.split(" ", QString::SkipEmptyParts);
                    cyl.point1.x = list1[0].trimmed().toFloat();
                    cyl.point1.y = list1[1].trimmed().toFloat();
                    cyl.point1.z = list1[2].trimmed().toFloat();

                    QString max = lines[i+4].remove("point2").trimmed().remove(";");
                    max = max.remove("(");
                    max = max.remove(")");
                    QStringList list2 = max.split(" ");
                    cyl.point2.x = list2[0].trimmed().toFloat();
                    cyl.point2.y = list2[1].trimmed().toFloat();
                    cyl.point2.z = list2[2].trimmed().toFloat();

                    cyl.radius = lines[i+5].remove("radius").trimmed().remove(";").toFloat();

                    gCylin.cylins.append(cyl);
                    gCylin.n =  gCylin.cylins.size();

                    i = i + 7;
                }
                else if(type == "searchableSphere;")
                {
                    QString name =  lines[i].trimmed();

                    GeomeSphereTypeDmesh  sph;
                    sph.name = name;
                    sph.type = type.remove(";");

                    QString min = lines[i+3].remove("centre").trimmed().remove(";");
                    min = min.remove("(");
                    min = min.remove(")");
                    QStringList list1 = min.split(" ", QString::SkipEmptyParts);
                    sph.centre.x = list1[0].toFloat();
                    sph.centre.y = list1[1].toFloat();
                    sph.centre.z = list1[2].toFloat();

                    sph.radius = lines[i+4].trimmed().split(" ", QString::SkipEmptyParts)[1].remove(";").toFloat();

                    gSphere.sphere.append(sph);
                    gSphere.n =  gSphere.sphere.size();

                    i = i + 6;
                }
                else if(type == "triSurfaceMesh;")
                {
                    QString name =  lines[i].trimmed();

                    GeomeUserdefineTypeDmesh user;
                    user.name_file= name;
                    user.type = type.remove(";");

                    user.name = lines[i+3].trimmed().remove(";").split(" ")[1];

                    gUserDefine.user_Defines.append(user);
                    gUserDefine.n =  gUserDefine.user_Defines.size();

                    i = i + 5;
                }
            }

        }
        else if(lines[i].contains("refinementSurfaces"))
        {
            i = i +2;
            while(lines[i].trimmed() != "}")
            {

                QString type =  lines[i+3].trimmed();
                if(type == "regions")      //la STL
                {
                    QString name =  lines[i].trimmed();
                    QString level = lines[i+2].trimmed().split("(")[1];
                    level = level.remove(")");
                    level = level.remove(";");
                    QStringList  levels = level.trimmed().split(" ", QString::SkipEmptyParts);

                    RefinementSurfaceSTL resur;
                    resur.name = name;
                    resur.lv1 = levels[0].toFloat();
                    resur.lv2 = levels[1].toFloat();

                    i = i+5;
                    while(!lines[i].contains("}"))//dung region
                    {
                        regionSTL reSTL;
                        reSTL.name = lines[i].trimmed();
                        QString level1 = lines[i+2].trimmed().split("(")[1];
                        level1 = level1.remove(")");
                        level1 = level1.remove(";");
                        QStringList  levels1 = level1.trimmed().split(" ", QString::SkipEmptyParts);
                        reSTL.lv1 = levels1[0].toFloat();
                        reSTL.lv2 = levels1[1].toFloat();
                        resur.regionSTLs.append(reSTL);
                        i = i+4;
                    }

                    resur.n = resur.regionSTLs.size();

                    list_SurfacesSTL.surfaces.append(resur);
                    list_SurfacesSTL.n = list_SurfacesSTL.surfaces.size();

                    i = i +2;
                }
                else
                {
                    QString name =  lines[i].trimmed();
                    QString level = lines[i+2].trimmed().split("(")[1];
                    level = level.remove(")");
                    level = level.remove(";");
                    QStringList  levels = level.trimmed().split(" ", QString::SkipEmptyParts);

                    RefinementSurface resur;
                    resur.name = name;
                    resur.lv1 = levels[0].toFloat();
                    resur.lv2 = levels[1].toFloat();

                    list_Surfaces.surfaces.append(resur);
                    list_Surfaces.n = list_Surfaces.surfaces.size();

                    i = i+4;
                }
            }
        }
        else if(lines[i].contains("refinementRegions"))
        {
            i = i +2;
            while(lines[i].trimmed() != "}")
            {
                QString name =  lines[i].trimmed();
                QString mode = lines[i+2].trimmed().split(" ", QString::SkipEmptyParts)[1];
                QString level = lines[i+3];
                level = level.remove("levels");
                level = level.remove(")");
                level = level.remove("(");
                level = level.remove(";");
                QStringList  levels = level.trimmed().split(" ", QString::SkipEmptyParts);
                RefinementRegion region;
                for(int l= 0; l<levels.size(); l = l+2)
                {
                    RefinementDistance dis;
                    dis.lv1 = levels[l].toFloat();
                    dis.lv2 = levels[l+1].toFloat();
                    region.distances.append(dis);
                }
                region.name = name;
                region.mode = mode.remove(";");

                list_Regions.region.append(region);
                list_Regions.n = list_Regions.region.size();

                i = i+5;
            }
        }
        else if(lines[i].contains("resolveFeatureAngle"))
        {
            QString angle = lines[i].trimmed().split(" ")[1];
            angle = angle.remove(";");
            resolveFeatureAngle = angle.toFloat();
        }
        else if(lines[i].contains("locationInMesh"))
        {
            QString loca = lines[i].trimmed().split("(")[1];
            loca = loca.remove(")");
            loca = loca.remove(";");
            QStringList list_loca = loca.trimmed().split(" ", QString::SkipEmptyParts);
            locationInMesh.x = list_loca[0].toFloat();
            locationInMesh.y = list_loca[1].toFloat();
            locationInMesh.z = list_loca[2].toFloat();
        }
        else if(lines[i].contains("addLayersControls"))
        {
            i = i +2;
            while(lines[i].trimmed() != "}")
            {
                if(lines[i].trimmed()== "layers")
                {
                    i = i+2;
                    while(lines[i].trimmed() !="}")
                    {
                        LayersControl lay;
                        lay.name = lines[i].trimmed().remove(".*");
                        lay.name = lay.name.remove("\"");
                        lay.nSurfaceLayers = lines[i+2].trimmed().
                                             split(" ", QString::SkipEmptyParts)[1].remove(";").toInt();
                        add_Layers_Controls.layers.append(lay);
                        add_Layers_Controls.nLayer = add_Layers_Controls.layers.size();
                        i= i+4;
                    }
                }
                else if(lines[i].trimmed().split(" ", QString::SkipEmptyParts)[0]== "expansionRatio")
                {
                    add_Layers_Controls.expansionRatio = lines[i].trimmed().
                                                         split(" ", QString::SkipEmptyParts)[1].remove(";").toFloat();
                }
                else if(lines[i].trimmed().split(" ", QString::SkipEmptyParts)[0] == "finalLayerThickness")
                {
                    add_Layers_Controls.finalLayerThickness = lines[i].trimmed().
                                                         split(" ", QString::SkipEmptyParts)[1].remove(";").toFloat();
                }
                else if(lines[i].trimmed().split(" ", QString::SkipEmptyParts)[0]== "minThickness")
                {
                    add_Layers_Controls.minThickness = lines[i].trimmed().
                                                         split(" ", QString::SkipEmptyParts)[1].remove(";").toFloat();
                }
                else if(lines[i].trimmed().split(" ", QString::SkipEmptyParts)[0]== "featureAngle")
                {
                    add_Layers_Controls.featureAngle = lines[i].trimmed().
                                                         split(" ", QString::SkipEmptyParts)[1].remove(";").toFloat();
                }
                else if(lines[i].trimmed().split(" ", QString::SkipEmptyParts)[0]== "slipFeatureAngle")
                {
                    add_Layers_Controls.slipFeatureAngle = lines[i].trimmed().
                                                         split(" ", QString::SkipEmptyParts)[1].remove(";").toFloat();
                }

                i = i+1;
            }
        }
    }
    //cap nhat region and surface
    for(int i=0; i< list_Regions.region.size(); i++)
    {
        for(int j =0; j < gBox.boxes.size(); j++)
        {
            if(list_Regions.region[i].name == gBox.boxes[j].name)
            {
                if(list_Regions.region[i].mode != "distance")
                {
                    int n = gBoxRegion.boxes.size()+1;
                    gBoxRegion.boxes.resize(n);
                    gBoxRegion.boxes[n-1].name = gBox.boxes[j].name;
                    gBoxRegion.boxes[n-1].min = gBox.boxes[j].min;
                    gBoxRegion.boxes[n-1].max = gBox.boxes[j].max;
                    gBoxRegion.boxes[n-1].type = gBox.boxes[j].type;
                    gBoxRegion.refi_Reg.region.append(list_Regions.region[i]);

                    gBox.boxes.remove(j,1);
                    j--;
                }
                else
                {
                    gBox.refi_Reg.region.append(list_Regions.region[i]);
                    gBox.refi_Reg.n = gBox.refi_Reg.region.size();
                }

            }
        }
        gBox.n = gBox.boxes.size();
        gBoxRegion.n = gBoxRegion.boxes.size();

        for(int j =0; j < gCylin.cylins.size(); j++)
        {
            if(list_Regions.region[i].name == gCylin.cylins[j].name)
            {
                if(list_Regions.region[i].mode != "distance")
                {
                    int n = gCylinRegion.cylins.size()+1;
                    gCylinRegion.cylins.resize(n);
                    gCylinRegion.cylins[n-1].name = gCylin.cylins[j].name;
                    gCylinRegion.cylins[n-1].point1 = gCylin.cylins[j].point1;
                    gCylinRegion.cylins[n-1].point2 = gCylin.cylins[j].point2;
                    gCylinRegion.cylins[n-1].radius = gCylin.cylins[j].radius;
                    gCylinRegion.cylins[n-1].type = gCylin.cylins[j].type;
                    gCylinRegion.refi_Reg.region.append(list_Regions.region[i]);

                    gCylin.cylins.remove(j,1);
                    j--;
                }
                else
                {
                    gCylin.refi_Reg.region.append(list_Regions.region[i]);
                    gCylin.refi_Reg.n = gCylin.refi_Reg.region.size();
                }

            }
        }
        gCylin.n = gCylin.cylins.size();
        gCylinRegion.n = gCylinRegion.cylins.size();

        for(int j =0; j < gSphere.sphere.size(); j++)
        {
            if(list_Regions.region[i].name == gSphere.sphere[j].name)
            {
                if(list_Regions.region[i].mode != "distance")
                {
                    int n = gSphereRegion.sphere.size()+1;
                    gSphereRegion.sphere.resize(n);
                    gSphereRegion.sphere[n-1].name = gSphere.sphere[j].name;
                    gSphereRegion.sphere[n-1].centre = gSphere.sphere[j].centre;
                    gSphereRegion.sphere[n-1].type = gSphere.sphere[j].type;
                    gSphereRegion.sphere[n-1].radius = gSphere.sphere[j].radius;
                    gSphereRegion.refi_Reg.region.append(list_Regions.region[i]);

                    gSphere.sphere.remove(j,1);
                    j--;
                }
                else
                {
                    gSphere.refi_Reg.region.append(list_Regions.region[i]);
                    gSphere.refi_Reg.n = gSphere.refi_Reg.region.size();
                }

            }
        }
        gSphere.n = gSphere.sphere.size();
        gSphereRegion.n = gSphereRegion.sphere.size();

        for(int j =0; j < gUserDefine.user_Defines.size(); j++)
        {
            if(list_Regions.region[i].name == gUserDefine.user_Defines[j].name)
            {
                gUserDefine.refi_Reg.region.append(list_Regions.region[i]);
                gUserDefine.refi_Reg.n = gUserDefine.refi_Reg.region.size();
            }
        }
    }

    //surface
    for(int i=0; i< list_Surfaces.surfaces.size(); i++)
    {
        for(int j =0; j < gBox.boxes.size(); j++)
        {
            if(list_Surfaces.surfaces[i].name == gBox.boxes[j].name)
            {
                gBox.refi_Sur.surfaces.append(list_Surfaces.surfaces[i]);
                gBox.refi_Sur.n = gBox.refi_Sur.surfaces.size();
            }
        }
        for(int j =0; j < gCylin.cylins.size(); j++)
        {
            if(list_Surfaces.surfaces[i].name == gCylin.cylins[j].name)
            {
                gCylin.refi_Sur.surfaces.append(list_Surfaces.surfaces[i]);
                gCylin.refi_Sur.n = gCylin.refi_Sur.surfaces.size();
            }
        }

        for(int j =0; j < gSphere.sphere.size(); j++)
        {
            if(list_Surfaces.surfaces[i].name == gSphere.sphere[j].name)
            {
                gSphere.refi_Sur.surfaces.append(list_Surfaces.surfaces[i]);
                gSphere.refi_Sur.n = gSphere.refi_Sur.surfaces.size();
            }
        }
    }
    for(int k =0; k < list_SurfacesSTL.surfaces.size(); k++)
    {
        gUserDefine.refi_Sur.surfaces.append(list_SurfacesSTL.surfaces[k]);
        gUserDefine.refi_Sur.n =  gUserDefine.refi_Sur.surfaces.size();
    }

    return true;
}
