#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlDatabase>
#include "common_def.h"

class DataBase
{
public:
    DataBase(QString dbname) :
        dbname_(dbname)
    {}

    ~DataBase() {}

    bool connect();

    bool createTable();

    bool insertData();

    bool insertFromFile();

    std::array<QString,GPU_INFO::MAX> queryData(QString product, QString vendor);
    std::array<QString,GPU_INFO::MAX> queryDataWithName(QString product);

    bool updataData();

    bool deleteData();

    bool deleteTable();

    void set_table_format(QString str);

private:
    QString table_str;
    QSqlDatabase db;
    QString dbname_;
};

#endif // DATABASE_H
