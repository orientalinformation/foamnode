#include "materialstable.h"

MaterialsTable::MaterialsTable()
{
    materialData = new Database("materials.sqlite");
}

MaterialsTable::MaterialsTable(QString name)
{
    materialData = new Database(name);
}

QList<Material> MaterialsTable::getMaterials()
{
    Material *m = new Material();
    QList<Material> ls;
    //QSqlError error = qx::dao::fetch_all(m, &materialData->db,ls);
    qx::dao::fetch_all(m,&materialData->db);
    return ls;
}

Material* MaterialsTable::getMaterialById(int id)
{
    Material *m = new Material();
    long mid = id;
    m->setid(mid);
    QStringList relation;
    relation.append("material_type_id");
//    QSqlError error = qx::dao::fetch_all(m, &materialData->db);
    QSqlError error = qx::dao::fetch_by_id_with_relation(relation, m, &materialData->db);
    this->log = error.text();
    qx::dump(m);
    qDebug()<< this->log;
    return m;
}

Material* MaterialsTable::getMaterialByName(QString name)
{
    Material *m = new Material();
    m->setfield_name(name);
    QSqlError error = qx::dao::fetch_all(m, &materialData->db);
    this->log = error.text();
    qDebug()<< this->log;
    return m;
}
void MaterialsTable::fecthAll(){

    MaterialType * mt = new MaterialType();
    const QStringList & columns = QStringList();
    QSqlError error = qx::dao::fetch_all(mt,&materialData->db,columns);
    const QStringList & relation = QStringList();
    QSqlError er = qx::dao::fetch_all_with_relation(relation,mt,&materialData->db);
    qDebug() << error;
    qDebug() << er;
}
int MaterialsTable::countAllMaterial()
{
    qx::QxSqlQuery query = qx::QxSqlQuery("");
    long m_count = qx::dao::count<Material>(query, &materialData->db);
    qDebug() << m_count;
    return m_count;
}
int MaterialsTable::countAllMaterialType(){

    qx::QxSqlQuery query =  qx::QxSqlQuery("");
    long mt_count = qx::dao::count<MaterialType>(query, &materialData->db);
    qDebug() << mt_count;
    return mt_count;
}
int MaterialsTable::countAllReactionSpecie(){

    qx::QxSqlQuery query = qx::QxSqlQuery("");
    long rs_count = qx::dao::count<ReactionSpecie>(query,&materialData->db);
    qDebug() << rs_count;
    return rs_count;
}
int MaterialsTable::countAllStoichCoefficient(){

    qx::QxSqlQuery query = qx::QxSqlQuery("");
    long sc_count = qx::dao::count<StoichCoefficient>(query,&materialData->db);
    qDebug() << sc_count;
    return sc_count;
}
bool MaterialsTable::update(Material *material)
{
    QSqlError error = qx::dao::update(material, &materialData->db);
    QString err = error.text();
    this->log = error.text();
    qDebug()<< this->log;
    if (err != "") {
        return false;
    } else {
        return true;
    }
}

bool MaterialsTable::remove(int id)
{
    Material *m = new Material();
    long mid = id;
    m->setid(mid);
    QSqlError error = qx::dao::delete_by_id(m, &materialData->db);
    QString err = error.text();
    this->log = err;
    qDebug()<< this->log;
    if (err != "") {
        return false;
    } else {
        return true;
    }
}

bool MaterialsTable::remove(QString name)
{
    Material *m = new Material();
    m->setfield_name(name);
    QSqlError error = qx::dao::delete_by_id(m, &materialData->db);
    QString err = error.text();
    this->log = err;
    qDebug()<< this->log;
    if (err != "") {
        return false;
    } else {
        return true;
    }
}
//bool MaterialsTable::removeQuery(int id){

//    qx::QxSqlQuery query = qx::QxSqlQuery("WHERE materials.id = :id");
//    query.bind(":id",QString::number(id));
//    qx::dao::delete_by_query<Material>(query,&materialData->db);
//}
bool MaterialsTable::removeAllDatabase(){

    qx::dao::delete_all<Material>(&materialData->db);

}
bool MaterialsTable::save(Material *material)
{
    QSqlError error = qx::dao::insert(material, &materialData->db);
    QString err = error.text();
    this->log = err;
    qDebug()<< this->log;
    if (err != "") {
        return false;
    } else {
        return true;
    }
}
void MaterialsTable::queryIdMaterial(){

    MaterialType * mt= new MaterialType();
    qx::QxSqlQuery query= qx::QxSqlQuery("WHERE material_types.id = :id");
    query.bind(":id",QString::number(mt->getid()));
    qx::dao::fetch_by_query(query, mt,&materialData->db);
    qx::dump(mt);
}
Material * MaterialsTable::getRelationMaterial(int id){

    Material *m = new Material();
    m->setid(id);
    QStringList relation;
    relation.append(m->relation_material_type_id());
//    relation.append(m->relation_rate_exponents());
//    relation.append(m->relation_reaction_species());
//    relation.append(m->relation_stoich_coefficients());
    qx::dao::fetch_by_id_with_relation(relation,m,&materialData->db);
    qx::dump(m);
    return m;
}



