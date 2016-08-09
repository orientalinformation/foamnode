#include "materials.h"
#include "material_all_include.gen.h"

Materials::Materials()
{

}

Materials::Materials(QSqlDatabase *db)
{
    this->db = db;
}
void Materials::setDatabase(QSqlDatabase *db)
{
    this->db = db;
}
Material *Materials::getById(int id)
{
    Material *m = new Material();
    m->setid(id);
    qx::dao::fetch_by_id(m,db);
    qx::dump(m);
    return m;
}

Material *Materials::getByName(QString name)
{
    Material *m = new Material();
    m->setfield_name(name);
    qx::dao::fetch_all(m,db);
    qx::dump(m);
    return m;
}

Material *Materials::getRelation(int id)
{
    Material *m = new Material();
    m->setid(id);
    QStringList relation;
    relation.append(m->relation_material_type_id());
    qx::dao::fetch_by_id_with_relation(relation,m,db);
    qx::dump(m);
    return m;
}

QList<Material> Materials::getAll()
{
  QList<Material> listMaterials;
  qx::dao::fetch_all(listMaterials,db);
  qx::dump(listMaterials);
  return listMaterials;
}

unsigned int Materials::countAll()
{
    qx::QxSqlQuery query = qx::QxSqlQuery("");
    long m_count = qx::dao::count<Material>(query,db);
    qDebug() << m_count;
    return m_count;

}

unsigned int Materials::countWithType(QString typeName)
{
    Material *m = new Material();
    qx::QxSqlQuery query= qx::QxSqlQuery("WHERE materials.id = :id");
    query.bind(":id", typeName.number(m->getid()));
    long m_count = qx::dao::count<Material>(query,db);
    qDebug() << m_count;
    return m_count;
}

bool Materials::save(Material *material)
{

    QSqlError error;
    QString err;
    if(material->getid()== NULL){

        error = qx::dao::insert(material, db);
        err = error.text();
        if (err != "") {
            return false;
        } else {
            return true;
        }
    }else{

        error = qx::dao::save(material, db);
        err = error.text();
        if (err != "") {
            return false;
        } else {
            return true;
        }

    }
}

bool Materials::removeById(int id)
{
    Material *m = new Material();
    m->setid(id);
    QSqlError error = qx::dao::delete_by_id(m,db);
    QString err = error.text();
    if(err != " "){
        return false;
    }else{
        return true;
    }
}

bool Materials::removeByName(QString name)
{
    Material *m = new Material();
    m->setfield_name(name);
    QSqlError error = qx::dao::delete_by_id(m,db);
    QString err = error.text();
    if(err != " "){
        return false;
    }else{
        return true;
    }
}

bool Materials::removeDatabase()
{
    QSqlError error = qx::dao::delete_all<Material>(db);
    QString err = error.text();
    if(err != " "){
        return false;
    }else{
        return true;
    }
}












