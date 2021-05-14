#include "gpuperf.h"
#include <QApplication>
#include <qopengl.h>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMessageBox>
#include <QSharedMemory>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSharedMemory singleton(a.applicationName());
    if(!singleton.create(1))
    {
        QMessageBox::warning(NULL, "Waring", "Program already running!");
        return 0;
    }

    QMessageBox::StandardButton result = QMessageBox::information(NULL, "免责声明", "本软件数据来自公开网站信息，数据仅供参考，软件保护用户个人隐私。是否打开？",
                             QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(QMessageBox::Yes == result)
    {
        GPUPerf w;
        w.show();
        return a.exec();
    }
}
