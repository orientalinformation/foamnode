#ifndef BLOCK_DMESH_H
#define BLOCK_DMESH_H

#include <QString>
#include <QVector>
#include <QFile>
#include <QDir>
#include <QMessageBox>

struct PointDmesh
{
    float x;
    float y;
    float z;
};
struct GeneralBounding
{
    //declare blocks
    int Layers;
    PointDmesh element;
};
struct Vertices
{
    int n;
    QVector <PointDmesh> v;
};
struct FacesDmesh
{
    int n;
    QVector<int> points;
};
struct BounTypeDmesh
{
    QString name;
    QString type;
    int n;
    QVector<FacesDmesh> faces;
};
struct BoundMesh
{
    int n;
    QVector<BounTypeDmesh> bounsType;
};

class Block_Dmesh
{
public:
    Block_Dmesh();
    //declare virtices
    Vertices vertice;
    void Set(Vertices v);
    Vertices Get_Ver();
    //declare general
    GeneralBounding gen_Bounding;
    //declare boundary
    BoundMesh boundMesh;
    bool isAutomatic;
    void Set(BoundMesh b);
    BoundMesh Get_Bou();
    void Write_Block_Dmesh(QString path);
    void Write_Bounding(QString path, QStringList string);
    bool Read_Block_Dmesh(QString path);
    QString Read_Setting(QString path);
};

#endif // BLOCK_DMESH_H
