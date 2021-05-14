#ifndef SYSCHECK_H
#define SYSCHECK_H
#include "database.h"

/// current_gpu表示当前显示的GPU信息
typedef struct {
    QString product;
    QString vendor;
} current_index_t;

class SysCheck
{
public:
    SysCheck();

    /**
     * getGPUcount: 获取系统上显卡的数量和名称信息 QMYSQL
    ***/
    bool getGPUcount(void);

    bool get_base_info();
    /**
     * getGLinfo: glxinfo
    ***/
    void getGLinfo(void);
    std::vector<QString> glx_extension, gl_extension;

    /**
     * get_cpu_clock: cat /proc/cpuinfo | grep MHz
    ***/
    void get_cpu_clock(void);
    std::array<QStringList,2> cpu_clock;

    /**
     * get_cpu_temper:
     * Core 0:       +52.0°C  (high = +90.0°C, crit = +90.0°C)
     * Core 2:       +52.0°C  (high = +90.0°C, crit = +90.0°C)
    ***/
    void get_cpu_temper(void);
    std::array<QStringList,2> cpu_temper;

    /**
     * get_mem_usage:
     * cat /proc/meminfo
    MemTotal:        8056548 kB
    MemFree:          394792 kB
    ***/
    void get_mem_usage(void);
    float mem_usage;

    /**
     * get_cpu_power:
     * Core 0:       +52.0°C  (high = +90.0°C, crit = +90.0°C)
     * Core 2:       +52.0°C  (high = +90.0°C, crit = +90.0°C)
    ***/
    void get_cpu_power(void);
    QString cpu_power;

    void select_gpu(int n) { select_index = n; }

    QString get_current_product()  { return system_gpus[select_index].product; }

    QString get_current_vendor() { return system_gpus[select_index].vendor; }

    QStringList get_name_list();

    //bool get_all_info();

    QStringList name_list;

    QString current_product;

    SysCheck *sys_gpus;

    // gears
    void gears_run(void);
    float gears_score;

    // glmark2
    void glmark_run(void);
    QString glmark_score;

    /// 当前系统的GPU信息用GPUInfo存储，每一个GPU的信息用std::array<QString,GPU_INFO::MAX>存储
    std::vector<std::array<QString,GPU_INFO::MAX>> sys_gpu_data;
    std::vector<current_index_t> system_gpus;
    int select_index;
    /// GPU_counts记录当前系统中GPU的数量
    int GPU_counts;
private:

    /**
     * executeLinuxCmd: 执行Linux shell命令
    ***/
    QString executeLinuxCmd(QString strCmd);

    DataBase *db;

    QString current_vendor;
};

#endif // SYSCHECK_H
