#ifndef GEARSWIN_H
#define GEARSWIN_H

#include <QWidget>
#include <QtOpenGL/QtOpenGL>

namespace Ui {
class gearsWin;
}

class gearsWin : public QGLWidget
{
    Q_OBJECT

public:
    explicit gearsWin(int n=500, QWidget *parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);
    ~gearsWin();
    int frame_cnt;
private:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void timerEvent(QTimerEvent *event);
    void gear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width,
                     GLint teeth, GLfloat tooth_depth);
    int width, height;
    float view_rotx, view_roty, view_rotz;
    int gear1, gear2, gear3;
    int frame_num;
    Ui::gearsWin *ui;
};

#endif // GEARSWIN_H
