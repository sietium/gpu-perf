#include "database.h"
#include <QFile>

//#define DEBUG
QString DBtable("gpu_info (\
                product varchar(255),\
                vendor varchar(255),\
                bois_version varchar(255),\
                technology varchar(255),\
                die_size varchar(255),\
                release_date varchar(255),\
                transistors varchar(255),\
                bus_width varchar(255),\
                gflops varchar(255),\
                bandwidth varchar(255),\
                texture_fillrate varchar(255),\
                rops varchar(255),\
                pixel_fillrate varchar(255),\
                tmus varchar(255),\
                gles_version varchar(255),\
                shaders varchar(255),\
                gl_version varchar(255),\
                memory_size varchar(255),\
                glsl_version varchar(255),\
                memory_type varchar(255),\
                elsl_version varchar(255),\
                mem_clock varchar(255),\
                vulkan_version varchar(255),\
                gpu_clock varchar(255),\
                cl_version varchar(255),\
                max_power varchar(255),\
                cuda_version varchar(255),\
                driver_version varchar(255))");

bool
DataBase::connect()
{
#ifdef DEBUG
    qDebug() << "Available drivers:";
#endif
    QStringList drivers = QSqlDatabase::drivers();

#ifdef DEBUG
    foreach(QString driver, drivers)
       qDebug() << driver;
#endif

    //建立数据库连接
#ifdef USE_MYSQL
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    //db.setHostName("botson");    //数据库主机名
    db.setDatabaseName("test_gpu_perf");    //数据库名
    db.setUserName("root");        //数据库用户名
    db.setPassword("botson");        //数据库密码
#else
    db = QSqlDatabase::addDatabase("QSQLITE","sqlite1");
    db.setHostName("localhost");
    db.setDatabaseName(dbname_);
    db.setUserName("botson");
    db.setPassword("botson");
#endif

    if( !db.open())
    {
       qDebug() << "db.open failed.";
       return false;
    }

    //创建表
#ifdef USE_MYSQL
    db = QSqlDatabase::database("QMYSQL");
#else
    db = QSqlDatabase::database("sqlite1");
#endif

    return true;
}

bool
DataBase::createTable()
{
    QSqlQuery query(db);
    bool success = query.exec("create table " + DBtable);
    if(success)
    {
        qDebug() << QObject::tr("create table success.");
        return true;
    }
    else
    {
        qDebug() << QObject::tr("create table failed.");
        return false;
    }
}

bool
DataBase::insertData()
{
    QSqlQuery query(db);
    //插入记录
    query.exec("insert into gpu_info values('NO GPU','','','','','','','','','','','','','','','','','','','','','','','','','','','')");
    query.exec("insert into gpu_info values('Atom/Celeron/Pentium Processor x5-E8000/J3xxx/N3xxx Integrated Graphics Controller','Intel Corporation','test','test','test','test','test','test','test','test','test','test','test','test','test','test','test','test','test','test','test','test','test','test','test','Unknow','test','test')");
    
    return true;
}

bool
DataBase::insertFromFile()
{
    QSqlQuery query(db);
    QFile file("info.txt");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!file.atEnd())
        {
            QByteArray line = file.readLine();
            QString str(line);
            qDebug() << str;
           // displayString << str;
        }
        file.close();

    }

    return true;
}

QString getVendorName(std::array<QString,GPU_INFO::MAX> info)
{
    return info[0];
}

std::array<QString,GPU_INFO::MAX>
DataBase::queryDataWithName(QString product)
{
    std::array<QString,GPU_INFO::MAX> info;
    int matched_counts=0;
    QSqlQuery query(db);

    //查询记录
    query.exec("select * from gpu_info where product = '" + product + "'");
    QSqlRecord rec = query.record();

    if(rec.count() == GPU_INFO::MAX)
    {
        while(query.next())
        {
            for(int i = 0; i < rec.count(); i++)
            {
                //info.push_back(query.value(i).toString());
                info[i] = query.value(i).toString();
#ifdef DEBUG
                qDebug() << "product name: " << product;
                qDebug() << "vendor name: " << vendor;
                qDebug() << info[i];
#endif
            }
            matched_counts++;
        }
    }
#ifdef DEBUG
    qDebug() << "matched counts: " << matched_counts;
#endif
    if(matched_counts != 1)
        info[0] = QString::number(matched_counts);
    return info;
}

std::array<QString,GPU_INFO::MAX>
DataBase::queryData(QString product, QString vendor)
{
    std::array<QString,GPU_INFO::MAX> info;
    int matched_counts=0;
    QSqlQuery query(db);

    //查询记录
    query.exec("select * from gpu_info where product = '" + product + "' and vendor = '" + vendor + "'");
    QSqlRecord rec = query.record();

    if(rec.count() == GPU_INFO::MAX)
    {
        while(query.next())
        {
            for(int i = 0; i < rec.count(); i++)
            {
                //info.push_back(query.value(i).toString());
                info[i] = query.value(i).toString();
#ifdef DEBUG
                qDebug() << "product name: " << product;
                qDebug() << "vendor name: " << vendor;
                qDebug() << info[i];
#endif
            }
            matched_counts++;
        }
    }
#ifdef DEBUG
    qDebug() << "matched counts: " << matched_counts;
#endif
    if(matched_counts != 1)
        info[0] = QString::number(matched_counts);
    return info;
}

//bool
//DataBase::queryData(std::array<QString,INFO_NUM> info, QString product, QString vendor)
//{
//    int matched_counts=0;
//    QSqlQuery query(db);

//    //查询记录
//    query.exec("select * from gpu_info where product = '" + product + "' and vendor = '" + vendor + "'");
//    //query.exec("select * from gpu_info where product = '" + product + "'");
//    QSqlRecord rec = query.record();

//    if(rec.count() == INFO_NUM)
//    {
//        while(query.next())
//        {
//            for(int i = 0; i < rec.count(); i++)
//            {
//                //info.push_back(query.value(i).toString());
//                info[i] = query.value(i).toString();
//#ifdef DEBUG
//                qDebug() << "product name: " << product;
//                qDebug() << "vendor name: " << vendor;
//                qDebug() << info[i];
//#endif
//            }
//            matched_counts++;
//        }
//    }
//#ifdef DEBUG
//    qDebug() << "matched counts: " << matched_counts;
//#endif
//    if(matched_counts == 1) {
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}

bool
DataBase::updataData()
{
    QSqlQuery query(db);
    //更新记录
    query.prepare(QString("update gpu_info set product = 'GenBu01'").arg(9));
    if(!query.exec())
    {
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("update failed."));
    }

    return true;
}

bool
DataBase::deleteData()
{
    QSqlQuery query(db);
    //删除记录
    query.prepare(QString("delete from gpu_info where product = 'GenBu01'").arg(2));
    if(!query.exec())
    {
        qDebug() << "delete failed.";
        return false;
    }

    return true;
}

bool
DataBase::deleteTable()
{
    QSqlQuery query(db);
    //删除记录
    query.prepare(QString("drop table gpu_info").arg(2));
    if(!query.exec())
    {
        qDebug() << "delete failed.";
        return false;
    }

    return true;
}

void
DataBase::set_table_format(QString str) {
    table_str = str;
}
