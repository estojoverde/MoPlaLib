#ifndef MPL_VECTORFIELDVIEWER_H
#define MPL_VECTORFIELDVIEWER_H


//#define QT_NO_OPENGL_ES_2

//#define GLEW_STATIC
//#include <GL/glew.h>

#include <cmath>

#include <QGLWidget>
#include <QtWidgets>
#include <QtOpenGL>
#include <QColor>
#include <QMutex>
#include <QMutexLocker>
#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>


#include "mpl_potentialfieldenvironment.h"
#include "scene3d.h"



class MPL_VectorFieldViewer: public Scene3D
{
    Q_OBJECT

public:
    MPL_VectorFieldViewer(QWidget *parent = 0);
    virtual ~MPL_VectorFieldViewer();

public slots:
    void slot_setPF(MPL_PotentialFieldEnvironment *p_field, QReadWriteLock  *locker);


protected:
    //void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;


    QSize minimumSizeHint();

    QSize sizeHint();



    //void initializeGL();

    //void resizeGL(int width, int height);

    //void paintGL();

    void drawField();

    void draw();

    void drawAxis();
    void drawCube(GLint x, GLint y, GLint z, bool bSolid = true, GLfloat color[]=NULL);
    void drawVector_rad(GLint x, GLint y, GLint z, GLfloat theta, GLfloat phi, bool bCellCenter=true, GLfloat colorTail[]=NULL, GLfloat colorHead[]=NULL);
    void drawVector_xyz(GLint x, GLint y, GLint z, GLfloat vx, GLfloat vy, GLfloat vz, bool bCellCenter=true, GLfloat colorTail[]=NULL, GLfloat colorHead[]=NULL);
    void drawVector_mplpos(MPL_Position pos, MPL_Position vec, bool bCellCenter=true, GLfloat colorTail[]=NULL, GLfloat colorHead[]=NULL);

    void calculateVectorNormal(GLfloat p1_x,   GLfloat p1_y, GLfloat p1_z, \
                                GLfloat p2_x,   GLfloat p2_y, GLfloat p2_z, \
                                GLfloat p3_x,   GLfloat p3_y, GLfloat p3_z, \
                                GLfloat &N_X,   GLfloat &N_Y, GLfloat &N_Z);

    QTimer t;
    MPL_PotentialFieldEnvironment *p_field__;

    //Locker
    QReadWriteLock *locker__;

};

#endif // MPL_VECTORFIELDVIEWER_H
