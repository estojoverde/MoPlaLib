#ifndef VIS_SCENE3D_H
#define VIS_SCENE3D_H

#include <QObject>

#include <QtGui>
#include <QGLWidget>
#include <QVector2D>
#include <QVector3D>
#include <QMatrix>
#include <QQuaternion>


//Constantes do zoom
#define zoomMax 10.0
#define zoomMin 0.001
#define zoomInit 0.5



#define scalefactorMin 0.0001f
#define scalefactorMax 10.0f
#define scalefactorStep 0.08f
#define scalefactorInit 0.5f


#define VIS_Scene3D_VeryLowNorm 0.00000001

class VIS_Scene3D : public QGLWidget
{
    Q_OBJECT
public:


    VIS_Scene3D(QGLWidget* parent=0);

    QMatrix4x4 rotate();
    void setMatrix(QMatrix4x4 &mat44_rotate);

    virtual QSize sizeHint() const;


private:
    QVector3D sphere_from_mouse(QPointF qpf_p);
    void move (const QPointF qpf_p);

    bool b_mouse_down__;
    QPointF qpf_start_pos__;
    QQuaternion qq_drag__;
    QMatrix4x4 mat44_matrix__;

    GLfloat f_zoomFactor__;

    GLfloat f_multMouse;

    GLfloat f_rotX__, f_rotY__, f_rotZ__, f_rotAngle__, f_rotSpeed__;

protected:
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void paintEvent(QPaintEvent* e);

    void initializeGL() Q_DECL_OVERRIDE;

    void paintGL() Q_DECL_OVERRIDE;

    void resizeGL(int width, int height) Q_DECL_OVERRIDE;

    void virtual draw();

signals:
    void signal_logMessage( QString s );
    void signal_errorMessage( QString s );
    void signal_angleChanged (QQuaternion qq_angle);
    void signal_mouseDown();
    void signal_mouseUp();


};

#endif // VIS_SCENE3D_H
