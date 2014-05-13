/*---------------------------------------------------------------------------*\
|                            |         DDDDDDD\   FFFFFF   MM\   /MM          |
|               DOFI         |         DD    DD   FF       MM\M M/MM          |
|          HCM.20/07/2011    |         DD    DD   FFFFF    MM  M  MM          |
|             Mr.Cloud       |         DD    DD   FF       MM     MM          |
|                            |         DDDDDDD/   FF       MM     MM          |
\*---------------------------------------------------------------------------*/

#ifndef OPENFOAM_H
#define OPENFOAM_H
#include <vector>
#include <QString>
#include <QStringList>
#include <QFileDialog>

using namespace std;

class OpenFoam
{
public:
    OpenFoam();    

    //var of OpenGL Function
    static float xMax;  //var of OpenGL to centred the object
    static float xMin; //var of OpenGL to centred the object
    static float yMax;  //var of OpenGL to centred the object
    static float yMin; //var of OpenGL to centred the object
    static float zMax;  //var of OpenGL to centred the object
    static float zMin; //var of OpenGL to centred the object
    static QStringList faceZonesName;
    static QStringList faceZonesType;
    static vector <float*> Points();
    static vector <int*> Faces();
    static vector <int*> FaceZones();
    static void Points(vector <float*> value);
    static void Faces(vector <int*> value);
    static void FaceZones(vector <int*> value);

    static void SetOpenFOAMPath(QString value);
    static void SetOpenState(bool value);
    static void SetSolver(QString value);
    static QString Solver();
    static void SetAlgorithm(QString value);
    static QString Algorithm();
    static void SetTurbulence(QString value);
    static QString Turbulence();
    static int EndTime();
    static void EndTime(int value);
    static void SetParameterRequire(QStringList value,int type = 0);
    static void SetListSolver(QList<QStringList> value);
    static QList<QStringList> ListSolver();
    static QStringList Material();
    static void SetMaterial(QStringList value);
    static QStringList Material_Propertives();
    static QStringList ParameterRequire();
    static QStringList ParameterRequire_Solver();
    static QStringList ParameterRequire_Turbulence();

    static QString OpenFOAMPath();
    static QString PointsPath();
    static QString FacesPath();
    static QString BoundaryPath();
    static QString PPath();
    static QString UPath();
    static QString KPath();
    static QString EpsilonPath();
    static QString OmegaPath();
    static QString QPath();
    static QString AlphaTPath();
    static QString AlphaSGSPath();
    static QString NutPath();
    static QString MutPath();
    static QString NuSGSPath();
    static QString MuSGSPath();
    static QString NuTildaPath();
    static QString RPath();
    static QString TPath();
    static QString P_RGHPath();
    static QString KappaTPath();
    static QString TransportPath();
    static QString FvSchemes();
    static QString FvSolution();
    static QString RASProperties();
    static QString LESProperties();
    static QString TurbulenceProperties();
    static QString ThermophysicalProperties();
    static QString GPath();
    static QString ControlDict();
    static QString DecomposeParDict();

    static bool IsOpen();
    static bool IsCalculating();
    static bool IsCompressible();
    static bool IsSupsonic();
    static bool IsSteady();
    static bool IsViscous();
    static bool IsPorous();
    static bool IsMultiphase();
    static bool IsHeatTransfer();
    static bool IsRAS();
    static bool IsLaminar();
    static bool IsSetTurbulenceMode();
    static bool IsRASOnly();
    static bool IsLESOnly();
    static bool IsSetting();
    static bool IsSetSolver();
    static bool IsSetTurbulence();

    static void IsCalculating(bool value);
    static void IsCompressible(bool value);
    static void IsSupsonic(bool value);
    static void IsSteady(bool value);
    static void IsViscous(bool value);
    static void IsPorous(bool value);
    static void IsMultiphase(bool value);
    static void IsHeatTransfer(bool value);
    static void IsRAS(bool value);
    static void IsLaminar(bool value);
    static void IsSetTurbulenceMode(bool value);
    static void IsRASOnly(bool value);
    static void IsLESOnly(bool value);
    static void IsSetting(bool value);
    static void IsSetSolver(bool value);
    static void IsSetTurbulence(bool value);
    static bool IsSetDiagram();
    static void IsSetDiagram(bool value);
    static bool IsSetReferenceValues();
    static void IsExportPtot(bool value);
    static bool IsExportPtot();
    static void IsParallel(bool value);
    static bool IsParallel();

    static QString InternalField_P();
    static QString InternalField_X();
    static QString InternalField_Y();
    static QString InternalField_Z();
    static QString InternalField_K();
    static QString InternalField_Epsilon();
    static QString InternalField_Omega();
    static QString InternalField_Q();
    static QStringList InternalField_R();
    static QString InternalField_AlphaT();
    static QString InternalField_Nut();
    static QString InternalField_T();
    static QString InternalField_P_RGH();
    static QString InternalField_KappaT();
    static QString ReferenceValues_Length();
    static QString ReferenceValues_Area();
    static QString ReferenceValues_VelocityMagnitude();
    static QString ReferenceValues_Density();
    static QString ReferenceValues_Pressure();

    static int PressureGauge();

    static void InternalField_P(QString value);
    static void InternalField_X(QString value);
    static void InternalField_Y(QString value);
    static void InternalField_Z(QString value);
    static void InternalField_K(QString value);
    static void InternalField_Epsilon(QString value);
    static void InternalField_Omega(QString value);
    static void InternalField_Q(QString value);
    static void InternalField_R(QStringList value);
    static void InternalField_AlphaT(QString value);
    static void InternalField_Nut(QString value);
    static void InternalField_T(QString value);
    static void InternalField_P_RGH(QString value);
    static void InternalField_KappaT(QString value);

    static void PressureGauge(int value);

    static bool CheckOpenFoam (QString dirPath);
    static bool CheckBlockMesh();
    static void CopyParameter(int type);
    static void FixDimention(QString boudnaryFile,QString newValue);
    static void RemoveFunctionOpenFoam();
    static void RemoveParameter(bool silent = false);
    static void ClearValue();
    static void SetEndTime(QString value);
    static void SetReferenceValues(QString length,QString area,QString velocity,QString density,QString pressure);

    static QString ReadFile(QString path);
    static void Save_File(QString path,QString value);
    static bool IsNumber(QString value);
    static float absf(float value);
    static bool CopyDir(QString src,QString dest);
    static bool RemoveDir(QString dirName);
    static void SaveCaseSetting();
    static int FindEndBlock(QStringList value,int begin = 0);
    static void Index_Moment_Force(int value);
    static int Index_Moment_Force();
    static void NB_CPU(QString value);
    static QString NB_CPU();
    static void PID(QString value);
    static QString PID();
    static void WriteControl(QString value);
    static QString WriteControl();
    static void WriteInterval(QString value);
    static QString WriteInterval();

private:

    static vector <float*> points;
    static vector <int*> faces;
    static vector <int*> faceZones;

    static QString openFOAMPath;
    static QString pointsPath;
    static QString facesPath;
    static QString boundaryPath;
    static QString pPath;
    static QString uPath;
    static QString kPath;
    static QString epsilonPath;
    static QString omegaPath;
    static QString qPath;
    static QString alphaTPath;
    static QString nutPath;
    static QString mutPath;
    static QString alphaSGSPath;
    static QString nuSGSPath;
    static QString muSGSPath;
    static QString nuTildaPath;
    static QString rPath;
    static QString tPath;
    static QString p_rghPath;
    static QString kappaTPath;
    static QString transportPath;
    static QString fvSchemes;
    static QString fvSolution;
    static QString rASProperties;
    static QString lESProperties;
    static QString turbulenceProperties;
    static QString thermophysicalProperties;
    static QString gPath;
    static QString controlDict;
    static QString decomposeParDict;

    static bool isOpen;
    static bool isCalculating;
    static bool isCompressible;
    static bool isSupsonic;
    static bool isSteady;
    static bool isViscous;
    static bool isPorous;
    static bool isMultiphase;
    static bool isHeatTransfer;
    static bool isSetTurbulenceMode;
    static bool isRASOnly;
    static bool isLESOnly;
    static bool isTurbulenceMode;
    static bool isRAS;
    static bool isLaminar;
    static bool isSetting;
    static bool isSetSolver;
    static bool isSetTurbulence;

    static QString internalField_P;
    static QString internalField_X;
    static QString internalField_Y;
    static QString internalField_Z;
    static QString internalField_K;
    static QString internalField_Epsilon;
    static QString internalField_Omega;
    static QString internalField_Q;
    static QStringList internalField_R;
    static QString internalField_AlphaT;
    static QString internalField_Nut;
    static QString internalField_T;
    static QString internalField_P_RGH;
    static QString internalField_KappaT;

    static QString referenceValues_Length;
    static QString referenceValues_Area;
    static QString referenceValues_VelocityMagnitude;
    static QString referenceValues_Density;
    static QString referenceValues_Pressure;
    static bool isSetReferenceValues;
    static bool isSetDiagram;
    static bool isExportPtot;
    static bool isParallel;

    static QString solver;
    static QStringList material;
    static QString turbulence;
    static QString algorithm;
    static QString nb_CPU;
    static QString pID;
    static QString writeInterval;
    static QString writeControl;
    static int index_Moment_Force;
    static int endTime;
    static int pressureGauge;
    static QList<QStringList> listSolver;
    static QStringList parameterRequire_Solver;
    static QStringList parameterRequire_Turbulence;
};

#endif // OPENFOAM_H
