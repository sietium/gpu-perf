#include "syscheck.h"
#include <QProcess>
#include <QRegExp>
#include <QThread>
///测试版说明、参考、nA
SysCheck::SysCheck() :
    GPU_counts(0),
    select_index(0),
    mem_usage(0.0)
{
    db = new DataBase("test_gpu_perf.db");
}

/**
 * executeLinuxCmd: 执行Linux shell命令
***/
QString SysCheck::executeLinuxCmd(QString strCmd)
{
    QProcess p;
    p.start("bash", QStringList() <<"-c" << strCmd);
    p.waitForFinished();
    QString strResult = p.readAllStandardOutput();
    return strResult;
}

/**
 * getGPUcount: 获取系统上显卡的数量和名称信息 QMYSQL
***/
bool SysCheck::getGPUcount(void)
{
    int pairs=0;
    QString strResult1 = executeLinuxCmd("lshw -c display");
    QStringList strList = strResult1.split("\n");
    for(QStringList::ConstIterator iter = strList.begin(); iter != strList.end(); ++iter)
    {
        QString str(*iter);
        if(str.contains("*-display"))
        {
            GPU_counts++;
            current_index_t tmp;
            system_gpus.push_back(tmp);
            continue;
        }
        if(str.contains("product"))
        {
            pairs = pairs + 1;
            QString temp = str.section(": ",1,1);
            if(temp == "DWC_usb3 / PCIe bridge")
            {
                QString tmp = "GenBu01";
                system_gpus[GPU_counts-1].product = tmp;
            }
            else
                system_gpus[GPU_counts-1].product = temp;
            continue;
        }
    }
    /// print GPU info
#ifdef DEBUG
    qDebug() << GPU_counts << endl;
#endif
    if(GPU_counts > 0 )
        return true;
    else
    {
        GPU_counts++;
        current_index_t tmp;
        system_gpus.push_back(tmp);
        system_gpus[GPU_counts-1].product = "NO GPU";
        return false;
    }
}

/**
 * initDB: 初始化数据库
***/
bool SysCheck::get_base_info()
{
    if( !getGPUcount() )
        return false;

    select_index = 0;
    sys_gpu_data.clear();
    current_product = system_gpus[select_index].product;

    if(!db->connect())
        return false;

    //db->createTable();
    //db->insertData();

    //selected_gpu = db->queryData(current_product, current_vendor);
    for(int i=0; i<GPU_counts; i++)
    {
        std::array<QString,GPU_INFO::MAX> temp_data = db->queryDataWithName(system_gpus[i].product);
        if(temp_data[0] != system_gpus[i].product)
        {
            QMessageBox::information(NULL, "Error", "GPU info not found in database",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            temp_data[0] = system_gpus[i].product;
            for(int i=1; i<GPU_INFO::MAX; i++)
                temp_data[i] = "Unknow";
        }
    #ifdef DEBUG
        for(int i = 0; i < INFO_NUM; i++)
        {
            qDebug() << selected_gpu[i];
        }
    #endif
        sys_gpu_data.push_back(temp_data);
    }

    for(int i=0; i<system_gpus.size(); i++)
        name_list << system_gpus[i].product;

    return true;
}

/**
 * getGLinfo: glxinfo
***/
void SysCheck::getGLinfo(void)
{
    bool get_glx_extension = false;
    bool get_gl_extension = false;

    QString strResult1 = executeLinuxCmd("glxinfo");
    QStringList strList = strResult1.split("\n");
    for(QStringList::ConstIterator iter = strList.begin(); iter != strList.end(); ++iter)
    {
        QString str(*iter);
        if(str.contains("GLX extensions:"))
        {
            get_glx_extension = true;
            continue;
        }
        if(get_glx_extension){
            if(str.contains("    GLX_"))
            {
                str = str.simplified();
                QStringList templist = str.split(",");
                for(QStringList::ConstIterator it = templist.begin(); it != templist.end(); ++it)
                {
                    QString tmp(*it);
                    tmp.remove(QRegExp("\\s"));
                    if(false == tmp.isEmpty())
                        glx_extension.push_back(*it);
                }
                continue;
            }
            else
            {
                str = str.simplified();
                get_glx_extension = false;
                continue;
            }
        }

        if(str.contains("OpenGL extensions:"))
        {
            get_gl_extension = true;
            continue;
        }
        if(get_gl_extension){
            if(str.contains("    GL_"))
            {
                str = str.simplified();
                QStringList templist = str.split(",");
                for(QStringList::ConstIterator it = templist.begin(); it != templist.end(); ++it)
                {
                    QString tmp(*it);
                    tmp = tmp.simplified();
                    if(false == tmp.isEmpty())
                        gl_extension.push_back(*it);
                }
                continue;
            }
            else
            {
                get_gl_extension = false;
                continue;
            }
        }
    }
}

/**
 * get_cpu_clock: cat /proc/cpuinfo | grep MHz
***/
void SysCheck::get_cpu_clock(void)
{
    QStringList cpu,clock;
    int cpu_num=0;
    QString strResult1 = executeLinuxCmd("cat /proc/cpuinfo | grep MHz");
    QStringList strList = strResult1.split("\n");
    for(QStringList::ConstIterator iter = strList.begin(); iter != strList.end(); ++iter)
    {
        QString str(*iter);
        if(str.contains("cpu MHz"))
        {
            cpu_num++;
            QString clk = str.right(str.indexOf(":")-1);
            clk = clk.simplified();
#ifdef DEBUG
            qDebug() << "CPU :" << clk;
#endif
            clock << clk;
            continue;
        }
    }
    if(cpu_num > 1) {
        for(int i=1; i<=cpu_num; i++)
            cpu.append("CPU" + QString::number(i) + " Clock");
    }
    else
    {
        cpu.append("CPU Clock");
        clock.append("Unknow");
    }
    cpu_clock[0] = cpu;
    cpu_clock[1] = clock;
}

/**
 * get_cpu_temper:
 * Core 0:       +52.0°C  (high = +90.0°C, crit = +90.0°C)
 * Core 2:       +52.0°C  (high = +90.0°C, crit = +90.0°C)
***/
void SysCheck::get_cpu_temper(void)
{
    QStringList cpu,temper;
    int cpu_num=0;

    QString strResult1 = executeLinuxCmd("sensors");
    QStringList strList = strResult1.split("\n");
    for(QStringList::ConstIterator iter = strList.begin(); iter != strList.end(); ++iter)
    {
        QString str(*iter);
        if(str.contains("°C"))
        {
            QString temp;
            cpu_num++;
            //QString tmp = str.left(str.indexOf("°C"));
            //QString temp = tmp.right(tmp.indexOf(":  "));
            //temp = temp.simplified();

            QStringList tempList = strResult1.split(" ");
            for(int i=tempList.size()-1; i>=0; i--)
            {
                QString tmp = tempList.at(i);
                if(tmp.contains("°C"))
                {
                    temp = tmp.left(tmp.length() - 2);
#ifdef DEBUG
                                qDebug() << str;
                                qDebug() << tmp;
                                qDebug() << "CPU temper:" << temp;
#endif
                    break;
                }
            }
            temper << temp;
            continue;
        }
    }
    if(cpu_num > 1)
    {
        for(int i=1; i<=cpu_num; i++)
            cpu << "CPU" + QString::number(i) + " Temperate";
    }
    else
    {
        cpu.append("CPU0 Temperate");
        temper.append("Unknow");
    }
    cpu_temper[0] = cpu;
    cpu_temper[1] = temper;
}

/**
 * get_cpu_temper:
 * cat /proc/meminfo
MemTotal:        8056548 kB
MemFree:          394792 kB
***/
void SysCheck::get_mem_usage(void)
{
    int total=0, free=0;
    bool is_get = false;
    QString strResult1 = executeLinuxCmd("cat /proc/meminfo");
    QStringList strList = strResult1.split("\n");
    for(QStringList::ConstIterator iter = strList.begin(); iter != strList.end(); ++iter)
    {
        QString str(*iter);
        if(str.contains("MemTotal"))
        {
            QString tmp = str.left(str.indexOf(" kB"));
            QString temp = tmp.right(tmp.indexOf(":  "));
            total = temp.toInt();
#ifdef DEBUG
            qDebug() << "MemTotal:" << temp;
            qDebug() << total;
#endif
            is_get = true;
            continue;
        }
        if(str.contains("MemFree"))
        {
            QString tmp = str.left(str.indexOf(" kB"));
            QString temp = tmp.right(tmp.indexOf(":  "));
            free = temp.toInt();
#ifdef DEBUG
            qDebug() << "MemFree:" << temp;
            qDebug() << free;
#endif
            break;
        }
    }
    if(!is_get) total = -1;
    mem_usage = 100*(total - free)/total;
#ifdef DEBUG
    qDebug() << "percent: " << mem_usage;
#endif
}

/**
 * 	Wake-up Type: Power Switch
    Power Supply State: Safe
    Number Of Power Cords: 1
        Power/Performance Control
System Power Supply
    Max Power Capacity: 75 W

***/
void SysCheck::get_cpu_power(void)
{
    bool is_get=false;
    QString strResult1 = executeLinuxCmd("pkexec dmidecode | grep Power");
    QStringList strList = strResult1.split("\n");
    for(QStringList::ConstIterator iter = strList.begin(); iter != strList.end(); ++iter)
    {
        QString str(*iter);
        if(str.contains("Max Power Capacity"))
        {
            QStringList tempList = strResult1.split(" ");
            cpu_power = tempList.at(tempList.size()-2);
#ifdef DEBUG
            qDebug() << cpu_power;
#endif
            is_get = true;
            break;
        }
    }
    if(!is_get)
        cpu_power = "Unknow";
}

/**
 * @brief get_name_list
 * @return
 */
QStringList
SysCheck::get_name_list()
{
    return name_list;
}

// timeout 60 glxgears -info
/**
 * gears_run
***/
void SysCheck::gears_run(void)
{
    gears_score = 0.0;
    QString strResult1 = executeLinuxCmd("export vblank_mode=0;timeout 10 glxgears -info -fullscreen");
    QStringList strList = strResult1.split("\n");
    for(QStringList::ConstIterator iter = strList.begin(); iter != strList.end(); ++iter)
    {
        QString str(*iter);
        if(str.contains("frames in"))
        {
            QString temp = str.mid(str.size() - 10, 6);
            float temp_score = temp.toFloat();
            if(gears_score < temp_score)
                gears_score = temp_score;
#ifdef DEBUG
            qDebug() << str;
#endif
            continue;
        }
    }

#ifdef DEBUG
    qDebug() << ave_fps;
#endif
}

/**
 * glmark_run
***/
void SysCheck::glmark_run(void)
{
    glmark_score = QString("%1").arg(0);
    QString strResult1 = executeLinuxCmd("glmark2");
    QStringList strList = strResult1.split("\n");
    for(QStringList::ConstIterator iter = strList.begin(); iter != strList.end(); ++iter)
    {
        QString str(*iter);
        if(str.contains("glmark2 Score"))
        {
            glmark_score = str.right(str.indexOf(": "));
            break;
        }
#ifdef DEBUG
    qDebug() << str;
#endif
    }
}
