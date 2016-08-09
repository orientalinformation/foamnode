#include "database.h"

Database::Database()
{

}

Database::Database(QString nameDB, QString driverDB)
{
    this->connectDB(nameDB, driverDB);
}

bool Database::connectDB(QString nameDB, QString driverDB)
{
    if (driverDB == "") {
        driverDB = "QSQLITE";
    }
    this->db = QSqlDatabase::addDatabase(driverDB);
    if (driverDB == "QSQLITE") {
        this->db.setDatabaseName(nameDB);
    }
    if (!this->db.open()) {
        this->message = "Can't open database! Please check again!";
        return false;
    } else {
        this->message = "Connect successfully!";
        QStringList stl = this->db.tables(QSql::Tables);
        stl.append("tables");
        return true;
    }
}
