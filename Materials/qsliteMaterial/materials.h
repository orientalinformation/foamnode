#ifndef MATERIALS_H
#define MATERIALS_H
#include <QtSql>
#include <QTableView>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QString>
#include <QStringList>
#include "material_all_include.gen.h"



class Materials
{
public:
    Materials();
    Materials(QSqlDatabase *db);
    void setDatabase(QSqlDatabase *db);
    Material* getById(int id);
    Material* getByName(QString name);
    Material* getRelation(int id);
    QList<Material> getAll();
    unsigned int countAll();
    unsigned int countWithType(QString typeName);
    bool save(Material *material);
    bool removeById(int id);
    bool removeByName(QString name);
    bool removeDatabase();


private:
    QSqlDatabase *db;

};

#endif // MATERIALS_H
