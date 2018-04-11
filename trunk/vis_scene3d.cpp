#include "vis_scene3d.h"

VIS_Scene3D::VIS_Scene3D(QGLWidget *parent) : QGLWidget(parent), b_mouse_down__(false)
{
    //setFrameShape(QFrame::Panel);
    //setFrameStyle(QFrame::Panel | QFrame::Raised);

    f_zoomFactor__ = 1.0f;

    f_rotSpeed__ = 1.0f/20.0f;
}

QSize VIS_Scene3D::sizeHint()const
{
    return QSize (200,200);
}

void VIS_Scene3D::setMatrix(QMatrix4x4 &mat44_rotate)
{
    mat44_matrix__ = mat44_rotate;
}

QMatrix4x4 VIS_Scene3D::rotate()
{
    QMatrix4x4 mat44_m = mat44_matrix__;
    mat44_m.rotate(qq_drag__);
    return mat44_m;
}


QVector3D VIS_Scene3D::sphere_from_mouse(QPointF qpf_p)
{ // Maps the point on the circle to a point on a sphere
    QVector3D qv3d_v(qpf_p);
    if (qv3d_v.lengthSquared() > 1.0){
        qv3d_v.normalize();
    } else {
        qv3d_v.setZ(sqrt (1.0 - qv3d_v.lengthSquared()));
    }
    return qv3d_v;
}



void VIS_Scene3D::move (const QPointF qpf_p)
{
    if (!b_mouse_down__){
        return;
    }
    QVector3D qv3d_v0 = sphere_from_mouse(qpf_p);
    QVector3D qv3d_v1 = sphere_from_mouse(qpf_start_pos__);
    QVector3D qv3d_cross = QVector3D::crossProduct (qv3d_v0,qv3d_v1);

    qq_drag__ = QQuaternion(QVector3D::dotProduct(qv3d_v0, qv3d_v1), qv3d_cross);

    float f_pitch, f_yaw, f_roll;
    qq_drag__.getEulerAngles(&f_pitch, &f_yaw, &f_roll);

    //DEBUG ANGLES HERE!
    //qWarning( QString("Pitch(%1) \t Yaw(%2) \t Row(%3)\n").arg(f_pitch).arg(f_yaw).arg(f_roll).toLatin1());

    emit signal_angleChanged(qq_drag__);
}


void VIS_Scene3D::paintEvent(QPaintEvent* e)
{
//    QPainter p (this);
//    p.setBackground(Qt::black);
//    p.drawArc(1,1,width()-2,height()-2,0,5760);
//updateGL();

}



void VIS_Scene3D::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        QPointF qpf_pos (((2.0 * e->x())/width())-1.0, ((2.0 * e->y())/height()) -1.0);
        b_mouse_down__ = true;
        qpf_start_pos__ = qpf_pos;
        e->accept();
        emit signal_mouseDown();
    } else {
        QWidget::mousePressEvent(e);
    }
}

void VIS_Scene3D::mouseMoveEvent(QMouseEvent *e)
{
    if (b_mouse_down__) {
        QPointF qpf_pos (((2.0 * e->x())/width())-1.0, ((2.0 * e->y())/height()) -1.0);
        move(qpf_pos);
        e->accept();
        updateGL();
    }
    QWidget::mouseMoveEvent(e);
}

void VIS_Scene3D::mouseReleaseEvent(QMouseEvent *e)
{
    if (b_mouse_down__) {
        b_mouse_down__ = false;
        emit signal_mouseUp();
        e->accept();
        return;
    }
    QWidget::mouseReleaseEvent(e);
}


void VIS_Scene3D::initializeGL()
{

    GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };

    glClearColor(0.83f, 0.83f, 1.0f, 1.0f);

    //glEnable(GL_CULL_FACE);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light_ambient);

    glEnable(GL_NORMALIZE); //WHY?

    //glClearDepth(1.0);

    //glDepthFunc(GL_LESS);

    glEnable(GL_DEPTH_TEST);

    glShadeModel(GL_SMOOTH);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void VIS_Scene3D::resizeGL(int width, int height)
{
    //int side = qMin(width, height);
    //glViewport((width - side) / 2, (height - side) / 2, side, side);
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //glFrustum(0.01, -0.01, -0.01, 0.01, 0.005, 100.0);

    if(width <= height){
        GLfloat ratio =(GLfloat)height/(GLfloat)width;
        glOrtho(-20/f_zoomFactor__, +20/f_zoomFactor__, -20*ratio/f_zoomFactor__, +20*ratio/f_zoomFactor__, 1, 100);
    }
    else{
        GLfloat ratio =(GLfloat)width/(GLfloat)height;
        glOrtho(-20*ratio/f_zoomFactor__, +20*ratio/f_zoomFactor__, -20/f_zoomFactor__, +20/f_zoomFactor__, 1, 100);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //glTranslatef(0, -0.05, -1.0/zoom);
    glTranslatef(0, 0, -50.0);

    updateGL();
}


void VIS_Scene3D::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glPushMatrix();
    // Rotate a camera

    int aux=0;
//glTranslated(-aux, -aux, 0);
    //glRotated(xRot__ / 16.0, 1.0, 0.0, 0.0);
    //glRotated(yRot__ / 16.0, 0.0, -1.0, 0.0);


    qq_drag__.getAxisAndAngle(&f_rotX__, &f_rotY__, &f_rotZ__, &f_rotAngle__);
    glRotatef(f_rotAngle__ * f_rotSpeed__, f_rotX__, f_rotY__, f_rotZ__);

//glTranslated(aux, aux, 0);
    //glRotated(180, 0.0, 1.0, 0.0);

    //qWarning( QString("[X_rot]= %1\n[Y_rot]= %2").arg(xRot__).arg(yRot__).toLatin1() );
    //qWarning( QString("Degrees \t[xrot]= %1\n\t[yrot]= %2\n").arg(xRot__/16).arg(yRot__/16).toLatin1() );

    //glTranslated(-dx__, -dy__, 0);
    draw();
    //glTranslated(dx__, dy__, 0);


    //glPopMatrix();
}


void VIS_Scene3D::draw()
{
    //Front side - Multicolor
    glBegin(GL_POLYGON);
        glColor3f( 1.0, 0.0, 0.0 );     glVertex3f(  0.5, -0.5, -0.5 );      // P1 é vermelho
        glColor3f( 0.0, 1.0, 0.0 );     glVertex3f(  0.5,  0.5, -0.5 );      // P2 é verde
        glColor3f( 0.0, 0.0, 1.0 );     glVertex3f( -0.5,  0.5, -0.5 );      // P3 é azul
        glColor3f( 1.0, 0.0, 1.0 );     glVertex3f( -0.5, -0.5, -0.5 );      // P4 é roxo
    glEnd();

    // Back side - white
    glBegin(GL_POLYGON);
        glColor3f(   1.0,  1.0, 1.0 );
        glVertex3f(  0.5, -0.5, 0.5 );
        glVertex3f(  0.5,  0.5, 0.5 );
        glVertex3f( -0.5,  0.5, 0.5 );
        glVertex3f( -0.5, -0.5, 0.5 );
    glEnd();

    // Right side - Green
    glBegin(GL_POLYGON);
        glColor3f(   0.0,  1.0,  0.0 );

        glVertex3f( 0.5, -0.5, -0.5 );
        glVertex3f( 0.5,  0.5, -0.5 );
        glVertex3f( 0.5,  0.5,  0.5 );
        glVertex3f( 0.5, -0.5,  0.5 );
    glEnd();

    // Left side - purple
    glBegin(GL_POLYGON);
        glColor3f(  1.0,  0.0,  1.0 );
        glVertex3f( -0.5, -0.5,  0.5 );
        glVertex3f( -0.5,  0.5,  0.5 );
        glVertex3f( -0.5,  0.5, -0.5 );
        glVertex3f( -0.5, -0.5, -0.5 );
    glEnd();

    // Up side - blue
    glBegin(GL_POLYGON);
        glColor3f(   0.0,  0.0,  1.0 );
        glVertex3f(  0.5,  0.5,  0.5 );
        glVertex3f(  0.5,  0.5, -0.5 );
        glVertex3f( -0.5,  0.5, -0.5 );
        glVertex3f( -0.5,  0.5,  0.5 );
    glEnd();

    // Bottom side - red
    glBegin(GL_POLYGON);
        glColor3f(   1.0,  0.0,  0.0 );
        glVertex3f(  0.5, -0.5, -0.5 );
        glVertex3f(  0.5, -0.5,  0.5 );
        glVertex3f( -0.5, -0.5,  0.5 );
        glVertex3f( -0.5, -0.5, -0.5 );
    glEnd();
}
