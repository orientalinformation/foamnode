#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QStringList>
#include <QString>

class Database
{
public:
    Database();
    Database(QString nameDB, QString driverDB = NULL);
    QSqlDatabase db;
    bool connectDB(QString nameDB,QString driverDB);
    QString message;
    bool disconnectDB();
    //    QTreeWidgetItem *Cplow = new  QTreeWidgetItem(ui->treeWidget);
    //    Cplow->setText(0,tr("Cp low"));
    //     addTreeChild(Cplow, "A", "Constant", "0.9123");
    //     addTreeChild(Cplow, "B", "Constant", "0.9124");
    //     QComboBox *cb = new QComboBox();
    //     cb->addItem("Constant");
    //     cb->addItem("sutherland");
    //     ui->treeWidget->setItemWidget(Cplow,1,cb);
};

#endif // DATABASE_H
