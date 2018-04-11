#ifndef VIS_VECTORFIELDVIEWER_H
#define VIS_VECTORFIELDVIEWER_H

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
#include "vis_scene3d.h"

#define VIS_VFV_MaxPathSize 70



class VIS_VectorFieldViewer: public VIS_Scene3D
{
    Q_OBJECT

public:
    VIS_VectorFieldViewer(QGLWidget *parent = 0);
    virtual ~VIS_VectorFieldViewer();

public slots:
    void slot_setPF(MPL_PotentialFieldEnvironment *p_field, QReadWriteLock  *locker);

    void slot_dumpOccupancyGrid();
    void slot_dumpPotentialGrid();

    void slot_setDrawPath(bool b_drawPath);
    void slot_toggleDrawPath();

protected:
    //void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;



    void inline setDrawPath(bool b_drawPath)
    {
            b_drawPath__ = b_drawPath;
            return;
    }

    void inline toggleDrawPath()
    {
        b_drawPath__ = !b_drawPath__;
    }



    inline void normalizeVector(GLfloat &x, GLfloat &y, GLfloat &z)
    {
        GLfloat norm = sqrt(pow(x,2) + pow(y,2) + pow(z,2));

        if( norm >= VIS_Scene3D_VeryLowNorm) {
            x/=norm;
            y/=norm;
            z/=norm;
        }
        else{
            x=y=z=0;
        }
    }

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

    long int x, xMax, xNext, xGoal;
    long int y, yMax, yNext, yGoal;
    long int z, zMax, zNext, zGoal;

    double vx, vy, vz;
    double d_auxDistance;

    bool b_pathReachedGoal;
    bool b_drawPath__;
    double d_maxModule;
    MPL_Position sMPLP_pCurrent, sMPLP_pNext, sMPLP_vDescendant, sMPLP_pGoal;

};






#endif // VIS_VECTORFIELDVIEWER_H
