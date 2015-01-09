#ifndef SNAPPY_DMESH_H
#define SNAPPY_DMESH_H
#include <Core_Fuctions/block_dmesh.h>
#include <QString>
#include <math.h>
#include <QVector>
#include <qDebug>

struct Surface_Min_Max
{
    QString name_Surface;
    PointDmesh min;
    PointDmesh max;
};

struct LayersControl
{
    QString name;
    int nSurfaceLayers;
};

struct AddLayersControls
{    
    int nLayer;
    QVector<LayersControl> layers;
    bool relativeSizes;
    float expansionRatio;
    float finalLayerThickness;
    float minThickness;
    float featureAngle;
    float slipFeatureAngle;
};

struct GeomeBoxTypeDmesh
{
    QString name;
    QString type;
    PointDmesh min;
    PointDmesh max;
};
struct GeomeCylinTypeDmesh
{
    QString name;
    QString type;
    float radius;
    PointDmesh point1;
    PointDmesh point2;
};
struct GeomeSphereTypeDmesh
{
    QString name;
    QString type;
    float radius;
    PointDmesh centre;
};
struct GeomeUserdefineTypeDmesh
{
    QString name_file;
    QString type;
    QString name;
    QString direction;
};
struct regionSTL
{
    QString name;
    int lv1;
    int lv2;
};
struct RefinementSurfaceSTL
{
    QString name;
    int lv1;
    int lv2;
    int n;
    QVector<regionSTL> regionSTLs;
};
struct FeatureSLT
{
    QString name;
    float angle;
    float lv;
};
struct RefinementFeaturesSTL
{
    int n;
    QVector<FeatureSLT> feature;
};

struct RefinementSurface
{
    QString name;
    int lv1;
    int lv2;
};
struct RefinementDistance
{
    float lv1;
    int lv2;
};

struct RefinementRegion
{
    QString name;
    QString mode;
    int lv1;
    int lv2;
    int n;
    QVector <RefinementDistance> distances;
};
struct RefinementSurfaces
{
    int n;
    QVector<RefinementSurface> surfaces;
};
struct RefinementSurfacesSTL
{
    int n;
    QVector<RefinementSurfaceSTL> surfaces;
};
struct RefinementRegions
{
    int n;
    QVector<RefinementRegion> region;
};

// Geometry Box
struct GeomeBoxSurface
{
    int n;
    QVector<GeomeBoxTypeDmesh> boxes;
    RefinementSurfaces refi_Sur;
    RefinementRegions refi_Reg;

};
struct GeomeBoxRegion
{
    int n;
    QVector<GeomeBoxTypeDmesh> boxes;
    RefinementRegions refi_Reg;

};

// Geometry Cylinder
struct GeomeCylinderSurface
{
    int n;
    QVector<GeomeCylinTypeDmesh> cylins;
    RefinementSurfaces refi_Sur;
    RefinementRegions refi_Reg;
};
struct GeomeCylinderRegion
{
    int n;
    QVector<GeomeCylinTypeDmesh> cylins;
    RefinementRegions refi_Reg;
};

// Geometry Sphere
struct GeomeSphereSurface
{
    int n;
    QVector<GeomeSphereTypeDmesh> sphere;
    RefinementSurfaces refi_Sur;
    RefinementRegions refi_Reg;
};
struct GeomeSphereRegion
{
    int n;
    QVector<GeomeSphereTypeDmesh> sphere;
    RefinementRegions refi_Reg;
};

// Geometry UserDefine (STL)
struct GeomeUserDefine
{
    int n;
    QVector<GeomeUserdefineTypeDmesh> user_Defines;
    RefinementSurfacesSTL refi_Sur;
    RefinementRegions refi_Reg;
    RefinementFeaturesSTL refi_Fea;
};

struct Facet
{
    PointDmesh normal;
    int n;
    QVector<PointDmesh> vertexs;
};

struct Solid
{
    QString name;
    PointDmesh color;

    int n;
    QVector<Facet> facets;

};

struct STL
{
    QString name;
    QString location;
    int n;
    QVector<Solid> solids;

};
struct DefaultBounding
{
    PointDmesh minBouDef;
    PointDmesh maxBouDef;
};

class Snappy_Dmesh
{
public:
    Snappy_Dmesh();

    QVector<STL> sTL;
    //point location in mesh
    PointDmesh locationInMesh;
    float resolveFeatureAngle;
    //declare general surface
    AddLayersControls add_Layers_Controls;

    //Geometry Surface
    GeomeBoxSurface gBox;
    GeomeCylinderSurface gCylin;
    GeomeSphereSurface gSphere;
    GeomeUserDefine gUserDefine;

    //Geometry Cellzone
    GeomeBoxSurface gBoxCellZone;
    GeomeCylinderSurface gCylinCellZone;
    GeomeSphereSurface gSphereCellZone;
    GeomeUserDefine gUserDefineCellZone;

    //Geometry Region
    GeomeBoxRegion gBoxRegion;
    GeomeCylinderRegion gCylinRegion;
    GeomeSphereRegion gSphereRegion;

    //Default Bounding

    DefaultBounding defaultBounding;

    void Write_Snappy(QString path);
    void Write_Topodict(QString path);
    bool Read_Snappy(QString path);
    bool ReadSTLFile(QString path_car);

    void FindMinMaxDefaultBounding(float x, float y, float z);
    void FindMinMax(QList<Surface_Min_Max> l);

    QVector <float *> points;
    QVector <int *> faces;
    QVector <int *> facezones;
    QVector <int> facetype;
    QStringList facename;

    PointDmesh deltaBaseMesh;
    //declare surface min max
    QList<Surface_Min_Max> list_Surface_Min_Max;
    Surface_Min_Max min_Max;
private:
    QString CreateHeader();
    QString CreateGeometry();
    QString CreateCastellatedMeshControls();
    QString CreateSnapControls();
    QString CreateAddLayersControls();
    QString CreateMeshQualityControls();

    QString CreateMeshRefinementRegions(RefinementRegions ref);
    QString CreateMeshRefinementSurface(RefinementSurfaces ref, int isCellZone = 0);
    QString CreateMeshRefinementSurfaceSTL(RefinementSurfacesSTL ref, int isCellZone = 0);
    QString FormatSnappyFile(QString value);
    int FinMaxLevel();
};

#endif // Snappy_Dmesh_H
