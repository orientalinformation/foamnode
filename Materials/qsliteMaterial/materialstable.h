#ifndef MATERIALSTABLE_H
#define MATERIALSTABLE_H

#include "material_all_include.gen.h"
#include "database.h"

#include <QtCore>
#include <QSqlError>
#include <QStringList>


class MaterialsTable
{
public:
    MaterialsTable();
    MaterialsTable(QString name);
    QList<Material> getMaterials();
    Material* getMaterialById(int id);
    Material* getMaterialByName(QString name);
    Material* getRelationMaterial(int id);
    int countAllMaterial();
    int countAllMaterialType();
    int countAllReactionSpecie();
    int countAllStoichCoefficient();
    bool save(Material *material);
    bool update(Material *material);
    bool remove(int id);
    bool remove(QString name);
    bool removeAllDatabase();
    bool removeAllTable();
    void fecthAll();
    void queryIdMaterial();



 private:
    QString log;
    Database *materialData;
};

#endif // MATERIALSTABLE_H
