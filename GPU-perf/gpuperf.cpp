#include "gpuperf.h"
#include "ui_gpuperf.h"
#include "smtp.h"
#include <QStandardItemModel>
#include <QDesktopServices>
#include <QUrl>
#include <QRegExpValidator>

GPUPerf::GPUPerf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GPUPerf)
{
    this->setWindowFlags(Qt::Dialog);
    this->setFixedSize(620,600);
    //ui->textEdit_feedback_commit->setFixedSize(410, 300);
    //this->setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint); setFixedSize(this->width(), this->height());
    //this->setWindowFlags(Qt::FramelessWindowHint);
    //this->setWindowFlags(Qt::Nor);
    sys_gpus = new SysCheck();
    sys_gpus->get_base_info();
    sys_gpus->getGLinfo();
    initLogoMap();
    ui->setupUi(this);
    //ui->gpu_selected->addItems(sys_gpus->name_list);

    sys_gpus->get_cpu_clock();
    ui->comboBox_CPUclock->addItems(sys_gpus->cpu_clock[0]);
    ui->progressBar_cpu_clock->setMaximum(1024*4);

    sys_gpus->get_cpu_temper();
    ui->comboBox_CPUtemper->addItems(sys_gpus->cpu_temper[0]);
    ui->progressBar_cpu_temper->setMaximum(200);

    ui->progressBar_cpu_clock->setStyleSheet("QProgressBar{background:white;} QProgressBar::chunk{background:blue}");
    ui->progressBar_cpu_powerr->setStyleSheet("QProgressBar{background:white;} QProgressBar::chunk{background:blue}");
    ui->progressBar_cpu_temper->setStyleSheet("QProgressBar{background:white;} QProgressBar::chunk{background:blue}");
    //ui->progressBar_gpu_clock->setStyleSheet("QProgressBar{background:white;} QProgressBar::chunk{background:blue}");
    //ui->progressBar_gpu_power->setStyleSheet("QProgressBar{background:white;} QProgressBar::chunk{background:blue}");
    //ui->progressBar_gpu_temper->setStyleSheet("QProgressBar{background:white;} QProgressBar::chunk{background:blue}");
    ui->progressBar_mem_usage->setStyleSheet("QProgressBar{background:white;} QProgressBar::chunk{background:blue}");

    ui->lineEdit_feedback_name->setMaxLength(50);
    ui->lineEdit_feedback_email->setMaxLength(30);
    //ui->textEdit_feedback_commit->setMaximumHeight(20);
    //ui->textEdit_feedback_commit->setMaximumWidth(50);

    //cpu power
    {
        sys_gpus->get_cpu_power();
        ui->progressBar_cpu_powerr->setMaximum(200);
        if(sys_gpus->cpu_power == "Unknow")
        {
            ui->lineEdit_cpu_power->setText(sys_gpus->cpu_power);
            ui->progressBar_cpu_powerr->setValue(0);
        }
        else
        {
            ui->lineEdit_cpu_power->setText(sys_gpus->cpu_power + "W");
            ui->progressBar_cpu_powerr->setValue(sys_gpus->cpu_power.toInt());
        }
    }

    myTimer = new QTimer(this);
    connect(myTimer, SIGNAL(timeout()), this, SLOT(update_monitor()));

    //init info page
    //
    QStringList gpu_list;
    for(int i=0; i<sys_gpus->system_gpus.size(); i++)
        gpu_list.append(sys_gpus->system_gpus[i].product);
    ui->gpu_selected->addItems(gpu_list);

    //init info page
    setinfo(sys_gpus->sys_gpu_data[sys_gpus->select_index]);
}

GPUPerf::~GPUPerf()
{
    logo_map.clear();
    delete ui;
}

/**
 * @brief setinfo set the content in info page
 * @param info: current gpu info to display
 */
void GPUPerf::setinfo(std::array<QString,GPU_INFO::MAX> info)
{
    QImage img;
    ui->product->setText(info[GPU_INFO::PRODUCT]);
    ui->vendor->setText(info[GPU_INFO::VENDOR]);
    ui->bois_version->setText(info[GPU_INFO::BOIS_VERSION]);
    ui->technology->setText(info[GPU_INFO::TECHNOLOGY]);
    ui->die_size->setText(info[GPU_INFO::DIE_SIZE]);
    ui->release_date->setText(info[GPU_INFO::RELEASE_DATE]);
    ui->transistors->setText(info[GPU_INFO::TRANSISTORS]);
    ui->bus_width->setText(info[GPU_INFO::BUS_WIDTH]);
    ui->gflops->setText(info[GPU_INFO::GFLOPS]);
    ui->bandwidth->setText(info[GPU_INFO::BANDWIDTH]);
    ui->texture_fillrate->setText(info[GPU_INFO::TEXTURE_FILLRATE]);
    ui->rops->setText(info[GPU_INFO::ROPS]);
    ui->pixel_fillrate->setText(info[GPU_INFO::PIXEL_FILLRATE]);
    ui->tmus->setText(info[GPU_INFO::TMUS]);
    ui->gles_version->setText(info[GPU_INFO::GLES_VERSION]);
    ui->shaders->setText(info[GPU_INFO::SHADERS]);
    ui->gl_version->setText(info[GPU_INFO::OGL_VERSION]);
    ui->memory_size->setText(info[GPU_INFO::MEMORY_SIZE]);
    ui->glsl_version->setText(info[GPU_INFO::GLSL_VERSION]);
    ui->memory_type->setText(info[GPU_INFO::MEMORY_TYPE]);
    ui->elsl_version->setText(info[GPU_INFO::ELSL_VERSION]);
    ui->mem_clock->setText(info[GPU_INFO::MEM_CLOCK]);
    ui->vulkan_version->setText(info[GPU_INFO::VULKAN_VERSION]);
    ui->gpu_clock->setText(info[GPU_INFO::GPU_CLOCK]);
    ui->cl_version->setText(info[GPU_INFO::OCL_VERSION]);
    ui->max_power->setText(info[GPU_INFO::MAX_POWER]);
    ui->cuda_version->setText(info[GPU_INFO::CUDA_VERSION]);
    ui->driver_version->setText(info[GPU_INFO::DRIVER_VERSION]);

    if(logo_map.contains(info[GPU_INFO::VENDOR])){
        QString pic_name = logo_map.value(info[GPU_INFO::VENDOR]);
        img.load(":/logo/logos/"+pic_name);
        img = img.scaledToWidth(60, Qt::SmoothTransformation);
        ui->logo->setScaledContents(true);
        ui->logo->setPixmap(QPixmap::fromImage(img));
    } else {
        ui->logo->clear();
    }
}

/**
 * @brief setlogo set logo picture
 * @param vendor: vendor name
 */
void GPUPerf::initLogoMap()
{
    logo_map.clear();
    logo_map.insert("Sietium Tech","sietium.png");
    logo_map.insert("Advanced Micro Devices, Inc. [AMD/ATI]","amd.jpg");
    logo_map.insert("Advanced Micro Devices, Inc.","amd.jpg");
    logo_map.insert("111","huawei.jpeg");
    logo_map.insert("JingJiaMicro, Ltd.","JJW.png");
    logo_map.insert("111","matrox.jpg");
    logo_map.insert("111","qualcomm.jpeg");
    logo_map.insert("111","arm.jpeg");
    logo_map.insert("111","imagination.jpg");
    logo_map.insert("111","lincom.jpg");
    logo_map.insert("NVIDIA Corporation","nvidia.jpeg");
    logo_map.insert("111","versilicon.jpeg");
    logo_map.insert("111","csic.jpeg");
    logo_map.insert("Intel Corporation","intel.jpeg");
    logo_map.insert("111","longson.jpeg");
    logo_map.insert("111","phytium.jpg");
    logo_map.insert("111","zhaoxin.jpg");
}

/**
 * @brief setlogo set logo picture
 * @param vendor: vendor name
 */
bool GPUPerf::setlogo(QString vendor)
{
}

/**
 * @brief setlogo set logo picture
 * @param vendor: vendor name
 */
void GPUPerf::setCPUClock(QString Value)
{
    ui->lineEdit_cpu_clock->setText(Value + "MHz");
    ui->progressBar_cpu_clock->setValue(Value.toDouble());
}

void GPUPerf::setCPUTemper(QString Value)
{
    #ifdef DEBUG
        qDebug() << Value;
        qDebug() << Value.toDouble();
    #endif
    ui->lineEdit_cpu_temper->setText(Value + "°C");
    ui->progressBar_cpu_temper->setValue(Value.toDouble());
}

void GPUPerf::on_pushButton_Feedback_clicked()
{
    ui->lineEdit_feedback_name->clear();
    ui->lineEdit_feedback_email->clear();
    ui->textEdit_feedback_commit->clear();

    if(myTimer->isActive())
        myTimer->stop();
    ui->stackedWidget->setCurrentIndex(3);
}

void GPUPerf::on_pushButton_Extension_clicked()
{
    if(myTimer->isActive())
        myTimer->stop();
    //init extension page
    QStandardItemModel *model = new QStandardItemModel(ui->treeView_extension);

    model->setItem(0,0,new QStandardItem("GL Extension Support"));
    for(int i=0; i<sys_gpus->gl_extension.size(); i++)
        model->item(0,0)->setChild(i,0,new QStandardItem(sys_gpus->gl_extension[i]));
    model->setItem(1,0,new QStandardItem("GLX Extension Support"));
    for(int i=0; i<sys_gpus->glx_extension.size(); i++)
        model->item(1,0)->setChild(i,0,new QStandardItem(sys_gpus->glx_extension[i]));

    ui->treeView_extension->setModel(model);
    ui->treeView_extension->setHeaderHidden(true);
    //
    ui->stackedWidget->setCurrentIndex(2);
}

void GPUPerf::update_monitor()
{
    //cpu clock
    {
        sys_gpus->get_cpu_clock();
        QString clock = sys_gpus->cpu_clock[1].at(0);
        if(clock == "Unknow")
        {
            ui->lineEdit_cpu_clock->setText("Unknow");
            ui->progressBar_cpu_clock->setValue(0.0);
        }
        else
            setCPUClock(clock);
    }
    //cpu temper
    {
        sys_gpus->get_cpu_temper();
        QString temper = sys_gpus->cpu_temper[1].at(0);
        #ifdef DEBUG
            qDebug() << temper;
        #endif

        if(temper == "Unknow")
        {
            ui->lineEdit_cpu_temper->setText("Unknow");
            ui->progressBar_cpu_temper->setValue(0);
        }
        else
            setCPUTemper(temper.remove(0, 1));
    }
    //mem usage
    {
        sys_gpus->get_mem_usage();
        ui->progressBar_mem_usage->setMaximum(100);
        if(sys_gpus->mem_usage < 0)
        {
            ui->lineEdit_mem_usage->setText("Unknow");
            ui->progressBar_mem_usage->setValue(0);
        }
        else
        {
            QString usage = QString("%1").arg(sys_gpus->mem_usage);
            ui->lineEdit_mem_usage->setText(usage+"%");
            ui->progressBar_mem_usage->setValue(sys_gpus->mem_usage);
        }
        #ifdef DEBUG
            qDebug() << usage;
        #endif
    }
}

void GPUPerf::on_pushButton_moitor_clicked()
{
    myTimer->start(1000);
    ui->stackedWidget->setCurrentIndex(1);
}

void GPUPerf::on_pushButton_Info_clicked()
{
    if(myTimer->isActive())
        myTimer->stop();
    //
    ui->stackedWidget->setCurrentIndex(0);
}

void GPUPerf::on_pushButton_close_clicked()
{
    this->close();
}

void GPUPerf::on_pushButton_lookup_clicked()
{
     QDesktopServices::openUrl(QUrl("https://www.gpuinsight.com/"));
}

void GPUPerf::on_pushButton_submit_clicked()
{
    QString feedback_name, feedback_email, feedback_commit;
    feedback_name = ui->lineEdit_feedback_name->text();
    feedback_email = ui->lineEdit_feedback_email->text();
    feedback_commit = ui->textEdit_feedback_commit->toPlainText();
#ifdef DEBUG
    qDebug() << feedback_name;
    qDebug() << feedback_email;
    qDebug() << feedback_commit;
#endif
    QRegExp reg("^[A-Za-z0-9]+([_\\.][A-Za-z0-9]+)*@([A-Za-z0-9\\-]+\\.)+[A-Za-z]{2,6}$");
    QRegExpValidator validator(reg,0);
    int pos = 0;
    if(QValidator::Acceptable!=validator.validate(feedback_email,pos)){
        QMessageBox::critical(NULL, "错误", "邮箱地址错误！！");
    }
    else
    {
        QString feedback_content("GPUInfoTool FeedBack From: ");
        feedback_content.append(feedback_name);
        feedback_content.append("\n");
        feedback_content.append(feedback_commit);

        QMessageBox::StandardButton result = QMessageBox::information(NULL, "确认发送", "是否发送该邮件，确定？",
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

        if(result == QMessageBox::Yes) {
            Smtp *sendmail = new Smtp(feedback_name,feedback_content);
            /// 判断邮件是否发送成功
            //if(sendmail->getSendResult())
            {
                QMessageBox::information(NULL, "信息", "邮件发送成功", NULL, NULL);
                ui->lineEdit_feedback_name->clear();
                ui->lineEdit_feedback_email->clear();
                ui->textEdit_feedback_commit->clear();
            //} else {
//                QMessageBox::information(NULL, "信息", "邮件发送失败", NULL, NULL);
            }
        }
    }
}

void GPUPerf::on_gpu_selected_currentIndexChanged(const QString &arg1)
{

    for(int i=0; i<sys_gpus->GPU_counts; i++)
    {
        if(arg1 == sys_gpus->system_gpus[i].product)
        {
            sys_gpus->select_index = i;
            break;
        }
    }
    setinfo(sys_gpus->sys_gpu_data[sys_gpus->select_index]);
}

void GPUPerf::on_comboBox_CPUclock_currentIndexChanged(const QString &arg1)
{
    QString clock;
    if(sys_gpus->cpu_clock[1].size() < 2)
    {
        clock = sys_gpus->cpu_clock[1].at(0);
    }
    else
    {
        QString index = arg1.mid(3,1);
#ifdef DEBUG
    qDebug() << arg1;
    qDebug() << index;
#endif
        clock = sys_gpus->cpu_clock[1].at(index.toInt() - 1);
    }

    if(clock == "Unknow")
    {
        ui->lineEdit_cpu_clock->setText("Unknow");
        ui->progressBar_cpu_clock->setValue(0.0);
    }
    else
        setCPUClock(clock);
}

void GPUPerf::on_comboBox_CPUtemper_currentIndexChanged(const QString &arg1)
{
    QString index = arg1.mid(3,1);
#ifdef DEBUG
    qDebug() << arg1;
    qDebug() << index;
#endif
    int idx = index.toInt() - 1;
    if(idx < 0) idx = 0;
    QString temper = sys_gpus->cpu_temper[1].at(idx);
    if(temper == "Unknow")
    {
        ui->lineEdit_cpu_temper->setText("Unknow");
        ui->progressBar_cpu_temper->setValue(0);
    }
    else
        setCPUTemper(temper.remove(0, 1));
}

void GPUPerf::on_pushBtn_gears_clicked()
{
    sys_gpus->gears_run();
    ui->lineEdit_gears_score->setText(QString("%1").arg(sys_gpus->gears_score));
}

//void GPUPerf::on_pushBtn_glmark_clicked()
//{
////    sys_gpus->glmark_run();
////    ui->lineEdit_glmark_score->setText(sys_gpus->glmark_score);
//}
