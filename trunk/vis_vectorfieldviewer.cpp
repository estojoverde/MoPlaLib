#include "vis_vectorfieldviewer.h"


VIS_VectorFieldViewer::VIS_VectorFieldViewer(QGLWidget *parent): VIS_Scene3D(parent)
{
    //setFixedSize(200, 200);
    //setAutoFillBackground(false);

    p_field__ = NULL;
    locker__ = NULL;

    t.setInterval(100);

    connect(&t, SIGNAL(timeout()), this, SLOT(repaint()) );

    xMax = yMax = zMax = 10;

    b_drawPath__ = false;

}

VIS_VectorFieldViewer::~VIS_VectorFieldViewer()
{

}


QSize VIS_VectorFieldViewer::minimumSizeHint()
{
    return QSize(50, 50);
}


QSize VIS_VectorFieldViewer::sizeHint()
{
    return QSize(400, 400);
}


//void VIS_VectorFieldViewer::initializeGL()
//{

//    qglClearColor(QColor(210,210,255));

//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
//    glShadeModel(GL_SMOOTH);

//    glEnable(GL_COLOR_MATERIAL);

//    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//    glEnable(GL_LIGHTING);

//    GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };

//    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light_ambient);
//}



//void VIS_VectorFieldViewer::resizeGL(int width, int height)
//{
//    int side = qMin(width, height);

//    GLfloat zoom = 1;

//    glViewport(0, 0, width, height);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();

//    if(width <= height){
//        GLfloat ratio =(GLfloat)height/(GLfloat)width;
//        glOrtho(-5/zoom, +5/zoom, -5*ratio/zoom, +5*ratio/zoom, 1, 15);
//        //glFrustrum( )

//    }
//    else{
//        GLfloat ratio =(GLfloat)width/(GLfloat)height;
//        glOrtho(-5*ratio/zoom, +5*ratio/zoom, -5/zoom, +5/zoom, 1, 15);
//    }


//    //glOrtho(-5, +5, -5, +5, 0.1, 15.0);
//    //glFrustum(-5, +5, -5, +5, 1.0, 15.0);

//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//}


//void VIS_VectorFieldViewer::paintGL()
//{
//    //pre_paintGL();
//    draw();
//    GLfloat mat_view[16];
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glLoadIdentity();

//    getView(mat_view);//magical stuff?
//    glMultMatrixf(mat_view);//magical stuff the return?

//    draw();
//}


void VIS_VectorFieldViewer::calculateVectorNormal(\
    GLfloat p1_x,   GLfloat p1_y, GLfloat p1_z,\
    GLfloat p2_x,   GLfloat p2_y, GLfloat p2_z,\
    GLfloat p3_x,   GLfloat p3_y, GLfloat p3_z,\
    GLfloat &N_X,   GLfloat &N_Y, GLfloat &N_Z)
{
    GLfloat Qx, Qy, Qz, Px, Py, Pz, aux_Nv[3], auxNorm;

    Qx = p2_x - p1_x;
    Qy = p2_y - p1_y;
    Qz = p2_z - p1_z;
    Px = p3_x - p1_x;
    Py = p3_y - p1_y;
    Pz = p3_z - p1_z;

    aux_Nv[0] = Py*Qz - Pz*Qy;
    aux_Nv[1] = Pz*Qx - Px*Qz;
    aux_Nv[2] = Px*Qy - Py*Qx;

    auxNorm = sqrt(pow(aux_Nv[0], 2) + pow(aux_Nv[1], 2) + pow(aux_Nv[2], 2));

    normalizeVector(aux_Nv[0], aux_Nv[1], aux_Nv[2]);
    N_X = aux_Nv[0];
    N_Y = aux_Nv[1];
    N_Z = aux_Nv[2];
}


void VIS_VectorFieldViewer::draw()
{
    //glPushMatrix();



    //VIS_Scene3D::draw();

    //keeps field in the center of rotation
    glTranslatef(-xMax/2.0f,-yMax/2.0f,-zMax/2.0f);

    drawAxis();

    if(NULL==p_field__){
        GLfloat x, y, z, vx, vy, vz;
        x=2;
        y=5;
        z=1;
        vx=1;
        vy=1;
        vz=-9;
        normalizeVector(vx,vy,vz);
        drawVector_xyz(x, y, z, vx, vy, vz);
    }
    else{
        drawField();
    }

    //keeps field in the center of rotation
    glTranslatef(xMax/2.0f,yMax/2.0f,zMax/2.0f);

    //glPopMatrix();
}



void VIS_VectorFieldViewer::drawVector_xyz(GLint x, GLint y, GLint z, GLfloat vx, GLfloat vy, GLfloat vz, bool bCellCenter, GLfloat colorTail[], GLfloat colorHead[])
{
    GLfloat theta = atan2(vy, vx);
    GLfloat phi = asin(vz);

    //qWarning("(vx,vy,vz) = (%lf, %lf, %lf) ", vx, vy, vz);
    //qWarning("RAD(theta,phi) = (%lf, %lf) ", theta, phi);

    theta = (theta/M_PI) * 180;
    phi = (phi/M_PI) * 180;

    //qWarning("DEGREES(theta,phi) = (%lf, %lf) ", theta, phi);


    drawVector_rad(x, y, z, theta, phi, bCellCenter, colorTail, colorHead);
}

void VIS_VectorFieldViewer::drawVector_mplpos(MPL_Position pos, MPL_Position vec, bool bCellCenter, GLfloat colorTail[], GLfloat colorHead[])
{
    drawVector_xyz(pos.getX(), pos.getY(), pos.getZ(), vec.getX(), vec.getY(), vec.getZ(), bCellCenter, colorTail, colorHead);
}


void VIS_VectorFieldViewer::drawVector_rad(GLint x, GLint y, GLint z, GLfloat theta, GLfloat phi, bool bCellCenter, GLfloat colorTail[], GLfloat colorHead[])
{
    glPushMatrix();
    GLfloat auxNorm[3];
    GLfloat z_shift = -0.5;

    GLfloat tailLength = 0.88;
    GLfloat sideUpperTriangle = 0.18;
    GLfloat sideUpperTriangle_half = sideUpperTriangle/2.0;
    GLfloat sideUpperTriangle_third = sideUpperTriangle/3.0;

    GLfloat sideLowerTriangle = sideUpperTriangle/3;
    GLfloat sideLowerTriangle_half = sideLowerTriangle/2.0;
    GLfloat sideLowerTriangle_third = sideLowerTriangle/3.0;



    //theta+=180; // because inverted canvas of opengl
    phi-=90; //because vector is draw on z-axis

    //qWarning("DRAWVECTOR2(theta,phi) = (%lf, %lf) ", theta, phi);


    glTranslatef(x,y,z);

    if(bCellCenter){
        glTranslatef(0.5f, 0.5f, 0.5f);
    }


    //rotate
    glRotatef(theta, 0, 0, 1); //XY plane - around Z axis [atan2(y,x)]
    glRotatef(phi, 0, 1, 0); //X'Z' plane - around Y' axis [atan2(x,z)]

    if(NULL == colorHead)
    {
        glColor4d(0,  0,  0, 1);
    }
    else{
        glColor4fv(colorHead);
    }


    //base
    glBegin(GL_TRIANGLES);
        glNormal3f(0,0,-1);
        glVertex3f(-sideUpperTriangle_half, sideUpperTriangle_third,    tailLength+z_shift);//P1
        glVertex3f(0,                       -2*sideUpperTriangle_third, tailLength+z_shift);//P3
        glVertex3f(sideUpperTriangle_half,  sideUpperTriangle_third,    tailLength+z_shift);//P2
    glEnd();
    //sides
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0,                       0,                          1+z_shift);         //P4
        glVertex3f(-sideUpperTriangle_half, sideUpperTriangle_third,    tailLength+z_shift);//P1
        glVertex3f(sideUpperTriangle_half,  sideUpperTriangle_third,    tailLength+z_shift);//P2
        glVertex3f(0,                       -2*sideUpperTriangle_third, tailLength+z_shift);//P3
        glVertex3f(-sideUpperTriangle_half, sideUpperTriangle_third,    tailLength+z_shift);//P1
    glEnd();


    //tail
    if(NULL == colorTail)
    {
        glColor4d(0.4,  0.4,  0.4, 0.8);
    }
    else{
        glColor4fv(colorTail);
    }

    //Tbase
    glBegin(GL_TRIANGLES);
        calculateVectorNormal(-sideLowerTriangle_half, sideLowerTriangle_third,    0,\
                              sideLowerTriangle_half,  sideLowerTriangle_third,    0,\
                              0,                       -2*sideLowerTriangle_third, 0,\
                              auxNorm[0],              auxNorm[1],                 auxNorm[2]);
        glNormal3f(auxNorm[0],auxNorm[1],auxNorm[2]);
        glVertex3f(-sideLowerTriangle_half, sideLowerTriangle_third,    0+z_shift);
        glVertex3f(sideLowerTriangle_half,  sideLowerTriangle_third,    0+z_shift);
        glVertex3f(0,                       -2*sideLowerTriangle_third, 0+z_shift);
    glEnd();



    //Tbase
    glBegin(GL_TRIANGLES);
        //Normal here
        glVertex3f(-sideLowerTriangle_half, sideLowerTriangle_third,    tailLength+z_shift);//P1'
        glVertex3f(-sideLowerTriangle_half, sideLowerTriangle_third,    0+z_shift);//P1''
        glVertex3f(0,                       -2*sideLowerTriangle_third, 0+z_shift);//P3''


        //Normal
        glVertex3f(0,                       -2*sideLowerTriangle_third, tailLength+z_shift);//P3'
        glVertex3f(-sideLowerTriangle_half, sideLowerTriangle_third,    tailLength+z_shift);//P1'
        glVertex3f(0,                       -2*sideLowerTriangle_third, 0+z_shift);//P3''


        //Normal
        glVertex3f(0,                       -2*sideLowerTriangle_third, tailLength+z_shift);//P3'
        glVertex3f(0,                       -2*sideLowerTriangle_third, 0+z_shift);//P3''
        glVertex3f(sideLowerTriangle_half,  sideLowerTriangle_third,    0+z_shift);//P2''

        //Normal
        glVertex3f(sideLowerTriangle_half,  sideLowerTriangle_third,    tailLength+z_shift);//P2'
        glVertex3f(0,                       -2*sideLowerTriangle_third, tailLength+z_shift);//P3'
        glVertex3f(sideLowerTriangle_half,  sideLowerTriangle_third,    0+z_shift);//P2''

        //Normal
        glVertex3f(sideLowerTriangle_half,  sideLowerTriangle_third,    tailLength+z_shift);//P2'
        glVertex3f(sideLowerTriangle_half,  sideLowerTriangle_third,    0+z_shift);//P2''
        glVertex3f(-sideLowerTriangle_half, sideLowerTriangle_third,    0+z_shift);//P1''

        //Normal
        glVertex3f(-sideLowerTriangle_half, sideLowerTriangle_third,    tailLength+z_shift);//P1'
        glVertex3f(sideLowerTriangle_half,  sideLowerTriangle_third,    tailLength+z_shift);//P2'
        glVertex3f(-sideLowerTriangle_half, sideLowerTriangle_third,    0+z_shift);//P1''

    glEnd();

    glPopMatrix();
}


void VIS_VectorFieldViewer::drawAxis()
{

    //axis: (x)black; (y)white; (z)red
    glBegin(GL_LINES);
        glColor3f(0,0,0);
        glVertex3f(0,0,0);
        glVertex3f(10,0,0);

        glColor3f(1,1,1);
        glVertex3f(0,0,0);
        glVertex3f(0,10,0);

        glColor3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(0,0,10);
    glEnd();
    //end axis
}


void VIS_VectorFieldViewer::drawCube(GLint x, GLint y, GLint z, bool bSolid, GLfloat color[])
{

    glPushMatrix();

    glTranslatef(x,y,z);

    if(NULL == color){
        glColor4f(0.0, 0.0, 0.0, 0.3);
    }
    else{
         glColor4fv(color);
    }

    if(bSolid){
        glBegin(GL_QUADS);
            //far
            glVertex3f(0,0,0);
            glVertex3f(0,1,0);
            glVertex3f(1,1,0);
            glVertex3f(1,0,0);

            //near
            glVertex3f(0,0,1);
            glVertex3f(1,0,1);
            glVertex3f(1,1,1);
            glVertex3f(0,1,1);

            //left
            glVertex3f(0,0,0);
            glVertex3f(0,0,1);
            glVertex3f(0,1,1);
            glVertex3f(0,1,0);

            //right
            glVertex3f(1,0,0);
            glVertex3f(1,1,0);
            glVertex3f(1,1,1);
            glVertex3f(1,0,1);

            //bottom
            glVertex3f(0,0,0);
            glVertex3f(1,0,0);
            glVertex3f(1,0,1);
            glVertex3f(0,0,1);

            //up
            glVertex3f(0,1,0);
            glVertex3f(0,1,1);
            glVertex3f(1,1,1);
            glVertex3f(1,1,0);


        glEnd();
    }
    else{

        glBegin(GL_LINE_STRIP);
            glVertex3f(0,0,0);
            glVertex3f(1,0,0);
            glVertex3f(1,1,0);
            glVertex3f(0,1,0);
            glVertex3f(0,0,0);

            glVertex3f(0,0,1);
            glVertex3f(1,0,1);
            glVertex3f(1,1,1);
            glVertex3f(0,1,1);
            glVertex3f(0,0,1);
        glEnd();


        glBegin(GL_LINES);
            glVertex3f(0,1,0);
            glVertex3f(0,1,1);

            glVertex3f(1,0,0);
            glVertex3f(1,0,1);

            glVertex3f(1,1,0);
            glVertex3f(1,1,1);
        glEnd();
    }

    glPopMatrix();
}



void VIS_VectorFieldViewer::drawField(){

    if( (NULL == p_field__) || (NULL == locker__) ){
        qWarning(" void MPL_VectorFieldViewer::drawField() FAILED");
        return;
    }

    QReadLocker readLocker(locker__);//lock to read here!

    xMax = p_field__->getGridSizeX();
    yMax = p_field__->getGridSizeY();
    zMax = p_field__->getGridSizeZ();

    MPL_StateCellGridType occup;


    GLfloat colorHead[4]={0.3,  0.3,  0.3, 0.8};
    GLfloat colorTail[4]={0.8,  0.8,  0.8, 0.3};
    GLfloat colorPath[4]={0.0,  0.0,  0.8, 0.9};

    GLfloat colorEnemy[4]={1,  0.5,  0.1, 0.5};
    GLfloat colorGoal[4]={0.1,  0.8,  0.1, 0.5};
    GLfloat colorFObstacle[4]={1,  0.2,  0.2, 0.5};
    GLfloat colorMObstacle[4]={0.8,  0.1,  0.1, 0.5};
    GLfloat colorWhite[4]={1, 1, 1, 1};
    GLfloat colorBlack[4]={0, 0, 0, 1};


    for( z=0; z<zMax; z++ )
    {
        for( x=0; x<xMax; x++)
        {
            for( y=0; y<yMax; y++)
            {
                if( !( MPL_ReturnCode_OK == p_field__->getOccupancy(x, y, z, occup) ) ){
                    emit signal_errorMessage(QString("Failed getting Occupancy at (%1, %2, %3)").arg(x).arg(y).arg(z) );
                    return;
                }

                if(!((MPL_OccupancyGrid_CellFree==occup) ||\
                     (MPL_OccupancyGrid_CellNotExplored==occup) || \
                     (MPL_OccupancyGrid_CellOccupiedByOwnRobot==occup) ))
                {

                    switch (occup)
                    {
                    case MPL_OccupancyGrid_CellOccupiedByFixedObstacle:
                        drawCube(x, y, z, true, colorFObstacle);
                        break;
                    case MPL_OccupancyGrid_CellOccupiedByMovableObstacle:
                        drawCube(x, y, z, true, colorMObstacle);
                        break;
                    case MPL_OccupancyGrid_CellOccupiedByEnemyRobot:
                        drawCube(x, y, z, true, colorEnemy);
                        break;
                    case MPL_OccupancyGrid_CellOccupiedByGoal:
                        drawCube(x, y, z, true, colorGoal);
                        break;
                    case MPL_OccupancyGrid_CellOccupiedByOwnRobot:
                        drawCube(x, y, z, true, colorWhite);
                        drawCube(x, y, z, false, colorBlack); //@BUG? Should be drawing own robot??
                        break;
                    default:
                        //do nothing
                        break;
                    }

                }
                else
                {

                    //p_field__->unsafe_getGradientDescent(pos_aux[0], pos_aux[1]); //BUG
                    p_field__->getGradientDescent(x, y, z, vx, vy, vz); //NO BUG

                    //normalizeVector(vx, vy, vz); //its already normalized by getGradientDescent call
                    drawVector_xyz(x, y, z, (float)vx, (float)vy, -(float)vz, true, colorTail, colorHead);
                }
            }
        }
    }

    //DRAW PATH HERE

    //follows the path to the goal (biggest coordinate goes to 1, coordinate less than 0.5 goes to 0 and bigger goes to 1), drawing lines
    if(b_drawPath__)
    {
        b_pathReachedGoal = false;
        p_field__->getOwnRobot(sMPLP_pNext);
        p_field__->getGoalAt(sMPLP_pGoal,0);

        xGoal = sMPLP_pGoal.getX();
        yGoal = sMPLP_pGoal.getY();
        zGoal = sMPLP_pGoal.getZ();

        xNext = sMPLP_pNext.getX();
        yNext = sMPLP_pNext.getY();
        zNext = sMPLP_pNext.getZ();

        for( register int i=0; (i<VIS_VFV_MaxPathSize) && (!b_pathReachedGoal) ; i++)
        {
            x = xNext;
            y = yNext;
            z = zNext;

            p_field__->getGradientDescent(x, y, z, vx, vy, vz);

            d_maxModule = MPL_MAX_MODULE3(vx, vy, vz);

//            qDebug() << "\n------";
//            qDebug() << "Current Position ( " << x << ", " << y << ", " << z << ")";
//            qDebug() << "Gradient ( " << vx << ", " << vy << ", " << vz << ")";
//            qDebug() << "Max Module [" << d_maxModule << "]";

            if( d_maxModule > MPL_Position_DELTA )
            {
                vx /= d_maxModule;
                vy /= d_maxModule;
                vz /= d_maxModule;

//                qDebug() << "Gradient Normatized ( " << vx << ", " << vy << ", " << vz << ")";


                //@BUG Should be PLUS, but gradient signal is wrong for some unknown reason
                xNext -= (int) round(vx);
                yNext -= (int) round(vy);
                zNext -= (int) round(vz);



                //DRAW

                //base
                glPushAttrib(GL_LINE_BIT);
                glColor4fv(colorPath);
                glLineWidth(5);
                glBegin(GL_LINES);
                    glVertex3f(x+0.5, y+0.5, z+0.5);//P1
                    glVertex3f(xNext+0.5, yNext+0.5, zNext+0.5);//P3
                glEnd();
                glPopAttrib();

                QString qs_aux = "( " + QString::number(x) + "," +
                                        QString::number(y) + "," +
                                        QString::number(z) + ") -> (" +
                                        QString::number(xNext) + "," +
                                        QString::number(yNext) + "," +
                                        QString::number(zNext) + ")";
                qDebug() << "\n-- Line " << qs_aux;

                d_auxDistance = (xGoal-xNext)*(xGoal-xNext) +\
                                (yGoal-yNext)*(yGoal-yNext) +\
                                (zGoal-zNext)*(zGoal-zNext);


                if( d_auxDistance < 0.5 )
                {
                    //goal reached
                    b_pathReachedGoal= true;
                    vx = vy = vz = 0;
                }
            }
            else
            {
                vx = vy = vz = 0;
                b_pathReachedGoal = true;
            }

        }
    }

    return;
}


void VIS_VectorFieldViewer::slot_setPF(MPL_PotentialFieldEnvironment *p_field, QReadWriteLock  *locker)
{
    p_field__ = p_field;
    locker__ = locker;
}



void VIS_VectorFieldViewer::slot_dumpPotentialGrid()
{
    double d_potential;
    xMax = p_field__->getGridSizeX();
    yMax = p_field__->getGridSizeY();
    zMax = p_field__->getGridSizeZ();

    QString qs_aux;
    qDebug() << "\n---- Potential Grid\n";
    qs_aux = "";
    for( z=0; z<zMax; z++ )
    {
        qs_aux = "Level " + QString::number(z);
        qDebug() << qs_aux ;
        for( x=0; x<xMax; x++)
        {
            qs_aux = "";
            for( y=0; y<yMax; y++)
            {
                p_field__->unsafe_getPotential(x, y, z, d_potential);
                qs_aux += " " + QString::number(d_potential, 'f', 6);
                //printf("%01.03lf ", d_potential);
            }
            qDebug() << qs_aux ;
        }
    }
}

void VIS_VectorFieldViewer::slot_dumpOccupancyGrid()
{
    MPL_StateCellGridType occupancy;

    xMax = p_field__->getGridSizeX();
    yMax = p_field__->getGridSizeY();
    zMax = p_field__->getGridSizeZ();

    QString c;
    QString qs_aux;
    qDebug() << "\n---- Occupancy Grid\n";
    qs_aux = "";
    for( z=0; z<zMax; z++ )
    {
        qs_aux = "Level " + QString::number(z);
        qDebug() << qs_aux ;
        for( x=0; x<xMax; x++)
        {
            qs_aux = "";
            for( y=0; y<yMax; y++)
            {
                p_field__->getOccupancy(x, y, z, occupancy);
                switch( occupancy )
                {
                    case 2:
                        c="R";
                        break;
                    case 4:
                        c="E";
                        break;
                    case 8:
                        c="M";
                        break;
                    case 16:
                        c="F";
                        break;
                    case 32:
                        c="G";
                        break;
                    default:
                        c=".";
                        break;
                }
                qs_aux += " " + c;

            }
            qDebug() << qs_aux ;
        }
    }

}







void VIS_VectorFieldViewer::slot_setDrawPath(bool b_drawPath)
{
    setDrawPath(b_drawPath);
    return;
}

void VIS_VectorFieldViewer::slot_toggleDrawPath()
{
    toggleDrawPath();
    return;
}

