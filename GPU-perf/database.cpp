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
    query.exec("insert into gpu_info values('Radeon R5 M230 Rebrand','Advanced Micro Devices, Inc.','Unknow','28 nm','56 mm²','2014.05','690 million','64bit','544GFLOPS','14.40 GB/s','17GTexel/s','8','6.9GPixel/s','20','Unknow','320','4.6','2G','460','DDR3','Unknow','900M/1800Mbps effective','1.2','780M','1.2','Unknow','Unknow','Unknow')");
    query.exec("insert into gpu_info values('Atom/Celeron/Pentium Processor x5-E8000/J3xxx/N3xxx Integrated Graphics Controller','Intel Corporation','test','test','test','test','test','test','test','test','test','test','test','test','test','test','test','test','test','test','test','test','test','test','test','Unknow','test','test')");
    query.exec("insert into gpu_info values('Radeon R5 M230 Rebrand','Advanced Micro Devices, Inc.','Unknow','28','56','2013.05','690','64bit','528','14.4GB/s','16.5','8','6.6','20','Unknow','320','4.6','2G','460','DDR3','Unknow','900','1.2','750','1.2','Unknow','Unknow','Unknow')");
    query.exec("insert into gpu_info values('Caicos [Radeon HD 6450/7450/8450 / R5 230 OEM]','Advanced Micro Devices, Inc.','Unknow','40','67','2013.12','370','64','200','10.67GB/s','5','4','2.5','8','Unknow','160','4.4','1024MB','440','DDR3','Unknow','667','Unknow','625','1.2','19W','Unknow','Unknow')");
    query.exec("insert into gpu_info values('Radeon R5 M230','Advanced Micro Devices, Inc.','Unknow','28','56','2014.1','690','64bit','547.2','16GB/s','6.84','8','17.1','20','Unknow','320','4.6','4G','460','DDR3','Unknow','1000','1.2','789','1.2','Unknow','Unknow','Unknow')");
    query.exec("insert into gpu_info values('GenBu01','Sietium Tech','1.4.9_04','40','21*21','2018.9','N/A','64','120G','4GB/s','2GPixel/s','2','1GPixel/s','4','3','6','4.3','1GB','430','DDR3','300','1066MHz','N/A','533MHz','1','4.5','N/A','677.1341159')");
    query.exec("insert into gpu_info values('Radeon E8860','Advanced Micro Devices, Inc.','Unknow','28','123','2014.1','1500','128bit','800','72GB/s','25','16','10','40','Unknow','640','4.6','2G','460','GDDR5','Unknow','1125','1.2','575','1.2','37W','Unknow','Unknow')");
    query.exec("insert into gpu_info values('Radeon E6465','Advanced Micro Devices, Inc.','Unknow','40','67','2015.9','370','64bit','192','25.6GB/s','4.8','4','2.4','8','Unknow','160','4.4','2G','440','GDDR5','Unknow','800','Unknow','600','1.2','25W','Unknow','Unknow')");
    query.exec("insert into gpu_info values('GP102','Wuhan Digital Engineering Institute','Unknow','40','21x21','2019','Unknow','64bit','Unknow','4GB/s','2.4','Unknow','2.4','Unknow','2','Unknow','2','1G','300','DDR3','2','1333M','Unknow','540M','Unknow','7','Unknow','Unknow')");
    query.exec("insert into gpu_info values('GP101','Wuhan Digital Engineering Institute','Unknow','40','21x21','2019','Unknow','64bit','Unknow','4GB/s','2.4','Unknow','2.4','Unknow','2','Unknow','2','1G','300','DDR3','2','1333M','Unknow','540M','Unknow','Unknow','Unknow','Unknow')");
    query.exec("insert into gpu_info values('G2002R2','Matrox Electronics SYstem Ltd .','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow')");
    query.exec("insert into gpu_info values('JM5400','JingJiaMicro, Ltd.','Unknow','65','37.5x37.5','2014.4','Unknow','32bit','160','12.8GB/s','Unknow','Unknow','2.2G','Unknow','Unknow','Unknow','Unknow','1G','Unknow','DDR3','Unknow','800','Unknow','Unknow','Unknow','6','Unknow','Unknow')");
    query.exec("insert into gpu_info values('JM7200','JingJiaMicro, Ltd.','Unknow','28','40x40','2018.8','Unknow','64bit','500','Unknow','Unknow','Unknow','5.2G','Unknow','Unknow','Unknow','1.5/2.0','4G','Unknow','DDR3','Unknow','1066M','Unknow','1G','Unknow','7~15','Unknow','Unknow')");
    query.exec("insert into gpu_info values('JARI G12','中船重工716','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','Unknow','3','Unknow','4.5','Unknow','Unknow','Unknow','3','Unknow','Unknow','Unknow','2','Unknow','Unknow','Unknow')");
    query.exec("insert into gpu_info values('GT640','NVIDIA Corporation','Unknow','28','118 mm²','Jun 5th, 2012.6','1270 million','128 bit','692.7 GFLOPS','28.51 GB/s','28.86 GTexel/s','16','7.216 GPixel/s','32','Unknow','384','4.6','2 GB','Unknow','DDR3','Unknow','891 MHz','1.1','902 MHz','1.2','65 W','Unknow','Unknow')");
    query.exec("insert into gpu_info values('GTX1050','NVIDIA Corporation','Unknow','14','132 mm²','2016.10','3300 million','128 bit','1.862 TFLOPS','112.1 GB/s','58.20 GTexel/s','32','46.56 GPixel/s','40','Unknow','640','4.6','2 GB','Unknow','GDDR5','Unknow','1752 MHz','1.2','1354 MHz','1.2','75 W','Unknow','Unknow')");
    query.exec("insert into gpu_info values('GK208B [GeForce GT 710]','NVIDIA Corporation','Unknow','40','79 mm²','2016.1','292 million','64 bit','155.5 GFLOPS','14.37 GB/s','6.480 GTexel/s','4','1.620 GPixel/s','8','Unknow','48','4.6','1024 MB','460','DDR3','Unknow','898 MHz','N/A','810 MHz','1.1','29 W','Unknow','Unknow')");
    query.exec("insert into gpu_info values('Caicos PRO [Radeon HD 7450]','Advanced Micro Devices, Inc.','Unknow','40','67 mm²','2012.1','370 million','64 bit','200.0 GFLOPS','8.528 GB/s','5.000 GTexel/s','4','2.500 GPixel/s','8','Unknow','160','4.4','512 MB','440','GDDR3','Unknow','533 MHz','N/A','625 MHz','1.2','18 W','Unknow','Unknow')");
    query.exec("insert into gpu_info values('R5 240 OEM','Advanced Micro Devices, Inc.','Unknow','28 nm','77 mm²','2013.11','950 million','64 bit','599.0 GFLOPS','14.40 GB/s','18.72 GTexel/s','8','6.240 GPixel/s','24','Unknow','384','4.6','2 GB','460','DDR3','Unknow','900 MHz','1.2','730 MHz','1.2','50 W','Unknow','Unknow')");
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
