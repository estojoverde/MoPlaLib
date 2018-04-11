#include "mpl_testsuite.h"

MPL_TestSuite::MPL_TestSuite(QWidget *parent) : QMainWindow(parent)
{

    QLW_log = new QListWidget();

    setCentralWidget(QLW_log);

    connect( this, SIGNAL(newLogMessageSignal(QString)), this, SLOT(newLogMessage(QString)) );

    connect( this, SIGNAL(signal_done()), this, SLOT(deleteLater()) );

}

MPL_TestSuite::~MPL_TestSuite()
{

    delete QLW_log;

}




void MPL_TestSuite::testPosition()
{

    MPL_Position *p_mpl_p[10];

    p_mpl_p[0] = new MPL_Position();
    p_mpl_p[1] = new MPL_Position(0,0,0);
    p_mpl_p[2] = new MPL_Position(1,1,1);
    p_mpl_p[3] = new MPL_Position(-1,-1,-1);
    p_mpl_p[4] = new MPL_Position(8,0,-8);
    p_mpl_p[5] = new MPL_Position(-7,3,9);
    p_mpl_p[6] = new MPL_Position(0,-2,0);
    p_mpl_p[7] = new MPL_Position();








    return;
}




int MPL_TestSuite::testGrid(void)
{
    FILE *fp=fopen("/tmp/testdump.txt", "w");
    MPL_GridEnvironment *grid;
    MPL_Position p[10];
    int aux[10];

    grid = new MPL_GridEnvironment(15, 15, 3);
    fprintf(fp, "############################\n%d", 1);
    grid->unsafe_dumpOccupancyGrid(fp);

    p[0].setXYZ(5, 5, 0);
    grid->addEnemyRobot(p[0]);

    p[1].setXYZ(10, 10, 2);
    grid->setOwnRobot(p[1]);

    grid->getOwnRobot(p[2]);
    if(p[2] == p[1]){
        fprintf(fp, "\np[2]==p[1]\nThey Are EQUAL!!!! UHU! \n\n");
    }
    else{
        fprintf(fp, "\np[2]!=p[1]\n ERROR! \n\n");
    }


    grid->getSizeListOfEnemyRobot(aux[0]);
    fprintf(fp, "\n Size Enemy Robot List= %d\n\n", aux[0]);

    p[3].setXYZ(7,7,1);
    grid->addEnemyRobot(p[3]);

    p[4].setXYZ(7,9,1);
    grid->addEnemyRobot(p[4]);

    p[5].setXYZ(7,11,1);
    grid->addEnemyRobot(p[5]);

    p[6].setXYZ(7,5,1);
    grid->addEnemyRobot(p[6]);

    p[7].setXYZ(7,3,1);
    grid->addEnemyRobot(p[7]);

    grid->getSizeListOfEnemyRobot(aux[1]);
    fprintf(fp, "\n Size Enemy Robot List= %d\n\n", aux[1]);



    grid->getEnemyRobotAt(p[8], 3);
    if(p[8] == p[5]){
        fprintf(fp, "\np[8]==p[5]\nThey Are EQUAL!!!! UHU! \n\n");
    }
    else{
        fprintf(fp, "\np[8]!=p[5]\n ERROR! \n\n");
    }


    grid->getSizeListOfEnemyRobot(aux[2]);
    fprintf(fp, "\n Size Enemy Robot List= %d\n\n", aux[2]);

    grid->deleteEnemyRobotAt(3);
    grid->getEnemyRobotAt(p[9], 3);
    if(p[9] != p[5]){
        fprintf(fp, "\np[9]!=p[5]\nThey Are DIFFERENT!!!! UHU! \n\n");
    }
    else{
        fprintf(fp, "\np[9]==p[5]\n ERROR! \n\n");
    }

    grid->getSizeListOfEnemyRobot(aux[3]);
    fprintf(fp, "\n Size Enemy Robot List= %d\n\n", aux[3]);

    grid->clearListOfEnemyRobot();
    grid->getSizeListOfEnemyRobot(aux[4]);
    fprintf(fp, "\n Size Enemy Robot List= %d\n\n", aux[4]);


    fprintf(fp, "############################\n%d", 2);
    grid->unsafe_dumpOccupancyGrid(fp);


    fflush(fp);
    fclose(fp);


    delete grid;
    return(0);

}




int MPL_TestSuite::testBase(void)
{
    MPL_BaseEnvironment *b;
    b = new MPL_BaseEnvironment();

    int aux, aux_v[20];

    MPL_Position p[20];

    b->getSizeListOfEnemyRobot(aux);
    qWarning("getSizeListOfEnemyRobot(%d)", aux );

    p[0].setXYZ(20,15,30);
    b->addEnemyRobot(p[0]);

    b->getSizeListOfEnemyRobot(aux);
    qWarning("getSizeListOfEnemyRobot(%d)", aux );

    b->getEnemyRobotAt(p[1], 0);
    qWarning("(%lf, %lf, %lf)", p[1].getX(), p[1].getY(), p[1].getZ() );

    b->deleteEnemyRobotAt(1);
    b->getSizeListOfEnemyRobot(aux);
    qWarning("getSizeListOfEnemyRobot(%d)", aux );

    p[2].setXYZ(-1,-1,-1);
    qWarning("(%lf, %lf, %lf)", p[2].getX(), p[2].getY(), p[2].getZ() );

    p[2] = p[1] -p[0];
    qWarning("(%lf, %lf, %lf)", p[2].getX(), p[2].getY(), p[2].getZ() );

    p[2]-=p[0];
    qWarning("(%lf, %lf, %lf)", p[2].getX(), p[2].getY(), p[2].getZ() );


    b->deleteEnemyRobotAt(0);
    b->getSizeListOfEnemyRobot(aux);
    qWarning("getSizeListOfEnemyRobot(%d)", aux );

    delete b;

    return(0);
}



int MPL_TestSuite::testHPF(char *s_filename)
{
    int x_size, y_size, z_size;
    char s_logfile_name[50], s_resultsfile_name[50], s_logfile_mode[4], s_resultsfile_mode[4];
    char c_action;
    double d_x, d_y, d_z;
    bool b_stop=false;
    int n_maxIt;
    double d_maxError;
    MPL_HPF_IterateModes hpf_itMode;
    struct timeval ti, tf;
    double d_omegaSor;
    bool b_log=true;

    int i_itReal;
    double d_errorReal;



    FILE *fp_logfile, *fp_resultsfile, *fp_plotfile;

    FILE *fp=fopen(s_filename, "r");
    if( NULL == fp ){
        MPL_ERROR_MESSAGE("File %s not found!", s_filename);
        return(-1);
    }
    fscanf(fp, "%d %d %d\n", &x_size, &y_size, &z_size);

    MPL_HarmonicPotentialField *hpf = new MPL_HarmonicPotentialField(x_size, y_size, z_size);

    fscanf(fp, "%s %s\n", s_logfile_name, s_logfile_mode);

    fscanf(fp, "%s %s\n", s_resultsfile_name, s_resultsfile_mode);

    fp_logfile=fopen(s_logfile_name, s_logfile_mode);
    if( NULL == fp_logfile ){
        MPL_ERROR_MESSAGE("Error while opening %s file!", s_filename);
        fclose(fp);
        return(-1);
    }

    fp_resultsfile=fopen(s_resultsfile_name, s_resultsfile_mode);
    if( NULL == fp_resultsfile ){
        MPL_ERROR_MESSAGE("Error while opening %s file!", s_resultsfile_name);
        fclose(fp_logfile);
        fclose(fp);
        return(-1);
    }

    fp_plotfile=fopen("cp3dgnuplot.data", "w");
    if( NULL == fp_plotfile ){
        MPL_ERROR_MESSAGE("Error while opening %s file!", "plot.gnu");
        fclose(fp_resultsfile);
        fclose(fp_logfile);
        fclose(fp);
        return(-1);
    }


    while( (!feof(fp)) && (!b_stop) ){
        fscanf(fp, "%c ", &c_action);
        switch (c_action) {
        case 'a':
            fscanf(fp, "%c %lf %lf %lf\n", &c_action, &d_x, &d_y, &d_z);
            switch (c_action) {
            case 'e':
                hpf->addEnemyRobot(MPL_Position(d_x, d_y, d_z));
                if(b_log){MPL_TEST_LOG_MESSAGE("[ADD] Enemy Robot (%lf, %lf, %lf)", d_x, d_y, d_z);}
                break;
            case 'f':
                hpf->addFixedObstacle(MPL_Position(d_x, d_y, d_z));
                if(b_log){MPL_TEST_LOG_MESSAGE("[ADD] Fixed Obstacle (%lf, %lf, %lf)", d_x, d_y, d_z);}
                break;
            case 'g':
                hpf->addGoal(MPL_Position(d_x, d_y, d_z));
                if(b_log){MPL_TEST_LOG_MESSAGE("[ADD] Goal (%lf, %lf, %lf)", d_x, d_y, d_z);}
                break;
            case 'm':
                hpf->addMovableObstacle(MPL_Position(d_x, d_y, d_z));
                if(b_log){MPL_TEST_LOG_MESSAGE("[ADD] Movable Obstacle (%lf, %lf, %lf)", d_x, d_y, d_z);}
                break;
            case 'o':
                hpf->setOwnRobot(MPL_Position(d_x, d_y, d_z));
                if(b_log){MPL_TEST_LOG_MESSAGE("[ADD] Own Robot (%lf, %lf, %lf)", d_x, d_y, d_z);}
                break;
            default:
                b_stop=true;
                if(b_log){MPL_TEST_LOG_MESSAGE("[ADD] [ERROR] Unknow char option when adding robot");}
                MPL_ERROR_MESSAGE("Unknow char option when adding robot");
                break;
            }
            break;
        case 'c':
            fscanf(fp, "%c ", &c_action);
            switch (c_action) {
            case 'e':
                hpf->clearListOfEnemyRobot();
                if(b_log){MPL_TEST_LOG_MESSAGE("[CLEAR] Enemy Robot");}
                break;
            case 'f':
                hpf->clearListOfFixedObstacle();
                if(b_log){MPL_TEST_LOG_MESSAGE("[CLEAR] Fixed Obstacle");}
                break;
            case 'g':
                hpf->clearListOfGoal();
                if(b_log){MPL_TEST_LOG_MESSAGE("[CLEAR] Goal");}
                break;
            case 'm':
                hpf->clearListOfMovableObstacle();
                if(b_log){MPL_TEST_LOG_MESSAGE("[CLEAR] Movable Obstacle");}
                break;
            default:
                b_stop=true;
                if(b_log){MPL_TEST_LOG_MESSAGE("[CLEAR] [ERROR] Unknow char option when cleaning list");}
                MPL_ERROR_MESSAGE("Unknow char option when cleaning list");
                break;
            }
            break;
        case 'd':
            fscanf(fp, "%c ", &c_action);
            switch (c_action) {
            case 'o':
                if(b_log){MPL_TEST_LOG_MESSAGE("[DUMP] Occupancy Grid");}
                hpf->unsafe_dumpOccupancyGrid(fp_logfile);
                break;
            case 'p':
                if(b_log){MPL_TEST_LOG_MESSAGE("[DUMP] Potential Grid");}
                hpf->unsafe_dumpPotentialGrid(fp_logfile);
                break;
//            case 'g':
//                if(b_log){MPL_TEST_LOG_MESSAGE("[DUMP] Potential Grid");}
//                hpf->unsafe_toGnuplotPotentialGrid(fp_plotfile);
//                break;
            default:
                b_stop=true;
                if(b_log){MPL_TEST_LOG_MESSAGE("[DUMP] [ERROR] Unknow char option when dumping grid");}
                MPL_ERROR_MESSAGE("Unknow char option when dumping grid");
                break;
            }
            break;
        case 'e':
            if(b_log){MPL_TEST_LOG_MESSAGE("[STOP]");}
            b_stop=true;
            break;
        case 's':
            fscanf(fp, "%c ", &c_action);
            switch (c_action) {
            case 'b':
                fscanf(fp, "%c %d %lf\n", &c_action, &n_maxIt, &d_maxError);
                hpf_itMode = ( 's' == c_action ? MPL_HPF_SOR : MPL_HPF_GS);
                if(b_log){MPL_TEST_LOG_MESSAGE("[SOLVE]\n[ErrorMax=%.17g]\n[IterationMax=%d]\n[Mode:%s]\n", d_maxError, n_maxIt, ( 's' == c_action ? "MPL_HPF_SOR" : "MPL_HPF_GS") );}
                gettimeofday( &ti, NULL);
                hpf->solveIE(hpf_itMode, n_maxIt, d_maxError, i_itReal, d_errorReal);
                gettimeofday( &tf, NULL);
                if(b_log){MPL_TEST_LOG_MESSAGE("[SOLVE]\n[ErrorReal=%.17g]\n[IterationReal=%d]\n[Time(ms): %lf]", d_errorReal, i_itReal, (((1000000*(tf.tv_sec - ti.tv_sec) ) + (tf.tv_usec - ti.tv_usec))/1000.0)); }
                MPL_TEST_RESULT_MESSAGE("[SOLVE]\n[ErrorReal=%.17g]\n[IterationReal=%d]\n[Time(ms): %lf]", d_errorReal, i_itReal, (((1000000*(tf.tv_sec - ti.tv_sec) ) + (tf.tv_usec - ti.tv_usec))/1000.0));
                break;
            case 'e':
                fscanf(fp, "%c %lf\n", &c_action, &d_maxError);
                hpf_itMode = ( 's' == c_action ? MPL_HPF_SOR : MPL_HPF_GS);
                if(b_log){MPL_TEST_LOG_MESSAGE("[SOLVE]\n[ErrorMax=%.17g]\n[IterationMax=%d]\n[Mode:%s]\n", d_maxError, n_maxIt, ( 's' == c_action ? "MPL_HPF_SOR" : "MPL_HPF_GS") );}
                gettimeofday( &ti, NULL);
                hpf->solveE(hpf_itMode, d_maxError, i_itReal, d_errorReal);
                gettimeofday( &tf, NULL);
                if(b_log){MPL_TEST_LOG_MESSAGE("[SOLVE]\n[ErrorReal=%.17g]\n[IterationReal=%d]\n[Time(ms): %lf]", d_errorReal, i_itReal, (((1000000*(tf.tv_sec - ti.tv_sec) ) + (tf.tv_usec - ti.tv_usec))/1000.0)); }
                MPL_TEST_RESULT_MESSAGE("[SOLVE]\n[ErrorReal=%.17g]\n[IterationReal=%d]\n[Time(ms): %lf]", d_errorReal, i_itReal, (((1000000*(tf.tv_sec - ti.tv_sec) ) + (tf.tv_usec - ti.tv_usec))/1000.0));
                break;
            case 'i':
                fscanf(fp, "%c %d\n", &c_action, &n_maxIt);
                hpf_itMode = ( 's' == c_action ? MPL_HPF_SOR : MPL_HPF_GS);
                if(b_log){MPL_TEST_LOG_MESSAGE("[SOLVE]\n[ErrorMax=%.17g]\n[IterationMax=%d]\n[Mode:%s]\n", d_maxError, n_maxIt, ( 's' == c_action ? "MPL_HPF_SOR" : "MPL_HPF_GS") );}
                gettimeofday( &ti, NULL);
                hpf->solveI(hpf_itMode, n_maxIt, i_itReal, d_errorReal);
                gettimeofday( &tf, NULL);
                if(b_log){MPL_TEST_LOG_MESSAGE("[SOLVE]\n[ErrorReal=%.17g]\n[IterationReal=%d]\n[Time(ms): %lf]", d_errorReal, i_itReal, (((1000000*(tf.tv_sec - ti.tv_sec) ) + (tf.tv_usec - ti.tv_usec))/1000.0)); }
                MPL_TEST_RESULT_MESSAGE("[SOLVE]\n[ErrorReal=%.17g]\n[IterationReal=%d]\n[Time(ms): %lf]", d_errorReal, i_itReal, (((1000000*(tf.tv_sec - ti.tv_sec) ) + (tf.tv_usec - ti.tv_usec))/1000.0));
                break;
            default:
                b_stop=true;
                if(b_log){MPL_TEST_LOG_MESSAGE("[SOLVE] [ERROR] Unknow char option when solving BVP");}
                MPL_ERROR_MESSAGE("Unknow char option when solving BVP");
                break;
            }
            break;
        case 'w':
            fscanf(fp, "%lf\n", &d_omegaSor);
            if(b_log){MPL_TEST_LOG_MESSAGE("[SET SOR] [Omega Sor = %lf]", d_omegaSor);}
            hpf->setOmega(d_omegaSor);
            break;
        case 'm': //Verbose
            fscanf(fp, "%c ", &c_action);
            if(b_log){MPL_TEST_LOG_MESSAGE("[SET VERBOSE] Verbose = %s", (c_action=='f' ? "False" : "True") );}
            hpf->setVerbose( c_action=='f' ? false : true );
            break;
        case 'l': //Log
            fscanf(fp, "%c ", &c_action);
            b_log = ( c_action=='f' ? false : true ) ;
            if(b_log){
                MPL_INFORMATION_MESSAGE("[LOG STARTED]");
            }
            else{
                MPL_INFORMATION_MESSAGE("[LOG STOPPED]");
            }
        default:
            b_stop=true;
            if(b_log){MPL_TEST_LOG_MESSAGE("[] [ERROR] Unknow char option switching action");}
            MPL_ERROR_MESSAGE("Unknow char option switching action");
            break;
        }
    }

    //fscanf(fp, "", );

    delete hpf;


    fclose(fp_resultsfile);
    fclose(fp_logfile);
    fclose(fp);

    emit signal_done();

    return(0);
}





int MPL_TestSuite::testOPF(char *s_filename)
{
    int x_size, y_size, z_size;
    char s_logfile_name[50], s_resultsfile_name[50], s_logfile_mode[4], s_resultsfile_mode[4];
    char c_action;
    double d_x, d_y, d_z;
    bool b_stop=false;
    int n_maxIt;
    double d_maxError;
    MPL_OPF_IterateModes opf_itMode;
    struct timeval ti, tf;
    double d_omegaSor;
    double d_epsilon;
    bool b_log=true;

    int i_itReal;
    double d_errorReal;



    FILE *fp_logfile, *fp_resultsfile, *fp_plotfile;

    FILE *fp=fopen(s_filename, "r");
    if( NULL == fp ){
        MPL_ERROR_MESSAGE("File %s not found!", s_filename);
        return(-1);
    }
    fscanf(fp, "%d %d %d\n", &x_size, &y_size, &z_size);

    MPL_OrientedPotentialField *opf = new MPL_OrientedPotentialField(x_size, y_size, z_size);

    fscanf(fp, "%s %s\n", s_logfile_name, s_logfile_mode);

    fscanf(fp, "%s %s\n", s_resultsfile_name, s_resultsfile_mode);

    fp_logfile=fopen(s_logfile_name, s_logfile_mode);
    if( NULL == fp_logfile ){
        MPL_ERROR_MESSAGE("Error while opening %s file!", s_filename);
        fclose(fp);
        return(-1);
    }

    fp_resultsfile=fopen(s_resultsfile_name, s_resultsfile_mode);
    if( NULL == fp_resultsfile ){
        MPL_ERROR_MESSAGE("Error while opening %s file!", s_resultsfile_name);
        fclose(fp_logfile);
        fclose(fp);
        return(-1);
    }


    fp_plotfile=fopen("cp3dgnuplot.data", "w");
    if( NULL == fp_plotfile ){
        MPL_ERROR_MESSAGE("Error while opening %s file!", "plot.gnu");
        fclose(fp_resultsfile);
        fclose(fp_logfile);
        fclose(fp);
        return(-1);
    }



    while( (!feof(fp)) && (!b_stop) ){
        fscanf(fp, "%c ", &c_action);
        switch (c_action) {
        case 'a': //Add
            fscanf(fp, "%c %lf %lf %lf\n", &c_action, &d_x, &d_y, &d_z);
            switch (c_action) {
            case 'e': //Enemy Robot
                opf->addEnemyRobot(MPL_Position(d_x, d_y, d_z));
                if(b_log){MPL_TEST_LOG_MESSAGE("[ADD] Enemy Robot (%lf, %lf, %lf)", d_x, d_y, d_z);}
                break;
            case 'f': //Fixed Obstacle
                opf->addFixedObstacle(MPL_Position(d_x, d_y, d_z));
                if(b_log){MPL_TEST_LOG_MESSAGE("[ADD] Fixed Obstacle (%lf, %lf, %lf)", d_x, d_y, d_z);}
                break;
            case 'g': //Goal
                opf->addGoal(MPL_Position(d_x, d_y, d_z));
                if(b_log){MPL_TEST_LOG_MESSAGE("[ADD] Goal (%lf, %lf, %lf)", d_x, d_y, d_z);}
                break;
            case 'm': //Movable Obstacle
                opf->addMovableObstacle(MPL_Position(d_x, d_y, d_z));
                if(b_log){MPL_TEST_LOG_MESSAGE("[ADD] Movable Obstacle (%lf, %lf, %lf)", d_x, d_y, d_z);}
                break;
            case 'o': //Own Robot
                opf->setOwnRobot(MPL_Position(d_x, d_y, d_z));
                if(b_log){MPL_TEST_LOG_MESSAGE("[ADD] Own Robot (%lf, %lf, %lf)", d_x, d_y, d_z);}
                break;
            default:
                b_stop=true;
                if(b_log){MPL_TEST_LOG_MESSAGE("[ADD] [ERROR] Unknow char option when adding robot");}
                MPL_ERROR_MESSAGE("Unknow char option when adding robot");
                break;
            }
            break;
        case 'c': //Clear
            fscanf(fp, "%c ", &c_action);
            switch (c_action) {
            case 'e': //Enemy Robot
                opf->clearListOfEnemyRobot();
                if(b_log){MPL_TEST_LOG_MESSAGE("[CLEAR] Enemy Robot");}
                break;
            case 'f': //Fixed Obstacle
                opf->clearListOfFixedObstacle();
                if(b_log){MPL_TEST_LOG_MESSAGE("[CLEAR] Fixed Obstacle");}
                break;
            case 'g': //Goal
                opf->clearListOfGoal();
                if(b_log){MPL_TEST_LOG_MESSAGE("[CLEAR] Goal");}
                break;
            case 'm': //Movable Obstacle
                opf->clearListOfMovableObstacle();
                if(b_log){MPL_TEST_LOG_MESSAGE("[CLEAR] Movable Obstacle");}
                break;
            default:
                b_stop=true;
                if(b_log){MPL_TEST_LOG_MESSAGE("[CLEAR] [ERROR] Unknow char option when cleaning list");}
                MPL_ERROR_MESSAGE("Unknow char option when cleaning list");
                break;
            }
            break;
        case 'd': //Dump (print)
            fscanf(fp, "%c ", &c_action);
            switch (c_action) {
            case 'o': //Occupancy Grid
                if(b_log){MPL_TEST_LOG_MESSAGE("[DUMP] Occupancy Grid");}
                opf->unsafe_dumpOccupancyGrid(fp_logfile);
                break;
            case 'p': //Potential Grid
                if(b_log){MPL_TEST_LOG_MESSAGE("[DUMP] Potential Grid");}
                opf->unsafe_dumpPotentialGrid(fp_logfile);
                break;
//            case 'g':
//                if(b_log){MPL_TEST_LOG_MESSAGE("[DUMP] Potential Grid");}
//                opf->unsafe_toGnuplotPotentialGrid(fp_plotfile);
//                break;
            default:
                b_stop=true;
                if(b_log){MPL_TEST_LOG_MESSAGE("[DUMP] [ERROR] Unknow char option when dumping grid");}
                MPL_ERROR_MESSAGE("Unknow char option when dumping grid");
                break;
            }
            break;
        case 'e': //End
            if(b_log){MPL_TEST_LOG_MESSAGE("[STOP]");}
            b_stop=true;
            break;
        case 's': //Solve
            fscanf(fp, "%c ", &c_action);
            switch (c_action) {
            case 'b': //by max iteration and error
                fscanf(fp, "%c %d %lf\n", &c_action, &n_maxIt, &d_maxError);
                opf_itMode = ( 's' == c_action ? MPL_OPF_SOR : MPL_OPF_GS);
                if(b_log){MPL_TEST_LOG_MESSAGE("[SOLVE]\n[ErrorMax=%.17g]\n[IterationMax=%d]\n[Mode:%s]\n", d_maxError, n_maxIt, ( 's' == c_action ? "MPL_OPF_SOR" : "MPL_OPF_GS") );}
                gettimeofday( &ti, NULL);
                opf->solveIE(opf_itMode, n_maxIt, d_maxError, i_itReal, d_errorReal);
                gettimeofday( &tf, NULL);
                if(b_log){MPL_TEST_LOG_MESSAGE("[SOLVE]\n[ErrorReal=%.17g]\n[IterationReal=%d]\n[Time(ms): %lf]", d_errorReal, i_itReal, (((1000000*(tf.tv_sec - ti.tv_sec) ) + (tf.tv_usec - ti.tv_usec))/1000.0)); }
                MPL_TEST_RESULT_MESSAGE("[SOLVE]\n[ErrorReal=%.17g]\n[IterationReal=%d]\n[Time(ms): %lf]", d_errorReal, i_itReal, (((1000000*(tf.tv_sec - ti.tv_sec) ) + (tf.tv_usec - ti.tv_usec))/1000.0));
                break;
            case 'e': //by error
                fscanf(fp, "%c %lf\n", &c_action, &d_maxError);
                opf_itMode = ( 's' == c_action ? MPL_OPF_SOR : MPL_OPF_GS);
                if(b_log){MPL_TEST_LOG_MESSAGE("[SOLVE]\n[ErrorMax=%.17g]\n[IterationMax=%d]\n[Mode:%s]\n", d_maxError, n_maxIt, ( 's' == c_action ? "MPL_OPF_SOR" : "MPL_OPF_GS") );}
                gettimeofday( &ti, NULL);
                opf->solveE(opf_itMode, d_maxError, i_itReal, d_errorReal);
                gettimeofday( &tf, NULL);
                if(b_log){MPL_TEST_LOG_MESSAGE("[SOLVE]\n[ErrorReal=%.17g]\n[IterationReal=%d]\n[Time(ms): %lf]", d_errorReal, i_itReal, (((1000000*(tf.tv_sec - ti.tv_sec) ) + (tf.tv_usec - ti.tv_usec))/1000.0)); }
                MPL_TEST_RESULT_MESSAGE("[SOLVE]\n[ErrorReal=%.17g]\n[IterationReal=%d]\n[Time(ms): %lf]", d_errorReal, i_itReal, (((1000000*(tf.tv_sec - ti.tv_sec) ) + (tf.tv_usec - ti.tv_usec))/1000.0));
                break;
            case 'i': //by iteration
                fscanf(fp, "%c %d\n", &c_action, &n_maxIt);
                opf_itMode = ( 's' == c_action ? MPL_OPF_SOR : MPL_OPF_GS);
                if(b_log){MPL_TEST_LOG_MESSAGE("[SOLVE]\n[ErrorMax=%.17g]\n[IterationMax=%d]\n[Mode:%s]\n", d_maxError, n_maxIt, ( 's' == c_action ? "MPL_OPF_SOR" : "MPL_OPF_GS") );}
                gettimeofday( &ti, NULL);
                opf->solveI(opf_itMode, n_maxIt, i_itReal, d_errorReal);
                gettimeofday( &tf, NULL);
                if(b_log){MPL_TEST_LOG_MESSAGE("[SOLVE]\n[ErrorReal=%.17g]\n[IterationReal=%d]\n[Time(ms): %lf]", d_errorReal, i_itReal, (((1000000*(tf.tv_sec - ti.tv_sec) ) + (tf.tv_usec - ti.tv_usec))/1000.0)); }
                MPL_TEST_RESULT_MESSAGE("[SOLVE]\n[ErrorReal=%.17g]\n[IterationReal=%d]\n[Time(ms): %lf]", d_errorReal, i_itReal, (((1000000*(tf.tv_sec - ti.tv_sec) ) + (tf.tv_usec - ti.tv_usec))/1000.0));
                break;
            default:
                b_stop=true;
                if(b_log){MPL_TEST_LOG_MESSAGE("[SOLVE] [ERROR] Unknow char option when solving BVP");}
                MPL_ERROR_MESSAGE("Unknow char option when solving BVP");
                break;
            }
            break;
        case 'w': //Omega Sor
            fscanf(fp, "%lf\n", &d_omegaSor);
            if(b_log){MPL_TEST_LOG_MESSAGE("[SET SOR] Omega Sor = %lf", d_omegaSor);}
            opf->setOmega(d_omegaSor);
            break;
        case 'v': //Vector
            fscanf(fp, "%lf %lf %lf\n", &d_x, &d_y, &d_z);
            if(b_log){MPL_TEST_LOG_MESSAGE("[SET VECTOR] Vector = (%lf, %lf, %lf)", d_x, d_y, d_z );}
            opf->setVector(d_x, d_y, d_z);
            break;
        case 'p': //Epsilon
            fscanf(fp, "%lf\n", &d_epsilon);
            if(b_log){MPL_TEST_LOG_MESSAGE("[SET EPSILON] Epsilon = %lf", d_epsilon );}
            opf->setEpsilon(d_epsilon);
            break;
        case 'm': //Verbose
            fscanf(fp, "%c ", &c_action);
            if(b_log){MPL_TEST_LOG_MESSAGE("[SET VERBOSE] Verbose = %s", (c_action=='f' ? "False" : "True") );}
            opf->setVerbose( c_action=='f' ? false : true );
            break;
        case 'l': //Log
            fscanf(fp, "%c ", &c_action);
            b_log = ( c_action=='f' ? false : true ) ;
            if(b_log){
                MPL_INFORMATION_MESSAGE("[LOG STARTED]");
            }
            else{
                MPL_INFORMATION_MESSAGE("[LOG STOPPED]");
            }
            break;
        default:
            b_stop=true;
            if(b_log){MPL_TEST_LOG_MESSAGE("[] [ERROR] Unknow char option switching action");}
            MPL_ERROR_MESSAGE("Unknow char option switching action");
            break;
        }
    }

    //fscanf(fp, "", );

    delete opf;

    fclose(fp_resultsfile);
    fclose(fp_logfile);
    fclose(fp);


    emit signal_done();

    return(0);
}


//int MPL_TestSuite::testHPF(char *s_filename)
//{
//    MPL_HarmonicPotentialField *pf = new MPL_HarmonicPotentialField(10, 10, 3);
//    FILE *fp=fopen("/tmp/testdump2.txt", "w");


//    pf->addGoal(MPL_Position(2,5,1));

//    pf->addEnemyRobot(MPL_Position(2,4,1));
//    pf->addEnemyRobot(MPL_Position(2,6,1));
//    pf->addEnemyRobot(MPL_Position(3,5,1));
//    pf->addEnemyRobot(MPL_Position(3,4,1));
//    pf->addEnemyRobot(MPL_Position(3,6,1));

//    pf->addEnemyRobot(MPL_Position(2,4,2));
//    pf->addEnemyRobot(MPL_Position(2,5,2));
//    pf->addEnemyRobot(MPL_Position(2,6,2));
//    pf->addEnemyRobot(MPL_Position(3,4,2));
//    pf->addEnemyRobot(MPL_Position(3,5,2));
//    pf->addEnemyRobot(MPL_Position(3,6,2));

//    pf->addEnemyRobot(MPL_Position(2,4,0));
//    pf->addEnemyRobot(MPL_Position(2,5,0));
//    pf->addEnemyRobot(MPL_Position(2,6,0));
//    pf->addEnemyRobot(MPL_Position(3,4,0));
//    pf->addEnemyRobot(MPL_Position(3,5,0));
//    pf->addEnemyRobot(MPL_Position(3,6,0));

//    pf->solveIE(MPL_HPF_GS, 500, 0.0000001);

//    pf->unsafe_dumpPotentialGrid(fp);

//    fclose(fp);

//    return(0);
//}
