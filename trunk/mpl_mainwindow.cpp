#include "mpl_mainwindow.h"
#include "vis_vectorfieldviewer.h"

MPL_MainWindow::MPL_MainWindow(QWidget *parent) : QMainWindow(parent)
{

    mplCore__=NULL;

    vfViewer__ = new VIS_VectorFieldViewer();
    b_verbose__=false;

//    VIS_VectorFieldViewer *teste;
//    teste= new VIS_VectorFieldViewer();
//    //Scene3D *scene = new Scene3D();

    setCentralWidget(vfViewer__);
    createLog();
    createCoreControl();


    create3dviewsToolbar();
    createActions();
    createMenus();
    createStatusBar();





    setConnections(); //last thing to be called! EVER!



    //connect(textEdit->document(), SIGNAL(contentsChanged()),
    //        this, SLOT(documentWasModified()));

    //setCurrentFile("");
    //setUnifiedTitleAndToolBarOnMac(true);


    //QTimer::singleShot(5000, vfViewer__, SLOT(snapshot()) );
}




void MPL_MainWindow::setCore(MPL_Core *mplCore, QReadWriteLock  *locker)
{
    if( (NULL == mplCore) || (NULL == locker) ){
        emit signal_errorMessage("No Core or Locker set!");
        return;
    }

    locker__ = locker;

    mplCore__=mplCore;

    mplCore__->slot_setCoreMode(MPL_Core_OPF);

    //LOG
    connect(mplCore__, SIGNAL(signal_errorMessage(QString)), this, SLOT(slot_errorMessage(QString)));
    connect(mplCore__, SIGNAL(signal_logMessage(QString)), this, SLOT(slot_logMessage(QString)));
    //verbose
    connect(verboseAct__, SIGNAL(triggered(bool)), mplCore__, SLOT(slot_toggleVerbose()) );


    //OPF

        //grid
    connect(this, SIGNAL(signalcore_setGridSize(int,int,int)), mplCore__, SLOT(slot_setGridSize(int,int,int)) );
    connect(this, SIGNAL(signalcore_setOmega(double)), mplCore__, SLOT(slot_setOmega(double)) );
    connect(this, SIGNAL(signalcore_setSolveMode(MPL_Core_SolveMode)), mplCore__, SLOT(slot_setSolveMode(MPL_Core_SolveMode)) );

        //solve
    connect(this, SIGNAL(signalcore_solveI(int)), mplCore__, SLOT(slot_solveI(int)) );
    connect(this, SIGNAL(signalcore_solveE(double)), mplCore__, SLOT(slot_solveE(double)) );
    connect(this, SIGNAL(signalcore_solveIE(int,double)), mplCore__, SLOT(slot_solveIE(int,double)) );


        //Objects Lists

            //OR
    connect(this, SIGNAL(signalcore_setEpsilon_OPF(double)), mplCore__, SLOT(slot_setEpsilon_OPF(double)) );
    connect(this, SIGNAL(signalcore_setVector_OPF(double,double,double)), mplCore__, SLOT(slot_setVector_OPF(double,double,double)) );
    connect(this, SIGNAL(signalcore_setOwnRobot(double,double,double)), mplCore__, SLOT(slot_setOwnRobot(double,double,double)) );


            //ER
    connect(this, SIGNAL(signalcore_addEnemyRobot(double,double,double)), mplCore__, SLOT(slot_addEnemyRobot(double,double,double)) );
    connect(this, SIGNAL(signalcore_clearListOfEnemyRobot()), mplCore__, SLOT(slot_clearListOfEnemyRobot()) );

            //FO
    connect(this, SIGNAL(signalcore_addFixedObstacle(double,double,double)), mplCore__, SLOT(slot_addFixedObstacle(double,double,double)) );
    connect(this, SIGNAL(signalcore_clearListOfFixedObstacle()), mplCore__, SLOT(slot_clearListOfFixedObstacle()) );

            //MO
    connect(this, SIGNAL(signalcore_addMovableObstacle(double,double,double)), mplCore__, SLOT(slot_addMovableObstacle(double,double,double)));
    connect(this, SIGNAL(signalcore_clearListOfMovableObstacle()), mplCore__, SLOT(slot_clearListOfMovableObstacle()) );

            //G
    connect(this, SIGNAL(signalcore_addGoal(double,double,double)), mplCore__, SLOT(slot_addGoal(double,double,double)) );
    connect(this, SIGNAL(signalcore_clearListOfGoal()), mplCore__, SLOT(slot_clearListOfGoal()) );




    vfViewer__->slot_setPF(mplCore__->getPF(), locker__);
    //void signal_pfSolved(int n_finalIt, double d_finalError ); DO IT!

    return;
}


void MPL_MainWindow::createLog()
{
    dockWidgetLog__ = new QDockWidget(tr("Log"), this);
    textEditLog__ = new QTextEdit(dockWidgetLog__);

    textEditLog__->setTextBackgroundColor(Qt::white);
    textEditLog__->setTextColor(Qt::black);
    textEditLog__->setFontPointSize(8);

    dockWidgetLog__->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockWidgetLog__->setWidget(textEditLog__);


    dockWidgetLog__->setObjectName("dockwidget_log__");

    addDockWidget(Qt::RightDockWidgetArea, dockWidgetLog__);

    b_textEdit_logState=true;

    toggleLogAct__ = dockWidgetLog__->toggleViewAction();
    toggleLogAct__->setIcon(QIcon(":icons/qrcAlias_log.ico"));
    toggleLogAct__->setIconText(tr("&Log"));
}


void MPL_MainWindow::create3dviewsToolbar()
{
    toolbar_3dviews = addToolBar(tr("3D View"));
    toolbar_3dviews->setObjectName("toolbar_3dviews");

    action_mouseMultiplier = new QAction(QIcon(":icons/qrcAlias_mousex10.ico"), "x10", this);
    action_mouseMultiplier->setStatusTip(tr("Multiplies by 10 in while scrooling"));
    action_mouseMultiplier->setCheckable(true);
    connect(action_mouseMultiplier, SIGNAL(toggled(bool)), vfViewer__, SLOT(mouseX10(bool)) );

    action_3DViews[0] = new QAction(QIcon(":icons/qrcAlias_top.ico"), "Top View", this);
    action_3DViews[1] = new QAction(QIcon(":icons/qrcAlias_bottom.ico"), "Bottom View", this);
    action_3DViews[2] = new QAction(QIcon(":icons/qrcAlias_left.ico"), "Left View", this);
    action_3DViews[3] = new QAction(QIcon(":icons/qrcAlias_right.ico"), "Right View", this);
    action_3DViews[4] = new QAction(QIcon(":icons/qrcAlias_front.ico"), "Front View", this);
    action_3DViews[5] = new QAction(QIcon(":icons/qrcAlias_back.ico"), "Back View", this);


    toolbar_3dviews->addAction(action_3DViews[0]);
    connect(action_3DViews[0], SIGNAL(triggered(bool)), this, SLOT(slotgui_topView()) );

    toolbar_3dviews->addAction(action_3DViews[1]);
    connect(action_3DViews[1], SIGNAL(triggered(bool)), this, SLOT(slotgui_bottomView()) );

    toolbar_3dviews->addAction(action_3DViews[2]);
    connect(action_3DViews[2], SIGNAL(triggered(bool)), this, SLOT(slotgui_leftView()) );

    toolbar_3dviews->addAction(action_3DViews[3]);
    connect(action_3DViews[3], SIGNAL(triggered(bool)), this, SLOT(slotgui_rightView()) );

    toolbar_3dviews->addAction(action_3DViews[4]);
    connect(action_3DViews[4], SIGNAL(triggered(bool)), this, SLOT(slotgui_frontView()) );

    toolbar_3dviews->addAction(action_3DViews[5]);
    connect(action_3DViews[5], SIGNAL(triggered(bool)), this, SLOT(slotgui_backView()) );

    toolbar_3dviews->addSeparator();

    toolbar_3dviews->addAction(action_mouseMultiplier);
    toggle3DViewsAct__ = toolbar_3dviews->toggleViewAction();
    toggle3DViewsAct__->setIcon(QIcon(":icons/qrcAlias_viewmenu.ico"));
    toggle3DViewsAct__->setIconText(tr("3D Views"));


}

void MPL_MainWindow::slotgui_topView()
{
    emit signalviewer_rotacoes(0, 180);
}


void MPL_MainWindow::slotgui_bottomView()
{
    emit signalviewer_rotacoes(0, 0);
}

void MPL_MainWindow::slotgui_leftView()
{
    emit signalviewer_rotacoes(0, 90);
}

void MPL_MainWindow::slotgui_rightView()
{
    emit signalviewer_rotacoes(0, 270);
}

void MPL_MainWindow::slotgui_frontView()
{
    emit signalviewer_rotacoes(270, 180);
}

void MPL_MainWindow::slotgui_backView()
{
    emit signalviewer_rotacoes(90, 180);
}



//bool MPL_MainWindow::eventFilter(QObject *obj, QEvent *event)
//{
//qWarning("Pfdhgdrghdfg!!!");
//    if(event->type() == QEvent::KeyPress)
//    {
//        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
//        if( keyEvent->key() == Qt::Key_0 ){
//            qWarning("PRESSED 0!!!");
//            return true;
//        }

//    }

//    return QMainWindow::eventFilter(obj, event);
//}


void MPL_MainWindow::createCoreControl()
{
    //other stuff that needs to stay here
    doublevalidator_positive__ = new QDoubleValidator();
    doublevalidator_positive__->setBottom(0);
    doublevalidator_positive__->setDecimals(10);


    //Core itself
    dockwidget_coreMode__ = new QDockWidget(tr("Core Mode"), this);
    dockwidget_coreMode__->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    widget_page_opf__ = new QWidget();
    //widget_page_hpf__ = new QWidget();
    tabwidget_coreMore__ = new QTabWidget();

    createPageOPF();
    //createPageHPF();


    widget_page_opf__->setLayout(gridlayout_opf__);

    tabwidget_coreMore__->insertTab(0, widget_page_opf__, "OPF");
    //tabwidget_coreMore__->insertTab(1, widget_page_hpf__, "HPF");


    dockwidget_coreMode__->setWidget(tabwidget_coreMore__);
    dockwidget_coreMode__->setObjectName("DockWidget_CoreMode");
    addDockWidget(Qt::LeftDockWidgetArea, dockwidget_coreMode__);

    toggleCoreModeAct__ = dockwidget_coreMode__->toggleViewAction();
    toggleCoreModeAct__->setIcon(QIcon(":icons/qrcAlias_advancedSettings.ico"));
    toggleCoreModeAct__->setIconText(tr("&Settings"));


}


void MPL_MainWindow::createPageHPF()
{

}




void MPL_MainWindow::createPageOPF_objectsListsGroup()
{
    gridlayout_opf_objects__ = new QGridLayout();
    label_enemyRobot__ = new QLabel(tr("Enemy\nRobot"));
    label_fixedObstacle__ = new QLabel(tr("Fixed\nObstacle"));
    label_movableObstacle__ = new QLabel(tr("Movable\nObstacle"));
    label_goal__ = new QLabel(tr("Goal"));

    //ER
    lineedit_ER[0] = new QLineEdit("0.000");
    lineedit_ER[0]->setValidator(doublevalidator_positive__);

    lineedit_ER[1] = new QLineEdit("0.000");
    lineedit_ER[1]->setValidator(doublevalidator_positive__);

    lineedit_ER[2] = new QLineEdit("0.000");
    lineedit_ER[2]->setValidator(doublevalidator_positive__);

    pushbutton_ER_add__ = new QPushButton();
    pushbutton_ER_add__->setIcon( QIcon(":icons/qrcAlias_add.ico") );
    pushbutton_ER_add__->setIconSize(QSize(20, 20));

    pushbutton_ER_clear__ = new QPushButton();
    pushbutton_ER_clear__->setIcon( QIcon(":icons/qrcAlias_clear.ico") );
    pushbutton_ER_clear__->setIconSize(QSize(20, 20));

    pushbutton_ER_recycle__ = new QPushButton();
    pushbutton_ER_recycle__->setIcon( QIcon(":icons/qrcAlias_recycle.ico") );
    pushbutton_ER_recycle__->setIconSize(QSize(20, 20));



    //FO
    lineedit_FO[0] = new QLineEdit("0.000");
    lineedit_FO[0]->setValidator(doublevalidator_positive__);

    lineedit_FO[1] = new QLineEdit("0.000");
    lineedit_FO[1]->setValidator(doublevalidator_positive__);

    lineedit_FO[2] = new QLineEdit("0.000");
    lineedit_FO[2]->setValidator(doublevalidator_positive__);

    pushbutton_FO_add__ = new QPushButton();
    pushbutton_FO_add__->setIcon( QIcon(":icons/qrcAlias_add.ico") );
    pushbutton_FO_add__->setIconSize(QSize(20, 20));

    pushbutton_FO_clear__ = new QPushButton();
    pushbutton_FO_clear__->setIcon( QIcon(":icons/qrcAlias_clear.ico") );
    pushbutton_FO_clear__->setIconSize(QSize(20, 20));

    pushbutton_FO_recycle__ = new QPushButton();
    pushbutton_FO_recycle__->setIcon( QIcon(":icons/qrcAlias_recycle.ico") );
    pushbutton_FO_recycle__->setIconSize(QSize(20, 20));



    //FO
    lineedit_MO[0] = new QLineEdit("0.000");
    lineedit_MO[0]->setValidator(doublevalidator_positive__);

    lineedit_MO[1] = new QLineEdit("0.000");
    lineedit_MO[1]->setValidator(doublevalidator_positive__);

    lineedit_MO[2] = new QLineEdit("0.000");
    lineedit_MO[2]->setValidator(doublevalidator_positive__);

    pushbutton_MO_add__ = new QPushButton();
    pushbutton_MO_add__->setIcon( QIcon(":icons/qrcAlias_add.ico") );
    pushbutton_MO_add__->setIconSize(QSize(20, 20));

    pushbutton_MO_clear__ = new QPushButton();
    pushbutton_MO_clear__->setIcon( QIcon(":icons/qrcAlias_clear.ico") );
    pushbutton_MO_clear__->setIconSize(QSize(20, 20));

    pushbutton_MO_recycle__ = new QPushButton();
    pushbutton_MO_recycle__->setIcon( QIcon(":icons/qrcAlias_recycle.ico") );
    pushbutton_MO_recycle__->setIconSize(QSize(20, 20));


    //G
    lineedit_G[0] = new QLineEdit("0.000");
    lineedit_G[0]->setValidator(doublevalidator_positive__);

    lineedit_G[1] = new QLineEdit("0.000");
    lineedit_G[1]->setValidator(doublevalidator_positive__);

    lineedit_G[2] = new QLineEdit("0.000");
    lineedit_G[2]->setValidator(doublevalidator_positive__);

    pushbutton_G_add__ = new QPushButton();
    pushbutton_G_add__->setIcon( QIcon(":icons/qrcAlias_add.ico") );
    pushbutton_G_add__->setIconSize(QSize(20, 20));

    pushbutton_G_clear__ = new QPushButton();
    pushbutton_G_clear__->setIcon( QIcon(":icons/qrcAlias_clear.ico") );
    pushbutton_G_clear__->setIconSize(QSize(20, 20));

    pushbutton_G_recycle__ = new QPushButton();
    pushbutton_G_recycle__->setIcon( QIcon(":icons/qrcAlias_recycle.ico") );
    pushbutton_G_recycle__->setIconSize(QSize(20, 20));

    //All
    gridlayout_opf_objects__->addWidget(label_x__, 0, 1, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(label_y__, 0, 2, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(label_z__, 0, 3, 1, 1, Qt::AlignCenter );

    //ER
    gridlayout_opf_objects__->addWidget(label_enemyRobot__, 1, 0, 1, 1, Qt::AlignRight | Qt::AlignVCenter );
    gridlayout_opf_objects__->addWidget(lineedit_ER[0], 1, 1, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(lineedit_ER[1], 1, 2, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(lineedit_ER[2], 1, 3, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(pushbutton_ER_add__, 1, 4, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(pushbutton_ER_recycle__, 1, 5, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(new QLabel("|"), 1, 6, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(pushbutton_ER_clear__, 1, 7, 1, 1, Qt::AlignCenter );

    //FO
    gridlayout_opf_objects__->addWidget(label_fixedObstacle__, 2, 0, 1, 1, Qt::AlignRight | Qt::AlignVCenter );
    gridlayout_opf_objects__->addWidget(lineedit_FO[0], 2, 1, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(lineedit_FO[1], 2, 2, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(lineedit_FO[2], 2, 3, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(pushbutton_FO_add__, 2, 4, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(pushbutton_FO_recycle__, 2, 5, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(new QLabel("|"), 2, 6, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(pushbutton_FO_clear__, 2, 7, 1, 1, Qt::AlignCenter );

    //MO
    gridlayout_opf_objects__->addWidget(label_movableObstacle__, 3, 0, 1, 1, Qt::AlignRight | Qt::AlignVCenter );
    gridlayout_opf_objects__->addWidget(lineedit_MO[0], 3, 1, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(lineedit_MO[1], 3, 2, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(lineedit_MO[2], 3, 3, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(pushbutton_MO_add__, 3, 4, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(pushbutton_MO_recycle__, 3, 5, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(new QLabel("|"), 3, 6, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(pushbutton_MO_clear__, 3, 7, 1, 1, Qt::AlignCenter );

    //G
    gridlayout_opf_objects__->addWidget(label_goal__, 4, 0, 1, 1, Qt::AlignRight | Qt::AlignVCenter );
    gridlayout_opf_objects__->addWidget(lineedit_G[0], 4, 1, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(lineedit_G[1], 4, 2, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(lineedit_G[2], 4, 3, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(pushbutton_G_add__, 4, 4, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(pushbutton_G_recycle__, 4, 5, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(new QLabel("|"), 4, 6, 1, 1, Qt::AlignCenter );
    gridlayout_opf_objects__->addWidget(pushbutton_G_clear__, 4, 7, 1, 1, Qt::AlignCenter );


    gridlayout_opf_objects__->setColumnMinimumWidth(0, 15);
    gridlayout_opf_objects__->setColumnMinimumWidth(1, 60);
    gridlayout_opf_objects__->setColumnMinimumWidth(2, 60);
    gridlayout_opf_objects__->setColumnMinimumWidth(3, 60);
    gridlayout_opf_objects__->setColumnMinimumWidth(4, 5);
    gridlayout_opf_objects__->setColumnMinimumWidth(5, 5);
}

void MPL_MainWindow::createPageOPF_ownRobotGroup()
{

    label_ownRobot__ = new QLabel(tr("Own\nRobot"));
    lineedit_OR[0] = new QLineEdit("0.000");
    lineedit_OR[1] = new QLineEdit("0.000");
    lineedit_OR[2] = new QLineEdit("0.000");
    lineedit_OR_opf[0] = new QLineEdit("0.000"); //x
    lineedit_OR_opf[1] = new QLineEdit("0.000"); //y
    lineedit_OR_opf[2] = new QLineEdit("0.000"); //z
    lineedit_OR_opf[3] = new QLineEdit("0.000"); //epsilon
    pushbutton_OR_set__ = new QPushButton();
    pushbutton_OR_set__->setIcon( QIcon(":icons/qrcAlias_forward.ico") );
    pushbutton_OR_set__->setIconSize(QSize(20, 20));
    pushbutton_OR_recycle__ = new QPushButton();
    pushbutton_OR_recycle__->setIcon( QIcon(":icons/qrcAlias_recycle.ico") );
    pushbutton_OR_recycle__->setIconSize(QSize(20, 20));
    label_epsilon__ = new QLabel(tr("Epsilon"));
    label_v__ = new QLabel(tr("Vector"));
    gridlayout_ownRobot__ = new QGridLayout();

    lineedit_OR[0]->setValidator(doublevalidator_positive__);
    lineedit_OR[1]->setValidator(doublevalidator_positive__);
    lineedit_OR[2]->setValidator(doublevalidator_positive__);
    lineedit_OR_opf[0]->setValidator(doublevalidator_positive__);
    lineedit_OR_opf[1]->setValidator(doublevalidator_positive__);
    lineedit_OR_opf[2]->setValidator(doublevalidator_positive__);
    lineedit_OR_opf[3]->setValidator(doublevalidator_positive__);

    gridlayout_ownRobot__->addWidget(new QLabel("X"), 0, 1, 1, 1, Qt::AlignCenter );
    gridlayout_ownRobot__->addWidget(new QLabel("Y"), 0, 2, 1, 1, Qt::AlignCenter );
    gridlayout_ownRobot__->addWidget(new QLabel("Z"), 0, 3, 1, 1, Qt::AlignCenter );

    gridlayout_ownRobot__->addWidget(label_ownRobot__, 1, 0, 1, 1, Qt::AlignRight | Qt::AlignVCenter );
    gridlayout_ownRobot__->addWidget(lineedit_OR[0], 1, 1, 1, 1, Qt::AlignCenter );
    gridlayout_ownRobot__->addWidget(lineedit_OR[1], 1, 2, 1, 1, Qt::AlignCenter );
    gridlayout_ownRobot__->addWidget(lineedit_OR[2], 1, 3, 1, 1, Qt::AlignCenter );

    gridlayout_ownRobot__->addWidget(label_v__, 2, 0, 1, 1, Qt::AlignRight | Qt::AlignVCenter );
    gridlayout_ownRobot__->addWidget(lineedit_OR_opf[0], 2, 1, 1, 1, Qt::AlignCenter );
    gridlayout_ownRobot__->addWidget(lineedit_OR_opf[1], 2, 2, 1, 1, Qt::AlignCenter );
    gridlayout_ownRobot__->addWidget(lineedit_OR_opf[2], 2, 3, 1, 1, Qt::AlignCenter );

    gridlayout_ownRobot__->addWidget(label_epsilon__, 3, 0, 1, 1, Qt::AlignRight | Qt::AlignVCenter );
    gridlayout_ownRobot__->addWidget(lineedit_OR_opf[3], 3, 1, 1, 1, Qt::AlignCenter );

    gridlayout_ownRobot__->addWidget(pushbutton_OR_set__, 1, 4, 1, 1, Qt::AlignCenter );
    gridlayout_ownRobot__->addWidget(pushbutton_OR_recycle__, 2, 4, 1, 1, Qt::AlignCenter );

}

void MPL_MainWindow::createPageOPF_gridsolve()
{
    gridlayout_opf_gridsolve__ = new QGridLayout();

    lineedit_grid_opf__[0] = new QLineEdit("10");
    lineedit_grid_opf__[0]->setValidator(new QIntValidator(1, 100000));
    lineedit_grid_opf__[0]->setMinimumWidth(80);
    lineedit_grid_opf__[0]->setMaximumWidth(80);

    lineedit_grid_opf__[1] = new QLineEdit("10");
    lineedit_grid_opf__[1]->setValidator(new QIntValidator(1, 100000));
    lineedit_grid_opf__[1]->setMinimumWidth(80);
    lineedit_grid_opf__[1]->setMaximumWidth(80);

    lineedit_grid_opf__[2] = new QLineEdit("10");
    lineedit_grid_opf__[2]->setValidator(new QIntValidator(1, 100000));
    lineedit_grid_opf__[2]->setMinimumWidth(80);
    lineedit_grid_opf__[2]->setMaximumWidth(80);

    lineedit_omegasor_opf__ = new QLineEdit("1.000");
    lineedit_omegasor_opf__->setValidator(doublevalidator_positive__);

    pushbutton_grid_set_opf__ = new QPushButton();
    pushbutton_grid_set_opf__->setIcon( QIcon(":icons/qrcAlias_forward.ico") );
    pushbutton_grid_set_opf__->setIconSize(QSize(20, 20));

    pushbutton_gridsolve_recycle__ = new QPushButton();
    pushbutton_gridsolve_recycle__->setIcon( QIcon(":icons/qrcAlias_recycle.ico") );
    pushbutton_gridsolve_recycle__->setIconSize(QSize(20, 20));

    groupBox_opf_omegaSor__= new QGroupBox();
    hboxLayout_omegaSor__ = new QHBoxLayout();
    pushbutton_omegasor_set_opf__ = new QPushButton();
    pushbutton_omegasor_set_opf__ ->setIcon( QIcon(":icons/qrcAlias_forward.ico") );
    pushbutton_omegasor_set_opf__ ->setIconSize(QSize(20, 20));

    groupBox_solveMode__ = new QGroupBox();
    radioButton__solveMode_gs__ = new QRadioButton(tr("GS"));
    radioButton__solveMode_sor__ = new QRadioButton(tr("SOR"));
    vboxLayout_solveMode__ = new QVBoxLayout();


    groupBox_opf_solveMode2__ = new QGroupBox();
    chechBox_opf_solveMode2_I__ = new QCheckBox("by Max Iteration");
    chechBox_opf_solveMode2_E__ = new QCheckBox("by Max Error");
    gridLayout_opf_solveMode2__ = new QGridLayout();
    lineedit_opf_solveMode_It__ = new QLineEdit("1000");
    lineedit_opf_solveMode_Er__ = new QLineEdit("0.000001");

    chechBox_opf_solveMode2_E__->setChecked(true);
    chechBox_opf_solveMode2_I__->setChecked(true);


    lineedit_opf_solveMode_Er__->setValidator( new QDoubleValidator(0.0000000001, 1000.0, 15) );
    lineedit_opf_solveMode_Er__->setMinimumWidth(150);
    lineedit_opf_solveMode_Er__->setMaximumWidth(250);
    lineedit_opf_solveMode_It__->setValidator( new QIntValidator(1, 100000) );
    lineedit_opf_solveMode_It__->setMinimumWidth(150);
    lineedit_opf_solveMode_It__->setMaximumWidth(250);

    pushbutton_solve_opf__ = new QPushButton("SOLVE!");
    pushbutton_solve_opf__->setMinimumSize(200, 50);


    gridLayout_opf_solveMode2__->addWidget(chechBox_opf_solveMode2_I__,     0, 0, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    gridLayout_opf_solveMode2__->addWidget(lineedit_opf_solveMode_It__,     0, 1, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    gridLayout_opf_solveMode2__->addWidget(chechBox_opf_solveMode2_E__,     1, 0, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    gridLayout_opf_solveMode2__->addWidget(lineedit_opf_solveMode_Er__,     1, 1, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    groupBox_opf_solveMode2__->setLayout(gridLayout_opf_solveMode2__);

    radioButton__solveMode_sor__->setChecked(true);
    vboxLayout_solveMode__->addWidget(radioButton__solveMode_gs__);
    vboxLayout_solveMode__->addWidget(radioButton__solveMode_sor__);
    vboxLayout_solveMode__->addStretch(1);
    groupBox_solveMode__->setLayout(vboxLayout_solveMode__);


    hboxLayout_omegaSor__->addWidget(lineedit_omegasor_opf__);
    hboxLayout_omegaSor__->addWidget(pushbutton_omegasor_set_opf__);
    hboxLayout_omegaSor__->addStretch(1);
    groupBox_opf_omegaSor__->setLayout(hboxLayout_omegaSor__);


    gridlayout_opf_gridsolve__->addWidget(new QLabel("X"),                  0, 1, 1, 1, Qt::AlignCenter );
    gridlayout_opf_gridsolve__->addWidget(new QLabel("Y"),                  0, 2, 1, 1, Qt::AlignCenter );
    gridlayout_opf_gridsolve__->addWidget(new QLabel("Z"),                  0, 3, 1, 1, Qt::AlignCenter );

    gridlayout_opf_gridsolve__->addWidget(new QLabel("Grid"),               1, 0, 1, 1, Qt::AlignRight | Qt::AlignVCenter );
    gridlayout_opf_gridsolve__->addWidget(lineedit_grid_opf__[0],           1, 1, 1, 1, Qt::AlignCenter );
    gridlayout_opf_gridsolve__->addWidget(lineedit_grid_opf__[1],           1, 2, 1, 1, Qt::AlignCenter );
    gridlayout_opf_gridsolve__->addWidget(lineedit_grid_opf__[2],           1, 3, 1, 1, Qt::AlignCenter );
    gridlayout_opf_gridsolve__->addWidget(pushbutton_gridsolve_recycle__,   1, 4, 1, 1, Qt::AlignRight | Qt::AlignVCenter );
    gridlayout_opf_gridsolve__->addWidget(pushbutton_grid_set_opf__,        1, 5, 1, 1, Qt::AlignRight | Qt::AlignVCenter );

    gridlayout_opf_gridsolve__->addWidget(new QLabel("Mode"),               2, 0, 1, 1, Qt::AlignRight | Qt::AlignVCenter );
    gridlayout_opf_gridsolve__->addWidget(groupBox_solveMode__,             2, 1, 1, 1, Qt::AlignLeft | Qt::AlignVCenter );
    gridlayout_opf_gridsolve__->addWidget(groupBox_opf_solveMode2__,        2, 2, 1, 3, Qt::AlignLeft | Qt::AlignVCenter);

    gridlayout_opf_gridsolve__->addWidget(new QLabel("Omega\nSor"),         3, 0, 1, 1, Qt::AlignRight | Qt::AlignVCenter );
    //gridlayout_opf_gridsolve__->addWidget(lineedit_omegasor_opf__,          3, 1, 1, 1, Qt::AlignRight | Qt::AlignVCenter );
    //gridlayout_opf_gridsolve__->addWidget(pushbutton_omegasor_set_opf__,    3, 2, 1, 1, Qt::AlignLeft | Qt::AlignVCenter );
    gridlayout_opf_gridsolve__->addWidget(groupBox_opf_omegaSor__,          3, 1, 1, 2, Qt::AlignLeft | Qt::AlignVCenter);


    gridlayout_opf_gridsolve__->addWidget(pushbutton_solve_opf__,           4, 0, 1, 6 );
}

void MPL_MainWindow::createPageOPF()
{

    //Creating things
    gridlayout_opf__ = new QGridLayout();
    groupbox_objectLists__ = new QGroupBox(tr("Object Lists"));
    groupbox_ownRobot__ = new QGroupBox(tr("Own Robot"));
    groupbox_gridsolve__ = new QGroupBox(tr("Grid and Solve"));


    //labels used by a lot of stuff... so they need to be here!
    label_x__ = new QLabel(tr("X"));
    label_y__ = new QLabel(tr("Y"));
    label_z__ = new QLabel(tr("Z"));

    createPageOPF_ownRobotGroup();
    createPageOPF_objectsListsGroup();
    createPageOPF_gridsolve();



    groupbox_ownRobot__->setLayout(gridlayout_ownRobot__);
    groupbox_objectLists__->setLayout(gridlayout_opf_objects__);
    groupbox_gridsolve__->setLayout(gridlayout_opf_gridsolve__);

    //set larger groups here!
    gridlayout_opf__->addWidget(groupbox_ownRobot__, 0, 0, 1, 1);
    gridlayout_opf__->addWidget(groupbox_objectLists__, 1, 0, 1, 1);
    gridlayout_opf__->addWidget(groupbox_gridsolve__, 2, 0, 1, 1);



}



void MPL_MainWindow::createActions()
{
    exitAct__ = new QAction(QIcon(":icons/qrcAlias_exit.ico"), tr("E&xit"), this);
    exitAct__->setShortcuts(QKeySequence::Quit);
    exitAct__->setStatusTip(tr("Exit the application"));
    connect(exitAct__, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct__ = new QAction(QIcon(":icons/qrcAlias_info.ico"), tr("&About"), this);
    aboutAct__->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct__, SIGNAL(triggered()), this, SLOT(slot_about()));

    helpAct__ = new QAction(QIcon(":icons/qrcAlias_help.ico"), tr("&Help"), this);
    helpAct__->setStatusTip(tr("Help"));
    connect(helpAct__, SIGNAL(triggered()), this, SLOT(slot_help()));

    verboseAct__ = new QAction(QIcon(":icons/qrcAlias_verbose.ico"), tr("&Verbose"), this);
    helpAct__->setStatusTip(tr("Toggle Verbose mode"));
    b_verbose__ = false;
    connect(verboseAct__, SIGNAL(triggered(bool)), this, SLOT(slotgui_Verbose()) );

    snapshotAct__ = new QAction(QIcon(":icons/qrcAlias_screenshot.ico"), tr("Snapshot"), this);
    snapshotAct__->setStatusTip(tr("Takes a snapshot from the Vector Field Viewer"));
    connect(snapshotAct__, SIGNAL(triggered(bool)), vfViewer__, SLOT(snapshot()) );
}




void MPL_MainWindow::createMenus()
{
    fileMenu__ = menuBar()->addMenu(tr("&File"));
    fileMenu__->addAction(exitAct__);

    viewMenu__ = menuBar()->addMenu(tr("&View"));
    viewMenu__->addAction(toggleLogAct__);
    viewMenu__->addAction(toggleCoreModeAct__);
    viewMenu__->addAction(toggle3DViewsAct__);
    viewMenu__->addSeparator();
    viewMenu__->addAction(verboseAct__);
    viewMenu__->addSeparator();
    viewMenu__->addAction(snapshotAct__);

    helpMenu__ = menuBar()->addMenu(tr("&Help"));
    helpMenu__->addAction(helpAct__);
    helpMenu__->addAction(aboutAct__);
}

void MPL_MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}



void MPL_MainWindow::setConnections()
{


    connect(this, SIGNAL(signal_errorMessage(QString)), this, SLOT(slot_errorMessage(QString)));
    connect(this, SIGNAL(signal_logMessage(QString)), this, SLOT(slot_logMessage(QString)));

    //GUI Connections

    //Controls
        //OPF
            //Grid
    connect(pushbutton_gridsolve_recycle__, SIGNAL(released()), this, SLOT(slotgui_OPF_RecycleGrid()) );
    connect(pushbutton_grid_set_opf__, SIGNAL(released()), this, SLOT(slotgui_OPF_SetGrid()) );
    connect(pushbutton_omegasor_set_opf__, SIGNAL(released()), this, SLOT(slotgui_OPF_SetOmega()) );
    connect(radioButton__solveMode_gs__, SIGNAL(clicked(bool)), this, SLOT(slotgui_OPF_modeGS()) );
    connect(radioButton__solveMode_sor__, SIGNAL(clicked(bool)), this, SLOT(slotgui_OPF_modeSOR()) );
    connect(chechBox_opf_solveMode2_E__, SIGNAL(toggled(bool)), lineedit_opf_solveMode_Er__, SLOT(setEnabled(bool)) );
    connect(chechBox_opf_solveMode2_I__, SIGNAL(toggled(bool)), lineedit_opf_solveMode_It__, SLOT(setEnabled(bool)) );
    connect(pushbutton_solve_opf__, SIGNAL(clicked(bool)), this, SLOT(slotgui_OPF_Solve()) );

    QTimer::singleShot(1000, this, SLOT( slotgui_delayedInit() )  );//1s-1000ms to call delayed init to set initial state.

            //Objects lists
                //OR
    connect(pushbutton_OR_recycle__, SIGNAL(released()), this, SLOT(slotgui_OPF_OR_Recycle()) );
    connect(pushbutton_OR_set__, SIGNAL(released()), this, SLOT(slotgui_OPF_OR_Set()) );
                //ER
    connect(pushbutton_ER_add__, SIGNAL(released()), this, SLOT(slotgui_OPF_ER_Add()) );
    connect(pushbutton_ER_clear__, SIGNAL(released()), this, SLOT(slotgui_OPF_ER_ClearList()) );
    connect(pushbutton_ER_recycle__, SIGNAL(released()), this, SLOT(slotgui_OPF_ER_Recycle()) );
                //FO
    connect(pushbutton_FO_add__, SIGNAL(released()), this, SLOT(slotgui_OPF_FO_Add()) );
    connect(pushbutton_FO_clear__, SIGNAL(released()), this, SLOT(slotgui_OPF_FO_ClearList()) );
    connect(pushbutton_FO_recycle__, SIGNAL(released()), this, SLOT(slotgui_OPF_FO_Recycle()) );
                //MO
    connect(pushbutton_MO_add__, SIGNAL(released()), this, SLOT(slotgui_OPF_MO_Add()) );
    connect(pushbutton_MO_clear__, SIGNAL(released()), this, SLOT(slotgui_OPF_MO_ClearList()) );
    connect(pushbutton_MO_recycle__, SIGNAL(released()), this, SLOT(slotgui_OPF_MO_Recycle()) );
                //ER
    connect(pushbutton_G_add__, SIGNAL(released()), this, SLOT(slotgui_OPF_G_Add()) );
    connect(pushbutton_G_clear__, SIGNAL(released()), this, SLOT(slotgui_OPF_G_ClearList()) );
    connect(pushbutton_G_recycle__, SIGNAL(released()), this, SLOT(slotgui_OPF_G_Recycle()) );



    //Vector Field Viewer
    connect(vfViewer__, SIGNAL(signal_errorMessage(QString)), this, SLOT(slot_errorMessage(QString)));
    connect(vfViewer__, SIGNAL(signal_logMessage(QString)), this, SLOT(slot_logMessage(QString)));
    connect(this, SIGNAL(signalviewer_rotacoes(int,int)), vfViewer__, SLOT(slot_rotacoes(int,int)) );
}





void MPL_MainWindow::readSettings()
{
    QSettings settings("LAR", "MOPLALIB");
    restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
    restoreState(settings.value("MainWindow/windowState").toByteArray());

    //qDebug() << settings.fileName();
}

void MPL_MainWindow::writeSettings()
{
    QSettings settings("LAR", "MOPLALIB");
    settings.setValue("MainWindow/geometry", saveGeometry());
    settings.setValue("MainWindow/windowState", saveState());



}


void MPL_MainWindow::slot_about()
{
    QMessageBox messageBox;
    messageBox.setText(tr("Motion Planning Library"));
    messageBox.setIcon(QMessageBox::Information);
    messageBox.setWindowTitle(tr("About MOPLALIB"));
    messageBox.exec();
    //QMessageBox::about(this, tr("About MOPLALIB"),tr("Motion Planning Library"));
    //emit signal_errorMessage(tr("Slot About Called! Error test!"));
}


void MPL_MainWindow::slot_open()
{
//    QString fileName = QFileDialog::getOpenFileName(this);
//        if (!fileName.isEmpty())
//            loadFile(fileName);

}




void MPL_MainWindow::slot_help()
{
    QMessageBox::about(this, tr("Help"),tr("See ..."));
    emit signal_logMessage(tr("Slot Help Called! Log test!"));
}



void MPL_MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    emit signal_closedGUI();
    QMainWindow::closeEvent(event);
}

void MPL_MainWindow::showEvent(QShowEvent* event)
{
    readSettings();
    QMainWindow::showEvent(event);
}



void MPL_MainWindow::slot_logMessage( QString s )
{
    if(!b_textEdit_logState)
    {
        textEditLog__->setTextBackgroundColor(Qt::white);
        textEditLog__->setTextColor(Qt::black);
        textEditLog__->setFontPointSize(8);
        b_textEdit_logState = !b_textEdit_logState;
    }
    textEditLog__->append(s);
}

void MPL_MainWindow::slot_errorMessage( QString s )
{
    if(b_textEdit_logState)
    {
        textEditLog__->setTextBackgroundColor(Qt::black);
        textEditLog__->setTextColor(Qt::red);
        textEditLog__->setFontPointSize(10);
        b_textEdit_logState = !b_textEdit_logState;
    }
    textEditLog__->append(s);
    statusBar()->showMessage(tr("ERROR"));
}


void MPL_MainWindow::slotgui_delayedInit()
{
    radioButton__solveMode_sor__->click();//more things to come TODO!

}



//GUI SLOTS
void  MPL_MainWindow::slotgui_OPF_OR_Recycle()
{
    if(b_verbose__){
        emit signal_logMessage("MPL_MainWindow::slotgui_OPF_OR_Recycle");
    }
    lineedit_OR[0]->setText("0.000");
    lineedit_OR[1]->setText("0.000");
    lineedit_OR[2]->setText("0.000");
    lineedit_OR_opf[0]->setText("0.000"); //x
    lineedit_OR_opf[1]->setText("0.000"); //y
    lineedit_OR_opf[2]->setText("0.000"); //z
    lineedit_OR_opf[3]->setText("0.000"); //epsilon
}





void MPL_MainWindow::slotgui_Verbose()
{
    if(!b_verbose__){
        verboseAct__->setIcon(QIcon(":icons/qrcAlias_mute.ico"));
        b_verbose__ = true;
        emit signal_logMessage("[Verbose = True]");
    }
    else{
        verboseAct__->setIcon(QIcon(":icons/qrcAlias_verbose.ico"));
        b_verbose__ = false;
        emit signal_logMessage("[Verbose = False]");
    }
}


void MPL_MainWindow::slotgui_OPF_OR_Set()
{

    if(NULL == mplCore__ ){
        emit signal_errorMessage("[MPL_MainWindow::slotgui_OPF_OR_Set] No core set.");
        return;
    }


    //Will convert comma to point, disregard of locale... potential @bug here.
    //Its useful because I keep putting commas instead points and fucking with the program.
    //QByteArray ba = lineedit_OR_opf[3]->text().toLatin1();
    //qWarning("Text Before \"%s\"", ba.data() );
    //ba = lineedit_OR_opf[3]->text().replace(",", ".").toLatin1();
    //qWarning("Text After \"%s\"", ba.data() );

    emit signalcore_setEpsilon_OPF( lineedit_OR_opf[3]->text().replace(",", ".").toDouble() );

    emit signalcore_setOwnRobot(    lineedit_OR[0]->text().replace(",", ".").toDouble(),\
                                    lineedit_OR[1]->text().replace(",", ".").toDouble(),\
                                    lineedit_OR[2]->text().replace(",", ".").toDouble());

    emit signalcore_setVector_OPF(  lineedit_OR_opf[0]->text().replace(",", ".").toDouble(),\
                                    lineedit_OR_opf[1]->text().replace(",", ".").toDouble(),\
                                    lineedit_OR_opf[2]->text().replace(",", ".").toDouble() );//only OPF

}



void MPL_MainWindow::slotgui_OPF_SetGrid()
{
    if(NULL == mplCore__ ){
        emit signal_errorMessage("[MPL_MainWindow::slotgui_OPF_SetGrid] No core set. ");
        return;
    }



    emit signalcore_setGridSize(    (int) lineedit_grid_opf__[0]->text().replace(",", ".").toDouble(),\
                                    (int) lineedit_grid_opf__[1]->text().replace(",", ".").toDouble(),\
                                    (int) lineedit_grid_opf__[2]->text().replace(",", ".").toDouble());

}

void MPL_MainWindow::slotgui_OPF_RecycleGrid()
{
    if(b_verbose__){
        emit signal_logMessage("[MPL_MainWindow::slotgui_OPF_RecycleGrid]");
    }

    lineedit_grid_opf__[0]->setText("10.000");
    lineedit_grid_opf__[1]->setText("10.000");
    lineedit_grid_opf__[2]->setText("10.000");
}

void MPL_MainWindow::slotgui_OPF_SetOmega()
{
    if(NULL == mplCore__ ){
        emit signal_errorMessage("[MPL_MainWindow::slotgui_OPF_SetOmega] No core set. ");
        return;
    }

    emit signalcore_setOmega(lineedit_omegasor_opf__->text().replace(",", ".").toDouble());

}



//ER

void MPL_MainWindow::slotgui_OPF_ER_Add()
{
    if(NULL == mplCore__ ){
        emit signal_errorMessage("[MPL_MainWindow::slotgui_OPF_ER_Add] No core set. ");
        return;
    }

    emit signalcore_addEnemyRobot(  lineedit_ER[0]->text().replace(",", ".").toDouble(),\
                                    lineedit_ER[1]->text().replace(",", ".").toDouble(),\
                                    lineedit_ER[2]->text().replace(",", ".").toDouble() );
}

void MPL_MainWindow::slotgui_OPF_ER_ClearList()
{
    if(NULL == mplCore__ ){
        emit signal_errorMessage("[MPL_MainWindow::slotgui_OPF_ER_ClearList] No core set. ");
        return;
    }

    emit signalcore_clearListOfEnemyRobot();
}

void MPL_MainWindow::slotgui_OPF_ER_Recycle()
{
    if(b_verbose__){
        emit signal_logMessage("MPL_MainWindow::slotgui_OPF_ER_Recycle");
    }

    lineedit_ER[0]->setText("0.000");
    lineedit_ER[1]->setText("0.000");
    lineedit_ER[2]->setText("0.000");
}


//FO

void MPL_MainWindow::slotgui_OPF_FO_Add()
{
    if(NULL == mplCore__ ){
        emit signal_errorMessage("[MPL_MainWindow::slotgui_OPF_FO_Add] No core set. ");
        return;
    }

    emit signalcore_addFixedObstacle(   lineedit_FO[0]->text().replace(",", ".").toDouble(),\
                                        lineedit_FO[1]->text().replace(",", ".").toDouble(),\
                                        lineedit_FO[2]->text().replace(",", ".").toDouble() );
}

void MPL_MainWindow::slotgui_OPF_FO_ClearList()
{
    if(NULL == mplCore__ ){
        emit signal_errorMessage("[MPL_MainWindow::slotgui_OPF_FO_ClearList] No core set. ");
        return;
    }

    emit signalcore_clearListOfFixedObstacle();
}

void MPL_MainWindow::slotgui_OPF_FO_Recycle()
{
    if(b_verbose__){
        emit signal_logMessage("MPL_MainWindow::slotgui_OPF_FO_Recycle");
    }

    lineedit_FO[0]->setText("0.000");
    lineedit_FO[1]->setText("0.000");
    lineedit_FO[2]->setText("0.000");
}


//MO

void MPL_MainWindow::slotgui_OPF_MO_Add()
{
    if(NULL == mplCore__ ){
        emit signal_errorMessage("[MPL_MainWindow::slotgui_OPF_MO_Add] No core set. ");
        return;
    }

    emit signalcore_addMovableObstacle( lineedit_MO[0]->text().replace(",", ".").toDouble(),\
                                        lineedit_MO[1]->text().replace(",", ".").toDouble(),\
                                        lineedit_MO[2]->text().replace(",", ".").toDouble() );
}

void MPL_MainWindow::slotgui_OPF_MO_ClearList()
{
    if(NULL == mplCore__ ){
        emit signal_errorMessage("[MPL_MainWindow::slotgui_OPF_MO_ClearList] No core set. ");
        return;
    }

    emit signalcore_clearListOfMovableObstacle();
}

void MPL_MainWindow::slotgui_OPF_MO_Recycle()
{
    if(b_verbose__){
        emit signal_logMessage("MPL_MainWindow::slotgui_OPF_MO_Recycle");
    }

    lineedit_MO[0]->setText("0.000");
    lineedit_MO[1]->setText("0.000");
    lineedit_MO[2]->setText("0.000");
}


//G

void MPL_MainWindow::slotgui_OPF_G_Add()
{
    if(NULL == mplCore__ ){
        emit signal_errorMessage("[MPL_MainWindow::slotgui_OPF_G_Add] No core set. ");
        return;
    }

    emit signalcore_addGoal(    lineedit_G[0]->text().replace(",", ".").toDouble(),\
                                lineedit_G[1]->text().replace(",", ".").toDouble(),\
                                lineedit_G[2]->text().replace(",", ".").toDouble() );
}

void MPL_MainWindow::slotgui_OPF_G_ClearList()
{
    if(NULL == mplCore__ ){
        emit signal_errorMessage("[MPL_MainWindow::slotgui_OPF_G_ClearList] No core set. ");
        return;
    }

    emit signalcore_clearListOfGoal();
}

void MPL_MainWindow::slotgui_OPF_G_Recycle()
{
    if(b_verbose__){
        emit signal_logMessage("MPL_MainWindow::slotgui_OPF_G_Recycle");
    }

    lineedit_G[0]->setText("0.000");
    lineedit_G[1]->setText("0.000");
    lineedit_G[2]->setText("0.000");
}



void MPL_MainWindow::slotgui_OPF_modeGS()
{
    groupBox_opf_omegaSor__->setEnabled(false);
    emit signalcore_setSolveMode(MPL_Core_SV_OPF_GS);
}

void MPL_MainWindow::slotgui_OPF_modeSOR()
{
    groupBox_opf_omegaSor__->setEnabled(true);
    emit signalcore_setSolveMode(MPL_Core_SV_OPF_SOR);
}


void MPL_MainWindow::slotgui_OPF_Solve()
{
    if( chechBox_opf_solveMode2_I__->isChecked() )
    {
        if( chechBox_opf_solveMode2_E__->isChecked() )
        {
            emit signalcore_solveIE(    lineedit_opf_solveMode_It__->text().replace(",", ".").toInt(),\
                                        lineedit_opf_solveMode_Er__->text().replace(",", ".").toDouble());
        }
        else
        {
            emit signalcore_solveI(     lineedit_opf_solveMode_It__->text().replace(",", ".").toInt());
        }
    }
    else
    {
        if( chechBox_opf_solveMode2_E__->isChecked() )
        {
            emit signalcore_solveE(     lineedit_opf_solveMode_Er__->text().replace(",", ".").toDouble());
        }
        else
        {
            emit signal_errorMessage("No convergency criteria selected!");
        }
    }
}

