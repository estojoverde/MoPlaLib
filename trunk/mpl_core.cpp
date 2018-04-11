#include "mpl_core.h"

MPL_Core::MPL_Core(QObject *parent): QObject(parent)
{
    hpf__ = NULL;
    opf__ = NULL;
    pf__ = NULL;
    coreMode__ = MPL_Core_NULL;
    solveMode__ = MPL_Core_SV_NULL;
    b_log__=false;
    b_aboutToExit__=false;

    locker__ = NULL;



    connect( this, SIGNAL(signal_SetOPF(MPL_OrientedPotentialField*,QReadWriteLock*)), &PSOR_opf_solver, SLOT(slot_SetOPF(MPL_OrientedPotentialField*,QReadWriteLock*)) );
    connect( &PSOR_opf_solver, SIGNAL(signal_LogMessage(QString)), this,  SIGNAL(signal_logMessage(QString)) );
    connect( &PSOR_opf_solver, SIGNAL(signal_ErrorMessage(QString)), this,  SIGNAL(signal_errorMessage(QString)) ); 

}

MPL_Core::~MPL_Core()
{
    if( NULL != hpf__)
        delete hpf__;

    if( NULL != opf__)
        delete opf__;

}

void MPL_Core::slot_setLocker(QReadWriteLock  *locker)
{
    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_setLocker]" MPL_CORE_LOG_MSG_END
    }

    locker__ = locker;
}



void MPL_Core::setCoreMode(MPL_Core_EnvironmentMode coreMode)
{


    switch(coreMode){
    case MPL_Core_HPF:
        coreMode__=coreMode;
        if(NULL == hpf__){
            hpf__ = new MPL_HarmonicPotentialField();
        }
        pf__ = (MPL_PotentialFieldEnvironment *)hpf__;
        emit signal_newCoreMode(coreMode);

        break;
    case MPL_Core_OPF:
        coreMode__=coreMode;
        if(NULL == opf__){
            opf__ = new MPL_OrientedPotentialField();
        }
        pf__ = (MPL_PotentialFieldEnvironment *)opf__;
        emit signal_newCoreMode(coreMode);
        break;
    default:
        coreMode__=MPL_Core_NULL;
        MPL_CORE_ERROR_MSG_START "[MPL_Core::setCoreMode] No valid Core Mode being set | " MPL_CORE_ERROR_MSG_END
        break;
    }
    connect( pf__, SIGNAL(signal_errorMessage(QString)), this,  SIGNAL(signal_errorMessage(QString)) );
    connect( pf__, SIGNAL(signal_logMessage(QString)), this,  SIGNAL(signal_logMessage(QString)) );

    return;
}


void  MPL_Core::setSolveMode(MPL_Core_SolveMode solveMode)
{
    switch (coreMode__)
    {
    case MPL_Core_HPF:
        switch(solveMode)
        {
        case MPL_Core_SV_HPF_SOR:
            solveMode__ = solveMode;
            emit signal_newSolveMode_HPF_SOR();
            if(b_log__){
                MPL_CORE_LOG_MSG_START "    setSolveMode: HPF - SOR" MPL_CORE_LOG_MSG_END
            }
            break;
        case MPL_Core_SV_HPF_GS:
            solveMode__ = solveMode;
            emit signal_newSolveMode_HPF_GS();
            if(b_log__){
                MPL_CORE_LOG_MSG_START "    setSolveMode: HPF - GS" MPL_CORE_LOG_MSG_END
            }
            break;
        default:
            solveMode__ = MPL_Core_SV_NULL;
            MPL_CORE_ERROR_MSG_START "[MPL_Core::setSolveMode] HPF | No valid Solve Mode being set | " MPL_CORE_ERROR_MSG_END
            break;
        }
        break;
    case MPL_Core_OPF:
        switch(solveMode)
        {
        case MPL_Core_SV_OPF_SOR:
            solveMode__ = solveMode;
            emit signal_newSolveMode_OPF_SOR();
            if(b_log__){
                MPL_CORE_LOG_MSG_START "    setSolveMode: OPF - SOR" MPL_CORE_LOG_MSG_END
            }
            break;
        case MPL_Core_SV_OPF_GS:
            solveMode__ = solveMode;
            emit signal_newSolveMode_OPF_GS();
            if(b_log__){
                MPL_CORE_LOG_MSG_START "    setSolveMode: OPF - GS" MPL_CORE_LOG_MSG_END
            }
            break;
        case MPL_Core_SV_OPF_SOR_Parallel:
            solveMode__ = solveMode;
            emit signal_SetOPF(opf__, locker__ );
            //@Bug?
            emit signal_newSolveMode_OPF_SOR_Parallel();
            if(b_log__){
                MPL_CORE_LOG_MSG_START "    setSolveMode: OPF - GS - Parallel" MPL_CORE_LOG_MSG_END
            }
            break;
        default:
            solveMode__ = MPL_Core_SV_NULL;
            MPL_CORE_ERROR_MSG_START QString("[MPL_Core::setSolveMode] OPF | No valid Solve Mode being set | ") MPL_CORE_ERROR_MSG_END
            break;
        }
        break;
    default:
        MPL_CORE_ERROR_MSG_START QString("[MPL_Core::setSolveMode] No valid core state | ")  MPL_CORE_ERROR_MSG_END
        break;
    }
    return;
}




void MPL_Core::setVerbose(bool b_verbose)
{
    QString qs_aux = "setVerbose ["+ (b_verbose?tr("true"):tr("false")) + "]";
    emit signal_logMessage( qs_aux );

    switch (coreMode__)
    {
    case MPL_Core_HPF:
        if(NULL == hpf__){
            MPL_CORE_ERROR_MSG_START "No valid Harmonic Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        hpf__->setVerbose(b_verbose);
        b_log__ = b_verbose;
        break;
    case MPL_Core_OPF:
        if(NULL == opf__){
            MPL_CORE_ERROR_MSG_START "No valid Oriented Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        opf__->setVerbose(b_verbose);
        b_log__ = b_verbose;
        break;
    default:
        MPL_CORE_ERROR_MSG_START QString("No valid core state | ") + qs_aux MPL_CORE_ERROR_MSG_END
        break;
    }
    return;
}



void MPL_Core::toggleVerbose()
{
    QString qs_aux = "MPL_Core::toggleVerbose";
    //emit signal_logMessage( qs_aux );

    switch (coreMode__)
    {
    case MPL_Core_HPF:
        if(NULL == hpf__){
            MPL_CORE_ERROR_MSG_START "No valid Harmonic Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        hpf__->toggleVerbose();
        b_log__ = !b_log__;
        break;
    case MPL_Core_OPF:
        if(NULL == opf__){
            MPL_CORE_ERROR_MSG_START "No valid Oriented Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        opf__->toggleVerbose();
        b_log__ = !b_log__;
        break;
    default:
        MPL_CORE_ERROR_MSG_START QString("No valid core state | ") + qs_aux MPL_CORE_ERROR_MSG_END
        break;
    }


    return;
}



/*_____       _
 / ____|     | |
| (___   ___ | |_   _____ _ __ ___
 \___ \ / _ \| \ \ / / _ \ '__/ __|
 ____) | (_) | |\ V /  __/ |  \__ \
|_____/ \___/|_| \_/ \___|_|  |___/*/







void MPL_Core::solveI(int i_maxIt)
{
    int n_finalIt;
    double d_finalError;

    QString qs_aux = "   solveI ["+ QString::number(i_maxIt) + "]";
    if(b_log__){
        emit signal_logMessage( qs_aux );
    }

    PSOR_opf_solver.slot_SetOPF(opf__, locker__);

    qWarning("-------------------------------------------START");

    this->thread()->msleep(1);

    PSOR_opf_solver.slot_Start();

    this->thread()->msleep(4);

    //Too slow to start, so it give resources, but no one consumes it!

    PSOR_opf_solver.slot_SolveByIteration(i_maxIt);

    this->thread()->msleep(1);

    PSOR_opf_solver.slot_Stop();

    qWarning("-------------------------------------------END");

    /*
    switch (coreMode__)
    {
    case MPL_Core_HPF:
        switch(solveMode__)
        {
        case MPL_Core_SV_HPF_SOR:
            hpf__->solveI(MPL_HPF_SOR, i_maxIt, n_finalIt, d_finalError );
            emit signal_pfSolved(n_finalIt, d_finalError);
            if(b_log__){
                MPL_CORE_LOG_MSG_START QString("      -It[%1]\n      -Error[%2]").arg(n_finalIt).arg(d_finalError) MPL_CORE_LOG_MSG_END
            }
            break;
        case MPL_Core_SV_HPF_GS:
            hpf__->solveI(MPL_HPF_GS, i_maxIt, n_finalIt, d_finalError );
            emit signal_pfSolved(n_finalIt, d_finalError);
            if(b_log__){
                MPL_CORE_LOG_MSG_START QString("      -It[%1]\n      -Error[%2]").arg(n_finalIt).arg(d_finalError) MPL_CORE_LOG_MSG_END
            }
            break;
        default:
            MPL_CORE_ERROR_MSG_START QString("No valid Solve Mode  | ") + qs_aux MPL_CORE_ERROR_MSG_END
            break;
        }
        break;
    case MPL_Core_OPF:
        switch(solveMode__)
        {
        case MPL_Core_SV_OPF_SOR:
            opf__->solveI(MPL_OPF_SOR, i_maxIt, n_finalIt, d_finalError );
            emit signal_pfSolved(n_finalIt, d_finalError);
            if(b_log__){
                MPL_CORE_LOG_MSG_START QString("      -It[%1]\n      -Error[%2]").arg(n_finalIt).arg(d_finalError) MPL_CORE_LOG_MSG_END
            }
            break;
        case MPL_Core_SV_OPF_GS:
            opf__->solveI(MPL_OPF_GS, i_maxIt, n_finalIt, d_finalError );
            emit signal_pfSolved(n_finalIt, d_finalError);
            if(b_log__){
                MPL_CORE_LOG_MSG_START QString("      -It[%1]\n      -Error[%2]").arg(n_finalIt).arg(d_finalError) MPL_CORE_LOG_MSG_END
            }
            break;
        default:
            MPL_CORE_ERROR_MSG_START QString("No valid Solve Mode | ") + qs_aux MPL_CORE_ERROR_MSG_END
            break;
        }
        break;
    default:
        MPL_CORE_ERROR_MSG_START QString("No valid core state | ")  MPL_CORE_ERROR_MSG_END
        break;
    }*/




    return;

}

void MPL_Core::solveE(double d_maxError)
{
    int n_finalIt;
    double d_finalError;

    QString qs_aux = "    solveE ["+ QString::number(d_maxError, 'f', 10) + "]";
    if(b_log__){
        emit signal_logMessage( qs_aux );
    }

    switch (coreMode__)
    {
    case MPL_Core_HPF:
        switch(solveMode__)
        {
        case MPL_Core_SV_HPF_SOR:
            hpf__->solveE(MPL_HPF_SOR, d_maxError, n_finalIt, d_finalError );
            emit signal_pfSolved(n_finalIt, d_finalError);
            if(b_log__){
                MPL_CORE_LOG_MSG_START QString("      -It[%1]\n      -Error[%2]").arg(n_finalIt).arg(d_finalError) MPL_CORE_LOG_MSG_END
            }
            break;
        case MPL_Core_SV_HPF_GS:
            hpf__->solveE(MPL_HPF_GS, d_maxError, n_finalIt, d_finalError );
            emit signal_pfSolved(n_finalIt, d_finalError);
            if(b_log__){
                MPL_CORE_LOG_MSG_START QString("      -It[%1]\n      -Error[%2]").arg(n_finalIt).arg(d_finalError) MPL_CORE_LOG_MSG_END
            }
            break;
        default:
            MPL_CORE_ERROR_MSG_START QString("No valid Solve Mode | ") + qs_aux MPL_CORE_ERROR_MSG_END
            break;
        }
        break;
    case MPL_Core_OPF:
        switch(solveMode__)
        {
        case MPL_Core_SV_OPF_SOR:
            opf__->solveE(MPL_OPF_SOR, d_maxError, n_finalIt, d_finalError );
            emit signal_pfSolved(n_finalIt, d_finalError);
            if(b_log__){
                MPL_CORE_LOG_MSG_START QString("      -It[%1]\n      -Error[%2]").arg(n_finalIt).arg(d_finalError) MPL_CORE_LOG_MSG_END
            }
            break;
        case MPL_Core_SV_OPF_GS:
            opf__->solveE(MPL_OPF_GS, d_maxError, n_finalIt, d_finalError );
            emit signal_pfSolved(n_finalIt, d_finalError);
            if(b_log__){
                MPL_CORE_LOG_MSG_START QString("      -It[%1]\n      -Error[%2]").arg(n_finalIt).arg(d_finalError) MPL_CORE_LOG_MSG_END
            }
            break;
        default:
            MPL_CORE_ERROR_MSG_START QString("No valid Solve Mode  | ") + qs_aux MPL_CORE_ERROR_MSG_END
            break;
        }
        break;
    default:
        MPL_CORE_ERROR_MSG_START QString("No valid core state | ")  MPL_CORE_ERROR_MSG_END
        break;
    }
    return;
}

void MPL_Core::solveIE(int i_maxIt, double d_maxError)
{
    int n_finalIt;
    double d_finalError;

    QString qs_aux = "    solveIE ["+ QString::number(i_maxIt) +","+ QString::number(d_maxError, 'f', 10) + "]";
    if(b_log__){
        emit signal_logMessage( qs_aux );
    }


    switch (coreMode__)
    {
    case MPL_Core_HPF:
        switch(solveMode__)
        {
        case MPL_Core_SV_HPF_SOR:
            hpf__->solveIE(MPL_HPF_SOR, i_maxIt, d_maxError, n_finalIt, d_finalError );
            emit signal_pfSolved(n_finalIt, d_finalError);
            if(b_log__){
                MPL_CORE_LOG_MSG_START QString("      -It[%1]\n      -Error[%2]").arg(n_finalIt).arg(d_finalError) MPL_CORE_LOG_MSG_END
            }
            break;
        case MPL_Core_SV_HPF_GS:
            hpf__->solveIE(MPL_HPF_GS, i_maxIt, d_maxError, n_finalIt, d_finalError );
            emit signal_pfSolved(n_finalIt, d_finalError);
            if(b_log__){
                MPL_CORE_LOG_MSG_START QString("      -It[%1]\n      -Error[%2]").arg(n_finalIt).arg(d_finalError) MPL_CORE_LOG_MSG_END
            }
            break;
        default:
            MPL_CORE_ERROR_MSG_START QString("No valid Solve Mode | ") + qs_aux MPL_CORE_ERROR_MSG_END
            break;
        }
        break;
    case MPL_Core_OPF:
        switch(solveMode__)
        {
        case MPL_Core_SV_OPF_SOR:
            opf__->solveIE(MPL_OPF_SOR, i_maxIt, d_maxError, n_finalIt, d_finalError );
            emit signal_pfSolved(n_finalIt, d_finalError);
            if(b_log__){
                MPL_CORE_LOG_MSG_START QString("      -It[%1]\n      -Error[%2]").arg(n_finalIt).arg(d_finalError) MPL_CORE_LOG_MSG_END
            }
            break;
        case MPL_Core_SV_OPF_GS:
            opf__->solveIE(MPL_OPF_GS, i_maxIt, d_maxError, n_finalIt, d_finalError );
            emit signal_pfSolved(n_finalIt, d_finalError);
            if(b_log__){
                MPL_CORE_LOG_MSG_START QString("      -It[%1]\n      -Error[%2]").arg(n_finalIt).arg(d_finalError) MPL_CORE_LOG_MSG_END
            }
            break;
        default:
            solveMode__ = MPL_Core_SV_NULL;
            MPL_CORE_ERROR_MSG_START QString("No valid Solve Mode  | ") + qs_aux MPL_CORE_ERROR_MSG_END
            break;
        }
        break;
    default:
        MPL_CORE_ERROR_MSG_START QString("No valid core state | ")  MPL_CORE_ERROR_MSG_END
        break;
    }
    return;
}






/*
  _____ _____  _____ _____     ____  _____
 / ____|  __ \|_   _|  __ \   / __ \|  __ \
| |  __| |__) | | | | |  | | | |  | | |__) |__
| | |_ |  _  /  | | | |  | | | |  | |  ___/ __|
| |__| | | \ \ _| |_| |__| | | |__| | |   \__ \
 \_____|_|  \_\_____|_____/   \____/|_|   |___/
*/


void MPL_Core::setGridSize(int n_sizeX, int n_sizeY, int n_sizeZ){
    QString qs_aux = "    setGridSize ["+ QString::number(n_sizeX, 'f', 0) + ", " + QString::number(n_sizeY, 'f', 0) + ", " + QString::number(n_sizeZ, 'f', 0) + "]";
    if(b_log__){
        emit signal_logMessage( qs_aux );
    }

    switch (coreMode__)
    {
    case MPL_Core_HPF:
        if(NULL == hpf__){
            MPL_CORE_ERROR_MSG_START "No valid Harmonic Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_PFE_ReturnCode_InvalidGridSize == hpf__->setGridSize(n_sizeX, n_sizeY, n_sizeZ) ){
            MPL_CORE_ERROR_MSG_START QString("HPF: Invalid Grid Size | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        else{
            clearListOfEnemyRobot();
            clearListOfFixedObstacle();
            clearListOfGoal();
            clearListOfMovableObstacle();
            MPL_Position mpl_p(0,0,0);
            setOwnRobot( mpl_p );
            //emit signal_newGridSize(n_sizeX, n_sizeY, n_sizeZ);
        }
        break;
    case MPL_Core_OPF:
        if(NULL == opf__){
            MPL_CORE_ERROR_MSG_START "No valid Oriented Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_GE_ReturnCode_InvalidPosition == opf__->setGridSize(n_sizeX, n_sizeY, n_sizeZ) ){
            MPL_CORE_ERROR_MSG_START QString("OPF: Invalid Grid Size | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        else{
            //emit signal_newGridSize(n_sizeX, n_sizeY, n_sizeZ);
            clearListOfEnemyRobot();
            clearListOfFixedObstacle();
            clearListOfGoal();
            clearListOfMovableObstacle();
            MPL_Position mpl_p(0,0,0);
            setOwnRobot( mpl_p );
        }
        break;
    default:
        MPL_CORE_ERROR_MSG_START QString("No valid core state | ") + qs_aux MPL_CORE_ERROR_MSG_END
        break;
    }
}




void MPL_Core::setAllPotentials(MPL_PFE_VarType MPL_VT_newValue){

#ifndef MPL_PFE_DOUBLE
#error Need to set adequate number string conversion
#endif

    QString qs_aux = "    setAllPotentials ["+ QString::number(MPL_VT_newValue, 'f', 10) + "]";
    if(b_log__){
        emit signal_logMessage( qs_aux );
    }

    switch (coreMode__)
    {
    case MPL_Core_HPF:
        if(NULL == hpf__){
            MPL_CORE_ERROR_MSG_START "No valid Harmonic Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        hpf__->setAllPotentials(MPL_VT_newValue);
        break;
    case MPL_Core_OPF:
        if(NULL == opf__){
            MPL_CORE_ERROR_MSG_START "No valid Oriented Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        /*
        if( MPL_GE_ReturnCode_InvalidPosition == opf__->setGridSize(n_sizeX, n_sizeY, n_sizeZ) ){
            MPL_CORE_ERROR_MSG_START QString("OPF: Invalid Grid Size | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        No need to check for errors currently, since setAllPotentials(); dont do that!
        */
        opf__->setAllPotentials(MPL_VT_newValue);
        break;
    default:
        MPL_CORE_ERROR_MSG_START QString("No valid core state | ") + qs_aux MPL_CORE_ERROR_MSG_END
        break;
    }
}


void MPL_Core::setOmega(double d_omega)
{
    QString qs_aux = "    setOmega ["+ QString::number(d_omega, 'f', 10) + "]";
    if(b_log__){
        emit signal_logMessage( qs_aux );
    }

    switch (coreMode__)
    {
    case MPL_Core_HPF:
        if(NULL == hpf__){
            MPL_CORE_ERROR_MSG_START "No valid Harmonic Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_HPF_ReturnCode_InvalidOmega == hpf__->setOmega(d_omega) ){
            MPL_CORE_ERROR_MSG_START QString("HPF: No valid omega | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        break;
    case MPL_Core_OPF:
        if(NULL == opf__){
            MPL_CORE_ERROR_MSG_START "No valid Oriented Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_OPF_ReturnCode_InvalidOmega == opf__->setOmega(d_omega) ){
            MPL_CORE_ERROR_MSG_START QString("OPF: No valid omega | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        else{
            emit signal_newOmega(d_omega);
        }
        break;
    default:
        MPL_CORE_ERROR_MSG_START QString("No valid core state | ") + qs_aux MPL_CORE_ERROR_MSG_END
        break;
    }
    return;
}



void MPL_Core::setEpsilon_OPF(double d_epsilon)
{
    QString qs_aux = "    setEpsilon_OPF ["+ QString::number(d_epsilon, 'f', 10) + "]";
    if(b_log__){
        emit signal_logMessage( qs_aux );
    }

    switch (coreMode__)
    {
    case MPL_Core_HPF:
        MPL_CORE_ERROR_MSG_START "Core mode in HPF mode. Epsilon are just OPF related. | " + qs_aux MPL_CORE_ERROR_MSG_END
        break;
    case MPL_Core_OPF:
        if(NULL == opf__){
            MPL_CORE_ERROR_MSG_START "No valid Oriented Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_OPF_ReturnCode_InvalidOmega == opf__->setEpsilon(d_epsilon) ){
            MPL_CORE_ERROR_MSG_START QString("OPF: No valid epsilon | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        else{
            emit signal_newEpsilon_OPF(d_epsilon);
        }
        break;
    default:
        MPL_CORE_ERROR_MSG_START QString("No valid core state | ") + qs_aux MPL_CORE_ERROR_MSG_END
        break;
    }
    return;
}

void MPL_Core::setVector_OPF(double d_vx, double d_vy, double d_vz)
{
    QString qs_aux = "    setVector_OPF ("+ QString::number(d_vx, 'f', 10) + "," +  QString::number(d_vy, 'f', 10) + "," +  QString::number(d_vz, 'f', 10) +  ")";
    if(b_log__){
        emit signal_logMessage( qs_aux );
    }

    switch (coreMode__)
    {
    case MPL_Core_HPF:
        MPL_CORE_ERROR_MSG_START "Core mode in HPF mode. Epsilon are just OPF related. | " + qs_aux MPL_CORE_ERROR_MSG_END
        break;
    case MPL_Core_OPF:
        if(NULL == opf__){
            MPL_CORE_ERROR_MSG_START "No valid Oriented Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        opf__->setVector(d_vx, d_vy, d_vz); //no errors are checked in OPF, so... no check here also.
        emit signal_newVector_OPF(d_vx, d_vy, d_vz);
        break;
    default:
        MPL_CORE_ERROR_MSG_START QString("No valid core state | ") + qs_aux MPL_CORE_ERROR_MSG_END
        break;
    }
    return;
}



//void iterator_GS(void);
//void iterator_SOR(void);

//void solveI(  MPL_Core_SolveMode solveMode, int i_maxItm);
//void solveE(  MPL_Core_SolveMode solveMode, double d_maxError);
//void solveIE( MPL_Core_SolveMode solveMode, int i_maxIt, double d_maxError);


/*
  _____ _      ____ _______ _____
 / ____| |    / __ \__   __/ ____|
| (___ | |   | |  | | | | | (___
 \___ \| |   | |  | | | |  \___ \
 ____) | |___| |__| | | |  ____) |
|_____/|______\____/  |_| |_____/
*/


void MPL_Core::slot_setGridSize(int n_sizeX, int n_sizeY, int n_sizeZ)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_setGridSize] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_setGridSize]" MPL_CORE_LOG_MSG_END
    }


    setGridSize(n_sizeX, n_sizeY, n_sizeZ);
    return;
}

void MPL_Core::slot_setAllPotentials(MPL_PFE_VarType MPL_VT_newValue)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_setAllPotentials] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_setAllPotentials]" MPL_CORE_LOG_MSG_END
    }


    setAllPotentials(MPL_VT_newValue);
    return;
}

void MPL_Core::slot_setVerbose(bool b_verbose)
{
    setVerbose(b_verbose);
    return;
}

void MPL_Core::slot_toggleVerbose()
{
    toggleVerbose();
    return;
}


void MPL_Core::slot_setCoreMode(MPL_Core_EnvironmentMode coreMode)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_setCoreMode] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_setCoreMode]" MPL_CORE_LOG_MSG_END
    }

    setCoreMode(coreMode);
    return;
}


void MPL_Core::slot_setOmega(double d_omega)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_setOmega] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_setOmega]" MPL_CORE_LOG_MSG_END
    }

    setOmega(d_omega);
    return;
}


void MPL_Core::slot_setSolveMode(MPL_Core_SolveMode solveMode)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_setSolveMode] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_setSolveMode]" MPL_CORE_LOG_MSG_END
    }

    setSolveMode(solveMode);
    return;
}

void MPL_Core::slot_solveI(int i_maxIt)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_solveI] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_solveI]" MPL_CORE_LOG_MSG_END
    }

    solveI(i_maxIt);
    return;
}

void MPL_Core::slot_solveE(double d_maxError)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_solveE] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_solveE]" MPL_CORE_LOG_MSG_END
    }

    solveE(d_maxError);
    return;
}

void MPL_Core::slot_solveIE(int i_maxIt, double d_maxError)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_solveIE] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_solveIE]" MPL_CORE_LOG_MSG_END
    }


    solveIE(i_maxIt, d_maxError);
    return;
}



void MPL_Core::slot_setEpsilon_OPF(double d_epsilon)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_setEpsilon_OPF] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_setEpsilon_OPF]" MPL_CORE_LOG_MSG_END
    }

    setEpsilon_OPF(d_epsilon);
    return;
}

void MPL_Core::slot_setVector_OPF(double d_vx, double d_vy, double d_vz)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_setVector_OPF] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_setVector_OPF]" MPL_CORE_LOG_MSG_END
    }

    setVector_OPF(d_vx, d_vy, d_vz);
    return;
}







void MPL_Core::slot_setOwnRobot(MPL_Position mpl_p)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_setOwnRobot] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_setOwnRobot]" MPL_CORE_LOG_MSG_END
    }

    setOwnRobot(mpl_p);
}

void MPL_Core::slot_setOwnRobot(double d_x, double d_y, double d_z)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_setOwnRobot] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_setOwnRobot]" MPL_CORE_LOG_MSG_END
    }

    MPL_Position mpl_p(d_x, d_y, d_z);
    setOwnRobot( mpl_p );
}



void MPL_Core::slot_addMovableObstacle(MPL_Position mpl_p)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_addMovableObstacle] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_addMovableObstacle]" MPL_CORE_LOG_MSG_END
    }

    addMovableObstacle(mpl_p);
}

void MPL_Core::slot_addMovableObstacle(double d_x, double d_y, double d_z)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_addMovableObstacle] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_addMovableObstacle]" MPL_CORE_LOG_MSG_END
    }

    MPL_Position mpl_p(d_x, d_y, d_z);
    addMovableObstacle(mpl_p);
}

void MPL_Core::slot_clearListOfMovableObstacle(void)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_clearListOfMovableObstacle] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_clearListOfMovableObstacle]" MPL_CORE_LOG_MSG_END
    }


    clearListOfMovableObstacle();
}


void MPL_Core::slot_addFixedObstacle(MPL_Position mpl_p)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_addFixedObstacle] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_addFixedObstacle]" MPL_CORE_LOG_MSG_END
    }


    addFixedObstacle(mpl_p);
}

void MPL_Core::slot_addFixedObstacle(double d_x, double d_y, double d_z)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_addFixedObstacle] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_addFixedObstacle]" MPL_CORE_LOG_MSG_END
    }

    MPL_Position mpl_p(d_x, d_y, d_z);
    addFixedObstacle(mpl_p);
}

void MPL_Core::slot_clearListOfFixedObstacle(void)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_clearListOfFixedObstacle] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_clearListOfFixedObstacle]" MPL_CORE_LOG_MSG_END
    }

    clearListOfFixedObstacle();
}




void MPL_Core::slot_addEnemyRobot(MPL_Position mpl_p)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_addEnemyRobot] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_addEnemyRobot]" MPL_CORE_LOG_MSG_END
    }

    addEnemyRobot(mpl_p);
}

void MPL_Core::slot_addEnemyRobot(double d_x, double d_y, double d_z)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_addEnemyRobot] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_addEnemyRobot]" MPL_CORE_LOG_MSG_END
    }


    MPL_Position mpl_p(d_x, d_y, d_z);
    addEnemyRobot(mpl_p);
}

void MPL_Core::slot_clearListOfEnemyRobot(void)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_clearListOfEnemyRobot] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_clearListOfEnemyRobot]" MPL_CORE_LOG_MSG_END
    }

    clearListOfEnemyRobot();
}


void MPL_Core::slot_addGoal(MPL_Position mpl_p)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_addGoal] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_addGoal]" MPL_CORE_LOG_MSG_END
    }

    addGoal(mpl_p);
}

void MPL_Core::slot_addGoal(double d_x, double d_y, double d_z)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_addGoal] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_addGoal]" MPL_CORE_LOG_MSG_END
    }

    MPL_Position mpl_p(d_x, d_y, d_z);
    addGoal(mpl_p);
}

void MPL_Core::slot_clearListOfGoal(void)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_clearListOfGoal] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_clearListOfGoal]" MPL_CORE_LOG_MSG_END
    }

    clearListOfGoal();
}


void MPL_Core::slot_exit(void)
{
    b_aboutToExit__=true;

    if( NULL != hpf__)
    {
        delete hpf__;
    }

    if( NULL != opf__)
    {
        delete opf__;
    }

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_exit]" MPL_CORE_LOG_MSG_END
    }

    MPL_CORE_LOG_MSG_START "\n\n----------EOF" MPL_CORE_LOG_MSG_END

    //quit?
}




/*
 _      _____  _____ _______     ____  _____
| |    |_   _|/ ____|__   __|   / __ \|  __ \
| |      | | | (___    | |___  | |  | | |__) |__
| |      | |  \___ \   | / __| | |  | |  ___/ __|
| |____ _| |_ ____) |  | \__ \ | |__| | |   \__ \
|______|_____|_____/   |_|___/  \____/|_|   |___/
*/

void MPL_Core::setOwnRobot(MPL_Position &mpl_p)
{

    QString qs_aux = "    setOwnRobot ("+ QString::number(mpl_p.getX(), 'f', 10) + ", " + QString::number(mpl_p.getY(), 'f', 10) + ", " + QString::number(mpl_p.getZ(), 'f', 10) + ")";
    if(b_log__){
        emit signal_logMessage( qs_aux );
    }

    switch (coreMode__)
    {
    case MPL_Core_HPF:
        if(NULL == hpf__){
            MPL_CORE_ERROR_MSG_START QString("No valid Harmonic Potential Field Environment") MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_GE_ReturnCode_InvalidPosition == hpf__->setOwnRobot(mpl_p) ){
            MPL_CORE_ERROR_MSG_START QString("HPF: No valid position | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        break;
    case MPL_Core_OPF:
        if(NULL == opf__){
            MPL_CORE_ERROR_MSG_START "No valid Oriented Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_GE_ReturnCode_InvalidPosition == opf__->setOwnRobot(mpl_p) ){
            MPL_CORE_ERROR_MSG_START QString("OPF: No valid position | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        break;
    default:
        MPL_CORE_ERROR_MSG_START QString("No valid core state | ") + qs_aux MPL_CORE_ERROR_MSG_END
        break;
    }

    return;
}


void MPL_Core::addMovableObstacle(MPL_Position &mpl_p)
{


    QString qs_aux = "    addMovableObstacle ("+ QString::number(mpl_p.getX(), 'f', 10) + ", " + QString::number(mpl_p.getY(), 'f', 10) + ", " + QString::number(mpl_p.getZ(), 'f', 10) + ")" ;
    if(b_log__){
       emit signal_logMessage( qs_aux );
    }

    switch (coreMode__)
    {
    case MPL_Core_HPF:
        if(NULL == hpf__){
            MPL_CORE_ERROR_MSG_START "No valid Harmonic Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_GE_ReturnCode_InvalidPosition == hpf__->addMovableObstacle(mpl_p) ){
            MPL_CORE_ERROR_MSG_START QString("HPF: No valid position | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        break;
    case MPL_Core_OPF:
        if(NULL == opf__){
            MPL_CORE_ERROR_MSG_START "No valid Oriented Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_GE_ReturnCode_InvalidPosition == opf__->addMovableObstacle(mpl_p) ){
            MPL_CORE_ERROR_MSG_START QString("OPF: No valid position | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        break;
    default:
        MPL_CORE_ERROR_MSG_START QString("No valid core state | ") + qs_aux MPL_CORE_ERROR_MSG_END
        break;
    }

    return;
}


void MPL_Core::clearListOfMovableObstacle(void)
{

    QString qs_aux = "    clearListOfMovableObstacle";
    if(b_log__){
        emit signal_logMessage( qs_aux );
    }

    switch (coreMode__)
    {
    case MPL_Core_HPF:
        if(NULL == hpf__){
            MPL_CORE_ERROR_MSG_START "No valid Harmonic Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_GE_ReturnCode_ErrorWhileClearing == hpf__->clearListOfMovableObstacle() ){
            MPL_CORE_ERROR_MSG_START QString("HPF: Error while clearing | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        break;
    case MPL_Core_OPF:
        if(NULL == opf__){
            MPL_CORE_ERROR_MSG_START "No valid Oriented Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_GE_ReturnCode_ErrorWhileClearing == opf__->clearListOfMovableObstacle() ){
            MPL_CORE_ERROR_MSG_START QString("OPF: Error while clearing | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        break;
    default:
        MPL_CORE_ERROR_MSG_START QString("No valid core state | ") + qs_aux MPL_CORE_ERROR_MSG_END
        break;
    }

    return;


}





void MPL_Core::addFixedObstacle(MPL_Position &mpl_p)
{


    QString qs_aux = "    addFixedObstacle ("+ QString::number(mpl_p.getX(), 'f', 10) + ", " + QString::number(mpl_p.getY(), 'f', 10) + ", " + QString::number(mpl_p.getZ(), 'f', 10) + ")";
    if(b_log__){
        emit signal_logMessage( qs_aux );
    }

    switch (coreMode__)
    {
    case MPL_Core_HPF:
        if(NULL == hpf__){
            MPL_CORE_ERROR_MSG_START "No valid Harmonic Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_GE_ReturnCode_InvalidPosition == hpf__->addFixedObstacle(mpl_p) ){
            MPL_CORE_ERROR_MSG_START QString("HPF: No valid position | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        break;
    case MPL_Core_OPF:
        if(NULL == opf__){
            MPL_CORE_ERROR_MSG_START "No valid Oriented Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_GE_ReturnCode_InvalidPosition == opf__->addFixedObstacle(mpl_p) ){
            MPL_CORE_ERROR_MSG_START QString("OPF: No valid position | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        break;
    default:
        MPL_CORE_ERROR_MSG_START QString("No valid core state | ") + qs_aux MPL_CORE_ERROR_MSG_END
        break;
    }

    return;
}


void MPL_Core::clearListOfFixedObstacle(void)
{

    QString qs_aux = "    clearListOfFixedObstacle";
    if(b_log__){
        emit signal_logMessage( qs_aux );
    }

    switch (coreMode__)
    {
    case MPL_Core_HPF:
        if(NULL == hpf__){
            MPL_CORE_ERROR_MSG_START "No valid Harmonic Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_GE_ReturnCode_ErrorWhileClearing == hpf__->clearListOfFixedObstacle() ){
            MPL_CORE_ERROR_MSG_START QString("HPF: Error while clearing | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        break;
    case MPL_Core_OPF:
        if(NULL == opf__){
            MPL_CORE_ERROR_MSG_START "No valid Oriented Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_GE_ReturnCode_ErrorWhileClearing == opf__->clearListOfFixedObstacle() ){
            MPL_CORE_ERROR_MSG_START QString("OPF: Error while clearing | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        break;
    default:
        MPL_CORE_ERROR_MSG_START QString("No valid core state | ") + qs_aux MPL_CORE_ERROR_MSG_END
        break;
    }

    return;
}





void MPL_Core::addEnemyRobot(MPL_Position &mpl_p)
{


    QString qs_aux = "     addEnemyRobot ("+ QString::number(mpl_p.getX(), 'f', 10) + ", " + QString::number(mpl_p.getY(), 'f', 10) + ", " + QString::number(mpl_p.getZ(), 'f', 10) + ")";
    if(b_log__){
        emit signal_logMessage( qs_aux );
    }

    switch (coreMode__)
    {
    case MPL_Core_HPF:
        if(NULL == hpf__){
            MPL_CORE_ERROR_MSG_START "No valid Harmonic Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_GE_ReturnCode_InvalidPosition == hpf__->addEnemyRobot(mpl_p) ){
            MPL_CORE_ERROR_MSG_START QString("HPF: No valid position | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        break;
    case MPL_Core_OPF:
        if(NULL == opf__){
            MPL_CORE_ERROR_MSG_START "No valid Oriented Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_GE_ReturnCode_InvalidPosition == opf__->addEnemyRobot(mpl_p) ){
            MPL_CORE_ERROR_MSG_START QString("OPF: No valid position | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        break;
    default:
        MPL_CORE_ERROR_MSG_START QString("No valid core state | ") + qs_aux MPL_CORE_ERROR_MSG_END
        break;
    }

    return;
}


void MPL_Core::clearListOfEnemyRobot(void)
{

    QString qs_aux = "    clearListOfEnemyRobot";
    if(b_log__){
        emit signal_logMessage( qs_aux );
    }

    switch (coreMode__)
    {
    case MPL_Core_HPF:
        if(NULL == hpf__){
            MPL_CORE_ERROR_MSG_START "No valid Harmonic Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_GE_ReturnCode_ErrorWhileClearing == hpf__->clearListOfEnemyRobot() ){
            MPL_CORE_ERROR_MSG_START QString("HPF: Error while clearing | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        break;
    case MPL_Core_OPF:
        if(NULL == opf__){
            MPL_CORE_ERROR_MSG_START "No valid Oriented Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_GE_ReturnCode_ErrorWhileClearing == opf__->clearListOfEnemyRobot() ){
            qWarning("Point 4");
            MPL_CORE_ERROR_MSG_START QString("OPF: Error while clearing | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        break;
    default:
        MPL_CORE_ERROR_MSG_START QString("No valid core state | ") + qs_aux MPL_CORE_ERROR_MSG_END
        break;
    }

    return;


}



void MPL_Core::addGoal(MPL_Position &mpl_p)
{


    QString qs_aux = "    addGoal ("+ QString::number(mpl_p.getX(), 'f', 10) + ", " + QString::number(mpl_p.getY(), 'f', 10) + ", " + QString::number(mpl_p.getZ(), 'f', 10) + ")";
    if(b_log__){
        emit signal_logMessage( qs_aux );
    }

    switch (coreMode__)
    {
    case MPL_Core_HPF:
        if(NULL == hpf__){
            MPL_CORE_ERROR_MSG_START "No valid Harmonic Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_GE_ReturnCode_InvalidPosition == hpf__->addGoal(mpl_p) ){
            MPL_CORE_ERROR_MSG_START QString("HPF: No valid position | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        break;
    case MPL_Core_OPF:
        if(NULL == opf__){
            MPL_CORE_ERROR_MSG_START "No valid Oriented Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_GE_ReturnCode_InvalidPosition == opf__->addGoal(mpl_p) ){
            MPL_CORE_ERROR_MSG_START QString("OPF: No valid position | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        break;
    default:
        MPL_CORE_ERROR_MSG_START QString("No valid core state | ") + qs_aux MPL_CORE_ERROR_MSG_END
        break;
    }

    return;
}


void MPL_Core::clearListOfGoal(void)
{

    QString qs_aux = "    clearListOfGoal";
    if(b_log__){
        emit signal_logMessage( qs_aux );
    }

    switch (coreMode__)
    {
    case MPL_Core_HPF:
        if(NULL == hpf__){
            MPL_CORE_ERROR_MSG_START "No valid Harmonic Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_GE_ReturnCode_ErrorWhileClearing == hpf__->clearListOfGoal() ){
            MPL_CORE_ERROR_MSG_START QString("HPF: Error while clearing | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        break;
    case MPL_Core_OPF:
        if(NULL == opf__){
            MPL_CORE_ERROR_MSG_START "No valid Oriented Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_GE_ReturnCode_ErrorWhileClearing == opf__->clearListOfGoal() ){
            MPL_CORE_ERROR_MSG_START QString("OPF: Error while clearing | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        break;
    default:
        MPL_CORE_ERROR_MSG_START QString("No valid core state | ") + qs_aux MPL_CORE_ERROR_MSG_END
        break;
    }

    return;


}



void MPL_Core::slot_getPath(MPL_Position pos_start, MPL_Position *pos_path, int n_maxSteps)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_getPath] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_addGoal]" MPL_CORE_LOG_MSG_END
    }

    getPath(pos_start, pos_path, n_maxSteps);


    return;
}

void MPL_Core::getPath(MPL_Position pos_start, MPL_Position *pos_path, int n_maxSteps)
{
    QString qs_aux = QString("    getPath( P [%1, %2, %3] with %4 max steps").arg(pos_start.getX()).arg(pos_start.getY()).arg(pos_start.getZ()).arg(n_maxSteps);
    if(b_log__){
        emit signal_logMessage( qs_aux );
    }

    switch (coreMode__)
    {
    case MPL_Core_HPF:
        if(NULL == hpf__)
        {
            MPL_CORE_ERROR_MSG_START "No valid Harmonic Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_ReturnCode_OK != hpf__->getPath(pos_start, pos_path, n_maxSteps) )
        {
            MPL_CORE_ERROR_MSG_START QString("HPF: Error while trying to get path | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        break;
    case MPL_Core_OPF:
        if(NULL == opf__)
        {
            MPL_CORE_ERROR_MSG_START "No valid Oriented Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_ReturnCode_OK != opf__->getPath(pos_start, pos_path, n_maxSteps) ){
            MPL_CORE_ERROR_MSG_START QString("OPF: Error while trying to get path | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        break;
    default:
        MPL_CORE_ERROR_MSG_START QString("No valid core state | ") + qs_aux MPL_CORE_ERROR_MSG_END
        break;
    }

    return;
}



void MPL_Core::getPathFromOwnRobot(MPL_Position *pos_path, int n_maxSteps)
{
    QString qs_aux = QString("    getPathFromOwnRobot( with %1 max steps").arg(n_maxSteps);
    if(b_log__){
        emit signal_logMessage( qs_aux );
    }

    switch (coreMode__)
    {
    case MPL_Core_HPF:
        if(NULL == hpf__)
        {
            MPL_CORE_ERROR_MSG_START "No valid Harmonic Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_ReturnCode_OK != hpf__->getPathFromOwnRobot(pos_path, n_maxSteps) )
        {
            MPL_CORE_ERROR_MSG_START QString("HPF: Error while trying to get path from OwnRobot | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        break;
    case MPL_Core_OPF:
        if(NULL == opf__)
        {
            MPL_CORE_ERROR_MSG_START "No valid Oriented Potential Field Environment" MPL_CORE_ERROR_MSG_END
            return;
        }
        if( MPL_ReturnCode_OK != opf__->getPathFromOwnRobot(pos_path, n_maxSteps) ){
            MPL_CORE_ERROR_MSG_START QString("OPF: Error while trying to get path from OwnRobot | ") + qs_aux MPL_CORE_ERROR_MSG_END
        }
        break;
    default:
        MPL_CORE_ERROR_MSG_START QString("No valid core state | ") + qs_aux MPL_CORE_ERROR_MSG_END
        break;
    }

    return;
}

void MPL_Core::slot_getPathFromOwnRobot(MPL_Position *pos_path, int n_maxSteps)
{
    if( NULL == locker__ ){
        MPL_CORE_ERROR_MSG_START "[MPL_Core::slot_getPath] No Locker have been found" MPL_CORE_ERROR_MSG_END
        return;
    }

    QWriteLocker wrlocker(locker__);

    if(b_log__)//verbose
    {
        MPL_CORE_LOG_MSG_START "[MPL_Core::slot_addGoal]" MPL_CORE_LOG_MSG_END
    }

    getPathFromOwnRobot(pos_path, n_maxSteps);


    return;
}

