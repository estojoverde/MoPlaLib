#ifndef MPL_CORE_H
#define MPL_CORE_H

#include <stdio.h>
#include <sys/time.h>

#include <QObject>
#include <QString>
#include <QListWidget>
#include <QMainWindow>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>
#include <QtDebug>


#include "mpl_utils.h"
#include "mpl_position.h"
#include "mpl_baseenvironment.h"
#include "mpl_gridenvironment.h"
#include "mpl_potentialfieldenvironment.h"
#include "mpl_harmonicpotentialfield.h"
#include "mpl_orientedpotentialfield.h"
#include "mpl_opf_parallelsor_mainlaucher.h"



#define MPL_CORE_ERROR_MSG_START emit signal_errorMessage(QString("\n----\n[ERROR] ") +
#define MPL_CORE_ERROR_MSG_END + "\n  File: " +__FILE__ + ":" + S__LINE__ + "\n  Func: " + __PRETTY_FUNCTION__ + "\n----\n" );

#define MPL_CORE_LOG_MSG_START emit signal_logMessage(QString("") +
#define MPL_CORE_LOG_MSG_END );

enum MPL_Core_EnvironmentMode{ MPL_Core_NULL=0, MPL_Core_HPF, MPL_Core_OPF};

enum MPL_Core_SolveMode{ MPL_Core_SV_NULL=0, MPL_Core_SV_HPF_GS, MPL_Core_SV_HPF_SOR, MPL_Core_SV_OPF_GS, MPL_Core_SV_OPF_SOR, MPL_Core_SV_OPF_SOR_Parallel};

class MPL_Core : public QObject
{
    Q_OBJECT
public:
    MPL_Core(QObject *parent=0);

    ~MPL_Core();

    inline MPL_PotentialFieldEnvironment* getPF()
    {
        return( pf__ );
    }

public slots:
    void slot_setCoreMode(MPL_Core_EnvironmentMode coreMode);
    void slot_setLocker(QReadWriteLock  *locker);
    void slot_setGridSize(int n_sizeX, int n_sizeY, int n_sizeZ);
    void slot_setAllPotentials(MPL_PFE_VarType MPL_VT_newValue);
    void slot_setVerbose(bool b_verbose);
    void slot_toggleVerbose();
    void slot_setOmega(double d_omega);

    void slot_setEpsilon_OPF(double d_epsilon);//only OPF
    void slot_setVector_OPF(double d_vx, double d_vy, double d_vz);//only OPF

    void slot_setSolveMode(MPL_Core_SolveMode solveMode);

    void slot_solveI(int i_maxIt);
    void slot_solveE(double d_maxError);
    void slot_solveIE(int i_maxIt, double d_maxError);

    void slot_setOwnRobot(MPL_Position mpl_p);
    void slot_setOwnRobot(double d_x, double d_y, double d_z);

    void slot_addMovableObstacle(MPL_Position mpl_p);
    void slot_addMovableObstacle(double d_x, double d_y, double d_z);
    void slot_clearListOfMovableObstacle(void);

    void slot_addFixedObstacle(MPL_Position mpl_p);
    void slot_addFixedObstacle(double d_x, double d_y, double d_z);
    void slot_clearListOfFixedObstacle(void);

    void slot_addEnemyRobot(MPL_Position mpl_p);
    void slot_addEnemyRobot(double d_x, double d_y, double d_z);
    void slot_clearListOfEnemyRobot(void);

    void slot_addGoal(MPL_Position mpl_p);
    void slot_addGoal(double d_x, double d_y, double d_z);
    void slot_clearListOfGoal(void);

    void slot_getPath(MPL_Position pos_start, MPL_Position *pos_path, int n_maxSteps);
    void slot_getPathFromOwnRobot(MPL_Position *pos_path, int n_maxSteps);

    void slot_exit(void);

signals:
    void signal_logMessage( QString s );
    void signal_errorMessage( QString s );

    void signal_newGridSize(int n_sizeX, int n_sizeY, int n_sizeZ);
    void signal_newCoreMode(MPL_Core_EnvironmentMode coreMode);
    void signal_newOmega(double d_omega);
    void signal_newEpsilon_OPF(double d_omega);
    void signal_newVector_OPF(double d_vx, double d_vy, double d_vz);

    void signal_newSolveMode_HPF_SOR(void);
    void signal_newSolveMode_HPF_GS(void);
    void signal_newSolveMode_OPF_SOR(void);
    void signal_newSolveMode_OPF_GS(void);

    void signal_newSolveMode_OPF_SOR_Parallel(void);
    void signal_SetOPF(MPL_OrientedPotentialField *opf, QReadWriteLock *locker);


    void signal_pfSolved(int n_finalIt, double d_finalError );



protected:

    /*
     * solve
     * * sor /gs
     * * it / error / both
     *
     *
     *
     * */

    void solveI(int i_maxIt);
    void solveE(double d_maxError);
    void solveIE(int i_maxIt, double d_maxError);


    /*
     * set grid size[done]
     * set all potentials[done]
     *
     * set verbose mode[done]
     *
     * set omega sor[done]
     *
     * set vector opf[done]
     * set epsilon opf[done]
     *
     *
     */

    void setGridSize(int n_sizeX, int n_sizeY, int n_sizeZ);

    void setAllPotentials(MPL_PFE_VarType MPL_VT_newValue);

    void setCoreMode(MPL_Core_EnvironmentMode coreMode);

    void setSolveMode(MPL_Core_SolveMode solveMode);

    void setVerbose(bool b_verbose);

    void toggleVerbose();

    void setOmega(double d_omega);

    void setEpsilon_OPF(double d_epsilon);

    void setVector_OPF(double d_vx, double d_vy, double d_vz);

    void getPath(MPL_Position pos_start, MPL_Position *pos_path, int n_maxSteps);

    void getPathFromOwnRobot(MPL_Position *pos_path, int n_maxSteps);


    /* movable obstacle[done]
     * fixed obstacle[done]
     * enemy robot[done]
     * goal[done]
     *
     * Add[done]
     * clear[done]
     * TODO "delete all nearest to"
     * */
    void setOwnRobot(MPL_Position &mpl_p);//aux

    void addMovableObstacle(MPL_Position &mpl_p);//aux
    void clearListOfMovableObstacle(void);//aux

    void addFixedObstacle(MPL_Position &mpl_p);//aux
    void clearListOfFixedObstacle(void);//aux

    void addEnemyRobot(MPL_Position &mpl_p);//aux
    void clearListOfEnemyRobot(void);//aux

    void addGoal(MPL_Position &mpl_p);//aux
    void clearListOfGoal(void);//aux









private:
    MPL_HarmonicPotentialField *hpf__;
    MPL_OrientedPotentialField *opf__;
    MPL_PotentialFieldEnvironment *pf__;

    MPL_Core_EnvironmentMode coreMode__;
    MPL_Core_SolveMode solveMode__;

    bool b_log__;

    bool b_aboutToExit__;

    QReadWriteLock *locker__;


    MPL_OPF_ParallelSor_MainLaucher PSOR_opf_solver;

};

Q_DECLARE_METATYPE(MPL_Core_EnvironmentMode)

Q_DECLARE_METATYPE(MPL_Core_SolveMode)


#endif // MPL_CORE_H
