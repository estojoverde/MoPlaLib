#include <cstdio>
#include <cstring>

#include <QObject>
#include <QApplication>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>

#include "mpl_utils.h"
#include "mpl_position.h"
#include "mpl_baseenvironment.h"
#include "mpl_gridenvironment.h"
#include "mpl_potentialfieldenvironment.h"
#include "mpl_khatibpotentialfield.h"
#include "mpl_harmonicpotentialfield.h"
#include "mpl_orientedpotentialfield.h"
#include "mpl_locallyorientedpotentialfield.h"
#include "mpl_wavefront.h"
//#include "mpl_vectorfieldviewer.h"
#include "mpl_core.h"
//#include "mpl_mainwindow.h"
#include "vis_mainwindow.h"

#include "mpl_testsuite.h"


#include "mpl_opf_parallelsor_mainlaucher.h"

//#include "vis_scene3d.h"


QReadWriteLock pf_locker;

int main(int argc, char **argv)
{



    Q_INIT_RESOURCE(icons);

    QApplication app(argc, argv);

    qRegisterMetaType<MPL_Core_EnvironmentMode>();
    qRegisterMetaType<MPL_Core_SolveMode>();

    MPL_Core *core = new MPL_Core();
    VIS_MainWindow *mainWindow = new VIS_MainWindow();
    QThread qthread_coreThread;

    core->moveToThread(&qthread_coreThread);
    qthread_coreThread.start(QThread::HighestPriority);

    QObject::connect(mainWindow, SIGNAL(signal_closedGUI()), core, SLOT(deleteLater()) );
    QObject::connect(mainWindow, SIGNAL(signal_closedGUI()), &qthread_coreThread, SLOT(quit()) );

    core->slot_setLocker(&pf_locker);
    mainWindow->setCore(core, &pf_locker);


    MPL_OPF_ParallelSor_MainLaucher *blabla = new MPL_OPF_ParallelSor_MainLaucher();
    delete blabla;


    mainWindow->show();

//    VIS_Scene3D *visaux = new VIS_Scene3D();
//    visaux->show();

//    MPL_TestSuite *teste = new MPL_TestSuite();
//    teste->show();
//    QObject::connect(teste, SIGNAL(signal_done()), &app, SLOT(quit()));


//    if( 3 == argc){
//        if( (argv[1][0] == 'h') || (argv[1][0] == 'H') ){
//            teste->testHPF(argv[2]);
//        }
//        else{
//            if( (argv[1][0] == 'o') || (argv[1][0] == 'O') ){
//                teste->testOPF(argv[2]);
//            }
//        }
//    }else{
//        qWarning("No arguments! Try %s hpf somefile.hpf", argv[0]);
//        MPL_ERROR_MESSAGE("No arguments! Try %s [hpf/opf] somefile.hpf", argv[0]);
//    }

    //testBase();
    //testGrid();

    //teste->testHPF("/tmp/teste1.hpf");

    //QTimer::singleShot(120000, &app, SLOT(quit()));


    //MPL_VectorFieldViewer *viewer = new MPL_VectorFieldViewer();
    //viewer->show();


    return app.exec();



    //return(0);
}



