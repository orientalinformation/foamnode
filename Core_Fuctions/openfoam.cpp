/*---------------------------------------------------------------------------*\
|                            |         DDDDDDD\   FFFFFF   MM\   /MM          |
|               DOFI         |         DD    DD   FF       MM\M M/MM          |
|          HCM.20/07/2011    |         DD    DD   FFFFF    MM  M  MM          |
|             Mr.Cloud       |         DD    DD   FF       MM     MM          |
|                            |         DDDDDDD/   FF       MM     MM          |
\*---------------------------------------------------------------------------*/
#include "openfoam.h"
#include <fstream>
using namespace std;
///////////////////////////////////  Init Value  //////////////////////////////////

float OpenFoam::xMax = 0;
float OpenFoam::xMin = 0;
float OpenFoam::yMax = 0;
float OpenFoam::yMin = 0;
float OpenFoam::zMax = 0;
float OpenFoam::zMin = 0;
vector <float*> OpenFoam::points;
vector <int*> OpenFoam::faces;
vector <int*> OpenFoam::faceZones;

QString OpenFoam::openFOAMPath = "";
QString OpenFoam::pointsPath = "";
QString OpenFoam::facesPath = "";
QString OpenFoam::boundaryPath = "";
QString OpenFoam::pPath = "";
QString OpenFoam::uPath = "";
QString OpenFoam::kPath = "";
QString OpenFoam::epsilonPath = "";
QString OpenFoam::omegaPath = "";
QString OpenFoam::qPath = "";
QString OpenFoam::alphaTPath = "";
QString OpenFoam::nutPath = "";
QString OpenFoam::mutPath = "";
QString OpenFoam::alphaSGSPath = "";
QString OpenFoam::nuSGSPath = "";
QString OpenFoam::muSGSPath = "";
QString OpenFoam::nuTildaPath = "";
QString OpenFoam::rPath = "";
QString OpenFoam::tPath = "";
QString OpenFoam::p_rghPath = "";
QString OpenFoam::kappaTPath = "";
QString OpenFoam::transportPath = "";
QString OpenFoam::fvSchemes = "";
QString OpenFoam::fvSolution = "";
QString OpenFoam::rASProperties = "";
QString OpenFoam::lESProperties = "";
QString OpenFoam::turbulenceProperties = "";
QString OpenFoam::thermophysicalProperties = "";
QString OpenFoam::gPath = "";
QString OpenFoam::controlDict = "";
QString OpenFoam::decomposeParDict = "";

QString OpenFoam::solver = "";
QString OpenFoam::turbulence = "";
QString OpenFoam::algorithm = "";
int OpenFoam::endTime = 0;
int OpenFoam::index_Moment_Force = 0;
QString OpenFoam::nb_CPU = "";
QString OpenFoam::pID = "";
QString OpenFoam::writeControl = "";
QString OpenFoam::writeInterval = "";

QString OpenFoam::referenceValues_Length = "";
QString OpenFoam::referenceValues_Area = "";
QString OpenFoam::referenceValues_VelocityMagnitude = "";
QString OpenFoam::referenceValues_Density = "";
QString OpenFoam::referenceValues_Pressure = "";

QStringList OpenFoam::faceZonesName;
QStringList OpenFoam::faceZonesType;
QList<QStringList> OpenFoam::listSolver;
QStringList OpenFoam::material;
QStringList OpenFoam::parameterRequire_Solver;
QStringList OpenFoam::parameterRequire_Turbulence;

bool OpenFoam::isOpen = false;
bool OpenFoam::isCalculating = false;
bool OpenFoam::isCompressible = false;
bool OpenFoam::isSupsonic = false;
bool OpenFoam::isSteady = false;
bool OpenFoam::isViscous = false;
bool OpenFoam::isPorous = false;
bool OpenFoam::isMultiphase = false;
bool OpenFoam::isHeatTransfer = false;
bool OpenFoam::isRAS = false;
bool OpenFoam::isLaminar = false;
bool OpenFoam::isSetTurbulenceMode = false;
bool OpenFoam::isRASOnly = false;
bool OpenFoam::isLESOnly = false;
bool OpenFoam::isSetting = false;
bool OpenFoam::isSetSolver = false;
bool OpenFoam::isSetTurbulence = false;
bool OpenFoam::isSetReferenceValues = false;
bool OpenFoam::isSetDiagram = false;
bool OpenFoam::isExportPtot = false;
bool OpenFoam::isParallel = false;

QString OpenFoam::internalField_P = "";
QString OpenFoam::internalField_X = "";
QString OpenFoam::internalField_Y = "";
QString OpenFoam::internalField_Z = "";
QString OpenFoam::internalField_K = "";
QString OpenFoam::internalField_Epsilon = "";
QString OpenFoam::internalField_Omega = "";
QString OpenFoam::internalField_Q = "";
QStringList OpenFoam::internalField_R;
QString OpenFoam::internalField_AlphaT = "";
QString OpenFoam::internalField_Nut = "";
QString OpenFoam::internalField_T = "";
QString OpenFoam::internalField_P_RGH = "";
QString OpenFoam::internalField_KappaT = "";

int OpenFoam::pressureGauge = -1;
///////////////////////////////////  Return Value  //////////////////////////////////

vector <float*> OpenFoam::Points()
{
    return points;
}
vector <int*> OpenFoam::Faces()
{
    return faces;
}
vector <int*> OpenFoam::FaceZones()
{
    return faceZones;
}

bool OpenFoam::IsOpen()
{
    return isOpen;
}
bool OpenFoam::IsCalculating()
{
    return isCalculating;
}

bool OpenFoam::IsCompressible()
{
    return isCompressible;
}
bool OpenFoam::IsSupsonic()
{
    return isSupsonic;
}
bool OpenFoam::IsSteady()
{
    return isSteady;
}
bool OpenFoam::IsViscous()
{
    return isViscous;
}
bool OpenFoam::IsPorous()
{
    return isPorous;
}
bool OpenFoam::IsMultiphase()
{
    return isMultiphase;
}
bool OpenFoam::IsHeatTransfer()
{
    return isHeatTransfer;
}
bool OpenFoam::IsRAS()
{
    return isRAS;
}
bool OpenFoam::IsLaminar()
{
    return isLaminar;
}
bool OpenFoam::IsSetTurbulenceMode()
{
    return isSetTurbulenceMode;
}
bool OpenFoam::IsRASOnly()
{
    return isRASOnly;
}
bool OpenFoam::IsLESOnly()
{
    return isLESOnly;
}
bool OpenFoam::IsSetting()
{
    return isSetting;
}
bool OpenFoam::IsSetSolver()
{
    return isSetSolver;
}
bool OpenFoam::IsSetTurbulence()
{
    return isSetTurbulence;
}
bool OpenFoam::IsSetReferenceValues()
{
    return isSetReferenceValues;
}
bool OpenFoam::IsSetDiagram()
{
    return isSetDiagram;
}
bool OpenFoam::IsExportPtot()
{
    return isExportPtot;
}
bool OpenFoam::IsParallel()
{
    return isParallel;
}

bool OpenFoam::CheckOpenFoam (QString dirPath)
{
    //Check the folder is opened by user is the Openfoam folder or not...
    QStringList allSub;
    int flag = 0;
    allSub = QDir(dirPath).entryList();
    if(allSub.size() > 2)
    {
       for ( int i = 2; i < allSub.size(); i++)
       {
            if(allSub[i] == "0" || allSub[i] == "constant" || allSub[i] == "system")
                flag = flag + 1;
       }
    }
    if(flag == 3)
        return true;
    return false;
}
bool OpenFoam::CheckBlockMesh()
{
    QString path = openFOAMPath + "/constant/polyMesh";
    int flag = 0;
    QStringList allSub;
    allSub = QDir(path).entryList();
    if(allSub.size() > 2)
    {
        for (int i = 2; i < allSub.size(); i++)
        {
            if(allSub[i] == "points" || allSub[i] == "faces" || allSub[i] == "boundary")
                flag = flag + 1;
        }
    }
    if(flag == 3)
        return true;
    return false;
}

QList<QStringList> OpenFoam::ListSolver()
{
    return listSolver;
}

QStringList OpenFoam::ParameterRequire()
{
    QStringList parameter;
    parameter.append(parameterRequire_Solver);
    parameter.append(parameterRequire_Turbulence);
    parameter.removeDuplicates();
    return parameter;
}
QStringList OpenFoam::ParameterRequire_Solver()
{
    return parameterRequire_Solver;
}
QStringList OpenFoam::ParameterRequire_Turbulence()
{
    return parameterRequire_Turbulence;
}
QString OpenFoam::Solver()
{
    return solver;
}
QStringList OpenFoam::Material()
{
    return material;
}
QString OpenFoam::Turbulence()
{
    return turbulence;
}
QString OpenFoam::Algorithm()
{
    return algorithm;
}
int OpenFoam::EndTime()
{
    return endTime;
}
int OpenFoam::Index_Moment_Force()
{
    return index_Moment_Force;
}
QString OpenFoam::NB_CPU()
{
    return nb_CPU;
}
QString OpenFoam::PID()
{
    return pID;
}
QString OpenFoam::WriteControl()
{
    return writeControl;
}
QString OpenFoam::WriteInterval()
{
    return writeInterval;
}
QString OpenFoam::OpenFOAMPath()
{
    return openFOAMPath;
}
QString OpenFoam::PointsPath()
{
    return pointsPath;
}
QString OpenFoam::BoundaryPath()
{
    return boundaryPath;
}
QString OpenFoam::FacesPath()
{
    return facesPath;
}
QString OpenFoam::PPath()
{
    return pPath;
}
QString OpenFoam::UPath()
{
    return uPath;
}
QString OpenFoam::KPath()
{
    return kPath;
}
QString OpenFoam::EpsilonPath()
{
    return epsilonPath;
}
QString OpenFoam::OmegaPath()
{
    return omegaPath;
}
QString OpenFoam::QPath()
{
    return qPath;
}
QString OpenFoam::RPath()
{
    return rPath;
}
QString OpenFoam::TPath()
{
    return tPath;
}
QString OpenFoam::P_RGHPath()
{
    return p_rghPath;
}
QString OpenFoam::KappaTPath()
{
    return kappaTPath;
}
QString OpenFoam::AlphaTPath()
{
    return alphaTPath;
}
QString OpenFoam::NutPath()
{
    return nutPath;
}
QString OpenFoam::MutPath()
{
    return mutPath;
}
QString OpenFoam::NuSGSPath()
{
    return nuSGSPath;
}
QString OpenFoam::MuSGSPath()
{
    return muSGSPath;
}
QString OpenFoam::AlphaSGSPath()
{
    return alphaSGSPath;
}
QString OpenFoam::NuTildaPath()
{
    return nuTildaPath;
}
QString OpenFoam::TransportPath()
{
    return transportPath;
}
QString OpenFoam::FvSchemes()
{
    return fvSchemes;
}
QString OpenFoam::FvSolution()
{
    return fvSolution;
}
QString OpenFoam::LESProperties()
{
    return lESProperties;
}
QString OpenFoam::RASProperties()
{
    return rASProperties;
}
QString OpenFoam::TurbulenceProperties()
{
    return turbulenceProperties;
}
QString OpenFoam::ThermophysicalProperties()
{
    return thermophysicalProperties;
}

QString OpenFoam::GPath()
{
    return gPath;
}
QString OpenFoam::ControlDict()
{
    return controlDict;
}
QString OpenFoam::DecomposeParDict()
{
    return decomposeParDict;
}
QString OpenFoam::InternalField_P()
{
    return internalField_P;
}
QString OpenFoam::InternalField_X()
{
    return internalField_X;
}
QString OpenFoam::InternalField_Y()
{
    return internalField_Y;
}
QString OpenFoam::InternalField_Z()
{
    return internalField_Z;
}
QString OpenFoam::InternalField_K()
{
    return internalField_K;
}
QString OpenFoam::InternalField_Epsilon()
{
    return internalField_Epsilon;
}
QString OpenFoam::InternalField_Omega()
{
    return internalField_Omega;
}
QString OpenFoam::InternalField_Q()
{
    return internalField_Q;
}
QStringList OpenFoam::InternalField_R()
{
    return internalField_R;
}
QString OpenFoam::InternalField_AlphaT()
{
    return internalField_AlphaT;
}
QString OpenFoam::InternalField_Nut()
{
    return internalField_Nut;
}
QString OpenFoam::InternalField_T()
{
    return internalField_T;
}
QString OpenFoam::InternalField_P_RGH()
{
    return internalField_P_RGH;
}
QString OpenFoam::InternalField_KappaT()
{
    return internalField_KappaT;
}

int OpenFoam::PressureGauge()
{
    return pressureGauge;
}

QString OpenFoam::ReferenceValues_Length()
{
    return referenceValues_Length;
}
QString OpenFoam::ReferenceValues_Area()
{
    return referenceValues_Area;
}
QString OpenFoam::ReferenceValues_VelocityMagnitude()
{
    return referenceValues_VelocityMagnitude;
}
QString OpenFoam::ReferenceValues_Density()
{
    return referenceValues_Density;
}
QString OpenFoam::ReferenceValues_Pressure()
{
    return referenceValues_Pressure;
}

///////////////////////////////////  Set Value  //////////////////////////////////
void OpenFoam::ClearValue()
{
    xMax = 0;
    xMin = 0;
    yMax = 0;
    yMin = 0;
    zMax = 0;
    zMin = 0;
    points.clear();
    faces.clear();
    faceZones.clear();

    openFOAMPath = "";
    pointsPath = "";
    facesPath = "";
    boundaryPath = "";
    pPath = "";
    uPath = "";
    kPath = "";
    epsilonPath = "";
    omegaPath = "";
    qPath = "";
    alphaTPath = "";
    nutPath = "";
    mutPath = "";
    alphaSGSPath = "";
    nuSGSPath = "";
    muSGSPath = "";
    nuTildaPath = "";
    rPath = "";
    tPath = "";
    p_rghPath = "";
    kappaTPath = "";
    transportPath = "";
    fvSchemes = "";
    fvSolution = "";
    rASProperties = "";
    lESProperties = "";
    turbulenceProperties = "";
    thermophysicalProperties = "";
    gPath = "";
    controlDict = "";
    decomposeParDict = "";

    solver = "";
    turbulence = "";
    algorithm = "";
    endTime = 0;

    faceZonesName.clear();
    faceZonesType.clear();
    listSolver.clear();
    parameterRequire_Solver.clear();
    parameterRequire_Turbulence.clear();

    isOpen = false;
    isCalculating = false;
    isCompressible = false;
    isSteady = false;
    isViscous = false;
    isPorous = false;
    isMultiphase = false;
    isHeatTransfer = false;
    isRAS = false;
    isSetTurbulenceMode = false;
    isSetting = false;
    isSetSolver = false;
    isSetTurbulence = false;
    isSetReferenceValues = false;
    isSetDiagram = false;

    internalField_P = "";
    internalField_X = "";
    internalField_Y = "";
    internalField_Z = "";
    internalField_K = "";
    internalField_Epsilon = "";
    internalField_Omega = "";
    internalField_Q = "";
    internalField_R;
    internalField_AlphaT = "";
    internalField_Nut = "";
    internalField_T = "";
    internalField_P_RGH = "";
    internalField_KappaT = "";
}

void OpenFoam::SetOpenFOAMPath(QString value)
{

    #if defined(Q_OS_WIN)
    {
        value.replace("/","\\");
        openFOAMPath = value;
        pointsPath = value + "\\constant\\polyMesh\\points";
        facesPath = value + "\\constant\\polyMesh\\faces";
        boundaryPath = value + "\\constant\\polyMesh\\boundary";
        transportPath = value + "\\constant\\transportProperties";
        rASProperties = value + "\\constant\\RASProperties";
        lESProperties = value + "\\constant\\LESProperties";
        turbulenceProperties = value + "\\constant\\turbulenceProperties";
        thermophysicalProperties = value + "\\constant\\thermophysicalProperties";
        gPath = value + "\\constant\\g";
        pPath = value + "\\0\\p";
        uPath = value + "\\0\\U";
        kPath = value + "\\0\\k";
        epsilonPath = value + "\\0\\epsilon";
        omegaPath = value + "\\0\\omega";
        qPath = value + "\\0\\Q";
        rPath = value + "\\0\\R";
        tPath = value + "\\0\\T";
        alphaTPath = value + "\\0\\alphat";
        nutPath = value + "\\0\\nut";
        mutPath = value + "\\0\\mut";
        alphaSGSPath = value + "\\0\\alphaSgs";
        nuSGSPath = value + "\\0\\nuSgs";
        muSGSPath = value + "\\0\\muSgs";
        nuTildaPath = value + "\\0\\nuTilda";
        p_rghPath = value + "\\0\\p_rgh";
        kappaTPath = value + "\\0\\kappat";
        fvSchemes = value + "\\system\\fvSchemes";
        fvSolution = value + "\\system\\fvSolution";
        controlDict = value + "\\system\\controlDict";
        decomposeParDict = value + "\\system\\decomposeParDict";
    }
    #elif defined(Q_OS_LINUX)
    {
        openFOAMPath = value;
        pointsPath = value + "/constant/polyMesh/points";
        facesPath = value + "/constant/polyMesh/faces";
        boundaryPath = value + "/constant/polyMesh/boundary";
        transportPath = value + "/constant/transportProperties";
        rASProperties = value + "/constant/RASProperties";
        lESProperties = value + "/constant/LESProperties";
        turbulenceProperties = value + "/constant/turbulenceProperties";
        thermophysicalProperties = value + "/constant/thermophysicalProperties";
        gPath = value + "/constant/g";
        pPath = value + "/0/p";
        uPath = value + "/0/U";
        kPath = value + "/0/k";
        epsilonPath = value + "/0/epsilon";
        omegaPath = value + "/0/omega";
        qPath = value + "/0/Q";
        rPath = value + "/0/R";
        tPath = value + "/0/T";
        alphaTPath = value + "/0/alphat";
        nutPath = value + "/0/nut";
        mutPath = value + "/0/mut";
        alphaSGSPath = value + "/0/alphaSgs";
        nuSGSPath = value + "/0/nuSgs";
        muSGSPath = value + "/0/muSgs";
        nuTildaPath = value + "/0/nuTilda";
        p_rghPath = value + "/0/p_rgh";
        kappaTPath = value + "/0/kappat";
        fvSchemes = value + "/system/fvSchemes";
        fvSolution = value + "/system/fvSolution";
        controlDict = value + "/system/controlDict";
        decomposeParDict = value + "/system/decomposeParDict";
    }
    #endif
}
void OpenFoam::Points(vector<float *> value)
{
    points = value;
}
void OpenFoam::Faces(vector<int *> value)
{
    faces = value;
}
void OpenFoam::FaceZones(vector<int *> value)
{
    faceZones = value;
}
void OpenFoam::SetOpenState(bool value)
{
    isOpen = value;
}
void OpenFoam::SetListSolver(QList<QStringList> value)
{
    listSolver = value;
}

void OpenFoam::SetParameterRequire(QStringList value, int type)
{
    if(type == 0)
    {
        parameterRequire_Solver.clear();
        foreach (QString temp,value)
        {
            if(!parameterRequire_Solver.contains(temp))
                parameterRequire_Solver.append(temp);
        }
    }
    else
    {
        parameterRequire_Turbulence.clear();
        foreach (QString temp,value)
        {
            if(!parameterRequire_Turbulence.contains(temp))
                parameterRequire_Turbulence.append(temp);
        }
    }
}
void OpenFoam::SetSolver(QString value)
{
    solver = value;
}
void OpenFoam::SetMaterial(QStringList value)
{
    material = value;
}
void OpenFoam::SetTurbulence(QString value)
{
    turbulence = value;
}
void OpenFoam::SetAlgorithm(QString value)
{
    algorithm = value;
}

void OpenFoam::EndTime(int value)
{
    endTime = value;
}
void OpenFoam::Index_Moment_Force(int value)
{
    index_Moment_Force = value;
}
void OpenFoam::NB_CPU(QString value)
{
    nb_CPU = value;
}
void OpenFoam::PID(QString value)
{
    pID = value;
}
void OpenFoam::WriteControl(QString value)
{
    writeControl = value;
}
void OpenFoam::WriteInterval(QString value)
{
    writeInterval = value;
}

void OpenFoam::IsCalculating(bool value)
{
    isCalculating = value;
}
void OpenFoam::IsCompressible(bool value)
{
    isCompressible = value;
}
void OpenFoam::IsSupsonic(bool value)
{
    isSupsonic = value;
}
void OpenFoam::IsSteady(bool value)
{
    isSteady = value;
}
void OpenFoam::IsViscous(bool value)
{
    isViscous = value;
}
void OpenFoam::IsPorous(bool value)
{
    isPorous = value;
}
void OpenFoam::IsMultiphase(bool value)
{
    isMultiphase = value;
}
void OpenFoam::IsHeatTransfer(bool value)
{
    isHeatTransfer = value;
}
void OpenFoam::IsRAS(bool value)
{
    isRAS = value;
}
void OpenFoam::IsLaminar(bool value)
{
    isLaminar = value;
}
void OpenFoam::IsSetTurbulenceMode(bool value)
{
    isSetTurbulenceMode = value;
}
void OpenFoam::IsRASOnly(bool value)
{
    isRASOnly = value;
}
void OpenFoam::IsLESOnly(bool value)
{
    isLESOnly = value;
}
void OpenFoam::IsSetting(bool value)
{
    isSetting = value;
}
void OpenFoam::IsSetSolver(bool value)
{
    isSetSolver = value;
}
void OpenFoam::IsSetTurbulence(bool value)
{
    isSetTurbulence = value;
}
void OpenFoam::IsSetDiagram(bool value)
{
    isSetDiagram = value;
}
void OpenFoam::IsExportPtot(bool value)
{
    isExportPtot = value;
}
void OpenFoam::IsParallel(bool value)
{
    isParallel = value;
}
void OpenFoam::InternalField_P(QString value)
{
    internalField_P = value;
}
void OpenFoam::InternalField_X(QString value)
{
    internalField_X = value;
}
void OpenFoam::InternalField_Y(QString value)
{
    internalField_Y = value;
}
void OpenFoam::InternalField_Z(QString value)
{
    internalField_Z = value;
}
void OpenFoam::InternalField_K(QString value)
{
    internalField_K = value;
}
void OpenFoam::InternalField_Epsilon(QString value)
{
    internalField_Epsilon = value;
}
void OpenFoam::InternalField_Omega(QString value)
{
    internalField_Omega = value;
}
void OpenFoam::InternalField_Q(QString value)
{
    internalField_Q = value;
}
void OpenFoam::InternalField_R(QStringList value)
{
    internalField_R = value;
}
void OpenFoam::InternalField_AlphaT(QString value)
{
    internalField_AlphaT = value;
}
void OpenFoam::InternalField_Nut(QString value)
{
    internalField_Nut = value;
}
void OpenFoam::InternalField_T(QString value)
{
    internalField_T = value;
}
void OpenFoam::InternalField_P_RGH(QString value)
{
    internalField_P_RGH = value;
}
void OpenFoam::InternalField_KappaT(QString value)
{
    internalField_KappaT = value;
}

void OpenFoam::PressureGauge(int value)
{
    pressureGauge = value;
}

void OpenFoam::RemoveFunctionOpenFoam()
{
    QString file = ReadFile(controlDict);
    QStringList lines = file.split("\n");
    if (file.contains("functions"))
    {
        while(!lines.last().contains("functions"))
            lines.removeLast();
        lines.removeLast();
        lines.append("\n// ************************************************************************* //");
    }
    file = lines.join("\n");
    Save_File(controlDict,file);
}

void OpenFoam::RemoveParameter(bool silent)
{
    QStringList parameterRequire = ParameterRequire();
    if(silent == false && parameterRequire.size() == 0)
        return;
    QStringList parameterTemp = parameterRequire;
    //Clear all parameter not in range
    QStringList allSub = QDir(OpenFoam::OpenFOAMPath() + "/0").entryList();
    while(allSub.size() > 0)
    {
        if(allSub[0] == "." || allSub[0] == "..")
        {
            allSub.removeFirst();
            continue;
        }
        if(!parameterTemp.contains(allSub[0]))
        {
            QFile::remove(OpenFoam::OpenFOAMPath() + "/0/" + allSub[0]);
        }
        allSub.removeFirst();
    }
}

void OpenFoam::CopyParameter(int type)
{
    QStringList parameterRequire;
    if(type == 0)
        parameterRequire = parameterRequire_Solver;
    else if(type == 1)
        parameterRequire = parameterRequire_Turbulence;
    if(parameterRequire.size() == 0)
        return;
    QStringList parameterTemp = parameterRequire;
    //Clear all parameter not in range
    QStringList allSub = QDir(OpenFoam::OpenFOAMPath() + "/0").entryList();
    while(allSub.size() > 0)
    {
        if(allSub[0] == "." || allSub[0] == "..")
        {
            allSub.removeFirst();
            continue;
        }
        if(parameterTemp.contains(allSub[0]))
        {
            parameterTemp.removeOne(allSub[0]);
        }
        allSub.removeFirst();
    }
    //If enought parameter then return
    if(parameterTemp.isEmpty())
        return;
    int i;
    //Prepare the boundary template
    QString boundaryFile = ReadFile(OpenFoam::BoundaryPath());
    QStringList tempBoundary = boundaryFile.split("\n",QString::SkipEmptyParts);
    //Remove header
    while(tempBoundary[0].trimmed() != "(")
        tempBoundary.removeFirst();
    tempBoundary.removeFirst();
    //Remmove footer
    while(tempBoundary[tempBoundary.size() - 1].trimmed() != ")")
        tempBoundary.removeLast();
    tempBoundary.removeLast();
    i = 0;
    //Remove contains
    while(i < tempBoundary.size())
    {
        if(tempBoundary[i].contains("type") &&
           !tempBoundary[i].contains("symmetryPlane") &&
           !tempBoundary[i].contains("cyclic") &&
           !tempBoundary[i].contains("empty"))
        {
            QStringList temp = tempBoundary[i].split(" ",QString::SkipEmptyParts);
            tempBoundary[i].replace(temp[1],"zeroGradient;");
        }
        if(tempBoundary[i].contains("nFaces") || tempBoundary[i].contains("startFace"))
        {
            tempBoundary.removeAt(i);
            continue;
        }
        i++;
    }
    QString boundaryTemp = tempBoundary.join("\n");
    tempBoundary.clear();

    //For each parameter require, copy them to 0
    foreach(QString parameter,parameterTemp)
    {
        QString path = OpenFoam::OpenFOAMPath() + "/0/" + parameter;
        QString path1 = "Data/OpenFoamDefault/0/" + parameter;
        QString file = OpenFoam::ReadFile(path1);
        //Coppy the boundary template to all file in 0 folder
        i = file.indexOf("//content");
        file.insert(i,boundaryTemp);
        file.remove("//content");
        OpenFoam::Save_File(path,file);
    }
}
void OpenFoam::FixDimention(QString boudnaryFile,QString newValue)
{
     QString file = ReadFile(boudnaryFile);
     QStringList lines = file.split("\n");
     int i = 0;
     while(!lines[i].contains("dimensions") && i < lines.size())
     {
         i++;
     }
     if(i != lines.size())
     {
         lines[i] = "dimensions      "+ newValue + ";";
         Save_File(boudnaryFile,lines.join("\n"));
     }
}

///////////////////////////////////  Utilities Functions \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

bool OpenFoam::IsNumber(QString value)
{    
    if (value == "")
        return false;
    bool result;
    double a = value.toDouble(&result);
    return result;
}
float OpenFoam::absf(float value)
{
    if(value > 0)
        return value;
    else
        return -value;
}
bool OpenFoam::CopyDir(QString src, QString dest)
{
    QDir dir(src);
        QDir dirdest(dest);
        if(!dirdest.exists())
            dirdest.mkdir(dest);
    if(!dir.isReadable()) return false;
    QFileInfoList entries = dir.entryInfoList();
    for(QList<QFileInfo>::iterator it = entries.begin(); it!=entries.end();++it)
    {
        QFileInfo &finfo = *it;
        if(finfo.fileName()=="." || finfo.fileName()=="..")
            continue;
        if(finfo.isDir())
        {
            CopyDir(finfo.filePath(),dirdest.absoluteFilePath(finfo.fileName()));
            continue;
        }
        if(finfo.isSymLink())
        {
            /* do something here */
            continue;
        }
        if(finfo.isFile() && finfo.isReadable())
        {
            QFile file(finfo.filePath());
            file.copy(dirdest.absoluteFilePath(finfo.fileName()));
        }
        else
            return false;
    }
    return true;
}
bool OpenFoam::RemoveDir(QString dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName))
    {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
        {
            if (info.isDir())
                result = RemoveDir(info.absoluteFilePath());
            else if(info.isFile())
                result = QFile::remove(info.absoluteFilePath());
            if (!result)
                return result;
        }
        result = dir.rmdir(dirName);
    }
    if(!result)
    {
        QString command;
        #if defined(Q_OS_WIN)
            command = "rd /S /Q \"" + dirName + "\"";
        #else
            command = "rm -rf " + dirName;
        #endif
        system(command.toAscii().data());
        result = true;
    }
    return result;
}

QString OpenFoam::ReadFile(QString path)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    return file.readAll();
//    ifstream t(path.toAscii().data());
//    t.seekg(0, std::ios::end);
//    size_t size = t.tellg();
//    string buffer(size, ' ');
//    t.seekg(0);
//    t.read(&buffer[0], size);
//    return QString::fromStdString(buffer);
}
void OpenFoam::Save_File(QString path, QString value)
{
    if ( path == "" )
        return;
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    file.write(value.toAscii().data());
    file.close();
}
void OpenFoam::SetEndTime(QString value)
{
    QString file = ReadFile(controlDict);
    QStringList lines = file.split("\n",QString::SkipEmptyParts);
    file.clear();
    int i = 0;
    while(i < lines.size())
    {
        if(lines[i].contains("endTime") && !lines[i].contains("stopAt"))
        {
            QStringList line = lines[i].split(" ",QString::SkipEmptyParts);
            lines[i].replace(line[1],value + ";");
            break;
        }
        i++;
    }
    file = lines.join("\n");
    Save_File(controlDict,file);
}
void OpenFoam::SetReferenceValues(QString length, QString area, QString velocity, QString density,QString pressure)
{
    referenceValues_Length = length;
    referenceValues_Area = area;
    referenceValues_VelocityMagnitude = velocity;
    referenceValues_Density = density;
    referenceValues_Pressure = pressure;
    isSetReferenceValues = true;
}

void OpenFoam::SaveCaseSetting()
{
    if(!isOpen || solver == "" || turbulence == "")
        return;
    QStringList temp;
    if(isSteady)
        temp.append("Steady");
    else
        temp.append("Unsteady");
    if(isCompressible)
        temp.append("Compressible");
    else
        temp.append("Incompressible");
    if(isSupsonic)
        temp.append("Supsonic");
    else
        temp.append("NoneSupsonic");
    if(isViscous)
        temp.append("Viscous");
    else
        temp.append("Inviscid");
    if(isPorous)
        temp.append("Porous");
    else
        temp.append("NonePorous");
    if(isMultiphase)
        temp.append("Multiphase");
    else
        temp.append("NoneMultiphase");
    if(isHeatTransfer)
        temp.append("HeatTransfer");
    else
        temp.append("NoneHeatTransfer");
    temp.append(solver);
    if(isSetTurbulenceMode)
        temp.append("RAS&LES");
    else
    {
        if(IsRASOnly())
            temp.append("OnlyRAS");
        else
            temp.append("OnlyLES");
    }
    temp.append(algorithm);
    if(isRAS && !isLaminar)
        temp.append("RAS");
    else if(!isRAS && !isLaminar)
        temp.append("LES");
    else if(isLaminar)
        temp.append("Laminar");
    temp.append(turbulence);
    temp.append(material.join("|"));
    QString parameter = parameterRequire_Solver.join("|") + "*" + parameterRequire_Turbulence.join("|");
    temp.append(parameter);
    Save_File(OpenFOAMPath() + "/DOFI",temp.join("\n"));
}
int OpenFoam::FindEndBlock(QStringList value, int begin)
{
    if(begin == 0 )
    {
        if(value[begin].trimmed() == "{")
        {

        }
        else
        {

        }
    }
    else
    {
        if(value[begin].trimmed() == "{")
        {

        }
        else
        {

        }
    }
	return -1;
}

