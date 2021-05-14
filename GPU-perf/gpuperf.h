#ifndef gpuperf_H
#define gpuperf_H

#include <QWidget>
#include <qmap.h>
#include "common_def.h"
#include "syscheck.h"
#include <QTimer>

namespace Ui {
class GPUPerf;
}

class GPUPerf : public QWidget
{
    Q_OBJECT

public:
    explicit GPUPerf(QWidget *parent = 0);

    ~GPUPerf();

    void initLogoMap();

private slots:
    void on_pushButton_close_clicked();

    void on_pushButton_lookup_clicked();

    void on_pushButton_submit_clicked();

    void on_gpu_selected_currentIndexChanged(const QString &arg1);

    void on_pushButton_Feedback_clicked();

    void on_pushButton_Extension_clicked();

    void on_pushButton_moitor_clicked();

    void on_pushButton_Info_clicked();

    void on_comboBox_CPUclock_currentIndexChanged(const QString &arg1);

    void on_comboBox_CPUtemper_currentIndexChanged(const QString &arg1);

    void update_monitor();

    void on_pushBtn_gears_clicked();

    //void on_pushBtn_glmark_clicked();

private:
    Ui::GPUPerf *ui;
    void setinfo(std::array<QString,GPU_INFO::MAX> info);

    bool setlogo(QString vendor);

    void setCPUClock(QString Value);
    void setCPUTemper(QString Value);

    QMap<QString,QString> logo_map;

    SysCheck *sys_gpus;

    QTimer *myTimer;
};

#endif // GPUPerf_H
