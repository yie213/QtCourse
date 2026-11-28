#include "idatabase.h"

void IDatabase::ininDatabase()
{
    database=QSqlDatabase::addDatabase("QSQLITE");
    QString aFile="../../HospitalSys.db";
    database.setDatabaseName(aFile);//设置数据库名称

    if(!database.open()){
        qDebug() << "failed to open database";
    }else
        qDebug() << "open database is ok";
}

IDatabase::IDatabase(QObject *parent) : QObject(parent)
{
    ininDatabase();
}
