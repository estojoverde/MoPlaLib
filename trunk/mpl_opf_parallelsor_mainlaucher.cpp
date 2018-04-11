#include "mpl_opf_parallelsor_mainlaucher.h"

MPL_OPF_ParallelSor_MainLaucher::MPL_OPF_ParallelSor_MainLaucher(QObject *parent) : QObject(parent)
{

    internal_state_ = kState_PSOR_MainLaucher_Unknown;


    opf_ = NULL;
    locker_opf_ = NULL;



    b_verbose_ = false;

    i_iteration_reached_ = 0;
    d_residue_reached_ = 0;
    d_residue_goal_ = 0;
    i_iteration_goal_ = 0;


    n_resource_red_ = 0;
    d_residue_red_ = 0;
    p_thread_red_ = NULL;
    pPSOR_sor_red_ = NULL;
    state_red__ = MPL_OPF_ParallelSor_RedBlack::PSor_UnknownExternalState;


    n_resource_black_ = 0;
    d_residue_black_ = 0;
    p_thread_black_ = NULL;
    pPSOR_sor_black_ = NULL;
    state_black__ = MPL_OPF_ParallelSor_RedBlack::PSor_UnknownExternalState;


    internal_state_ = kState_PSOR_MainLaucher_NotInitialized;



}



MPL_OPF_ParallelSor_MainLaucher::~MPL_OPF_ParallelSor_MainLaucher()
{
    //delete opf__;

//    if (b_initialized__)
//    {
//        delete psor_red__;
//        delete psor_black__;

//        threadRed__->deleteLater();
//        threadBlack__->deleteLater();
//    }
}






void MPL_OPF_ParallelSor_MainLaucher::SetOPF(MPL_OrientedPotentialField *opf, QReadWriteLock *locker)
{
    if(b_verbose_)//verbose
    {
        emit signal_LogMessage("    [MPL_OPF_ParallelSor_MainLaucher::setOPF]");
    }

    locker_opf_ = locker;
    opf_ = opf;
    connect(this, SIGNAL(signal_ErrorMessage(QString)), opf_, SIGNAL(signal_errorMessage(QString)) );
    connect(this, SIGNAL(signal_LogMessage(QString)), opf_, SIGNAL(signal_logMessage(QString)) );

    CreateChildProcess();


    internal_state_ = kState_PSOR_MainLaucher_Initialized;

}

void MPL_OPF_ParallelSor_MainLaucher::CreateChildProcess( void )
{
    //check if created, if not, create

    if(NULL == p_thread_red_ ){
        p_thread_red_ = new QThread();
        pPSOR_sor_red_ = new MPL_OPF_ParallelSor_RedBlack();
        p_thread_red_->start();
        pPSOR_sor_red_->moveToThread(p_thread_red_);
        if(b_verbose_)//verbose
        {
            emit signal_LogMessage("      - Creating Child Red");
            if( p_thread_red_->isRunning() )
            {
                emit signal_LogMessage("         - Child Red Running");
            }
            else
            {
                emit signal_LogMessage("         - Child Red NOT Running");
            }
        }
    }
    else{
        if(b_verbose_)//verbose
        {
            if( p_thread_red_->isRunning() )
            {
                emit signal_LogMessage("      - Child Red Already Created and Running");
            }
            else
            {
                emit signal_LogMessage("      - Child Red Already Created BUT NOT Running");
            }
        }
    }
    //red connections
    connect(pPSOR_sor_red_, SIGNAL(signal_errorMessage(QString)), this, SIGNAL( signal_ErrorMessage(QString))  );
    connect(pPSOR_sor_red_, SIGNAL(signal_logMessage(QString)), this, SIGNAL( signal_LogMessage(QString)) );
    connect(this, SIGNAL(signal_ChildStop( void )), pPSOR_sor_red_, SLOT(slot_Stop()) );
    connect(this, SIGNAL(signal_ChildStart( void )), pPSOR_sor_red_, SLOT(slot_Start()) );
    connect(this, SIGNAL(signal_ChildQuit( void )), pPSOR_sor_red_, SLOT(slot_Quit()) );
    //set
    pPSOR_sor_red_->setAll( MPL_OPF_ParallelSor_RedBlack::PSor_red,
                            &n_resource_red_,
                            &state_red__,
                            &locker_resource_red_,
                            &d_residue_red_,
                            &locker_residue_red_,
                            &(opf_->qvMPLSCGT_occupancy3DMatrix__),
                            &(opf_->qvMPL_PFE_VarType_potential3DMatrix__),
                            opf_->d_omegaSor__,
                            opf_->d_epsilon__,
                            opf_->d_vx__,
                            opf_->d_vy__,
                            opf_->d_vz__,
                            opf_->d_sizeX__,
                            opf_->d_sizeY__,
                            opf_->d_sizeZ__);


    if(NULL == p_thread_black_ ){
        p_thread_black_ = new QThread();
        pPSOR_sor_black_ = new MPL_OPF_ParallelSor_RedBlack();
        p_thread_black_->start();
        pPSOR_sor_black_->moveToThread(p_thread_black_);
        if(b_verbose_)//verbose
        {
            emit signal_LogMessage("      - Creating Child Black");
            if( p_thread_black_->isRunning() )
            {
                emit signal_LogMessage("         - Child Black Running");
            }
            else
            {
                emit signal_LogMessage("         - Child Black NOT Running");
            }
        }
    }
    else{
        if(b_verbose_)//verbose
        {
            if( p_thread_black_->isRunning() )
            {
                emit signal_LogMessage("      - Child Black Already Created and Running");
            }
            else
            {
                emit signal_LogMessage("      - Child Black Already Created BUT NOT Running");
            }
        }
    }
    //black connections
    connect(pPSOR_sor_black_, SIGNAL(signal_errorMessage(QString)), this, SIGNAL( signal_ErrorMessage(QString)) );
    connect(pPSOR_sor_black_, SIGNAL(signal_logMessage(QString)), this, SIGNAL( signal_LogMessage(QString)) );
    connect(this, SIGNAL(signal_ChildStop( void )), pPSOR_sor_black_, SLOT(slot_Stop()) );
    connect(this, SIGNAL(signal_ChildStart( void )), pPSOR_sor_black_, SLOT(slot_Start()) );
    connect(this, SIGNAL(signal_ChildQuit( void )), pPSOR_sor_black_, SLOT(slot_Quit()) );
    //set
    pPSOR_sor_black_->setAll(   MPL_OPF_ParallelSor_RedBlack::PSor_black,
                                &n_resource_black_,
                                &state_black__,
                                &locker_resource_black_,
                                &d_residue_black_,
                                &locker_residue_black_,
                                &(opf_->qvMPLSCGT_occupancy3DMatrix__),
                                &(opf_->qvMPL_PFE_VarType_potential3DMatrix__),
                                opf_->d_omegaSor__,
                                opf_->d_epsilon__,
                                opf_->d_vx__,
                                opf_->d_vy__,
                                opf_->d_vz__,
                                opf_->d_sizeX__,
                                opf_->d_sizeY__,
                                opf_->d_sizeZ__);


    return;
}



void MPL_OPF_ParallelSor_MainLaucher::SetConnections( void )
{


    return;
}






void MPL_OPF_ParallelSor_MainLaucher::Start( void )
{

    if(kState_PSOR_MainLaucher_NotInitialized == internal_state_)
    {
        emit signal_ErrorMessage("MPL_OPF_ParallelSor_MainLaucher::Start] Not Initialized");
        return;
    }


    internal_state_ = kState_PSOR_MainLaucher_Waiting;


    locker_resource_red_.lockForWrite();
    state_red__ = MPL_OPF_ParallelSor_RedBlack::PSor_External_Start;
    locker_resource_red_.unlock();

    locker_resource_black_.lockForWrite();
    state_black__ = MPL_OPF_ParallelSor_RedBlack::PSor_External_Start;
    locker_resource_black_.unlock();

    emit signal_ChildStart();

    return;
}


void MPL_OPF_ParallelSor_MainLaucher::Stop( void )
{

    if(kState_PSOR_MainLaucher_NotInitialized == internal_state_)
    {
        emit signal_ErrorMessage("MPL_OPF_ParallelSor_MainLaucher::Stop] Not Initialized");
        return;
    }


    internal_state_ = kState_PSOR_MainLaucher_Stopped;


    locker_resource_red_.lockForWrite();
    state_red__ = MPL_OPF_ParallelSor_RedBlack::PSor_External_Stop;
    locker_resource_red_.unlock();

    locker_resource_black_.lockForWrite();
    state_black__ = MPL_OPF_ParallelSor_RedBlack::PSor_External_Stop;
    locker_resource_black_.unlock();

    emit signal_ChildStop();


    return;
}


void MPL_OPF_ParallelSor_MainLaucher::Quit( void )
{

    Stop();

    emit signal_ChildQuit();

    return;
}



/*
 ___ _                 _
|_ _| |_ ___ _ __ __ _| |_ ___  _ __ ___
 | || __/ _ \ '__/ _` | __/ _ \| '__/ __|
 | || ||  __/ | | (_| | || (_) | |  \__ \
|___|\__\___|_|  \__,_|\__\___/|_|  |___/
*/








/*
 ____        _
/ ___|  ___ | |_   _____ _ __ ___
\___ \ / _ \| \ \ / / _ \ '__/ __|
 ___) | (_) | |\ V /  __/ |  \__ \
|____/ \___/|_| \_/ \___|_|  |___/
*/



void MPL_OPF_ParallelSor_MainLaucher::SolveByIteration(int i_max_it)
{

    if( kState_PSOR_MainLaucher_Waiting != internal_state_){
        emit signal_ErrorMessage("MPL_OPF_ParallelSor_MainLaucher::SolveByIteration] Not Waiting");
        return;
    }


    internal_state_ = kState_PSOR_MainLaucher_Solving;


    i_iteration_reached_=-1; //POG, since n_resource_red_/black__ starts with 0 and it counts as an interation
    d_residue_reached_=100;
    d_residue_goal_ = 0;
    n_auxHalfIteration_black_ = 1; // POG, because red are being tested!
    n_auxHalfIteration_red_ = 0;

    i_iteration_goal_ = i_max_it;
    //i_iteration_goal_ = 50;// POG, because red are being tested!

    int n_aux=0;

    do{

        //red
        locker_resource_red_.lockForRead();
        if( 0 == n_resource_red_ )
        {
            locker_resource_red_.unlock();
            locker_resource_red_.lockForWrite();
            n_resource_red_ = 1;
            locker_resource_red_.unlock();
            n_auxHalfIteration_red_ =1;
        }
        else
        {
//            if( n_resource_red_ < 3) //up to three iterations ahead... magical number, I know... but (probably) works better than nothing
//            {
//                n_resource_red_++;
//            }
            //Has a bug... if up to three iterations ahead are allowed, halfIterations scheme needs to be modified
            //since it is working just for binary states.
            locker_resource_red_.unlock();
        }


        //black
        locker_resource_black_.lockForRead();
        if( 0 == n_resource_black_ )
        {
            locker_resource_black_.unlock();
            locker_resource_black_.lockForWrite();
            n_resource_black_ = 1;
            locker_resource_black_.unlock();
            n_auxHalfIteration_black_ =1;
        }
        else
        {
            locker_resource_black_.unlock();
        }


        if( (1 == n_auxHalfIteration_red_) && (1 == n_auxHalfIteration_black_) )
        {
            i_iteration_reached_++;
            n_auxHalfIteration_red_ = 0;
            n_auxHalfIteration_black_ = 0;
            n_aux=0;
        }
        else
        {
            n_aux++;

            if( n_aux >= 1000 )
            {
                if( 0 == n_aux%1000 )
                    qWarning(QString("[MAIN] N I [4]: %1" ).arg(n_aux).toLatin1());
                this->thread()->usleep(500);
            }
            if( n_aux >= 100 )
            {
                if( 0 == n_aux%100 )
                    qWarning(QString("[MAIN] N I [3]: %1" ).arg(n_aux).toLatin1());
                this->thread()->usleep(20);
            }
            else if( n_aux >= 20 )
            {
                //if( 0 == n_aux%20 )
                    //qWarning(QString("[MAIN] N I [2]: %1" ).arg(n_aux).toLatin1());
                this->thread()->usleep(5);
            }
            else if( n_aux >= 5 )
            {
                //if( 0 == n_aux%5 )
                    //qWarning(QString("[MAIN] N I [1]: %1" ).arg(n_aux).toLatin1());
                this->thread()->usleep(2);
            }

        }

    }while( (i_iteration_reached_ < i_iteration_goal_) );


    if(b_verbose_)//verbose
    {
        emit signal_LogMessage( QString("    Iterator - Iterations(%1) - Error(%2)").arg(i_iteration_reached_).arg(d_residue_reached_) );
    }

    emit signal_PDE_Solved(i_iteration_reached_, d_residue_reached_);


    internal_state_ = kState_PSOR_MainLaucher_Waiting;


    return;
}



void MPL_OPF_ParallelSor_MainLaucher::SolveByResidue(double d_max_residue)
{
//    i__=0;
//    d_error__=100;

//    do{
//        //iterator_SOR(d_error__);
//        i__++;
//    }while( d_error__>d_maxError );

//    if(b_log__)//verbose
//    {
//        MPL_SGN_LOG_MSG_START QString("    Iterator - Iterations(%1) - Error(%2)").arg(i__).arg(d_error__) MPL_SGN_LOG_MSG_END;
//    }

//    emit signal_pfSolved(i__, d_error__);

//    return(MPL_ReturnCode_OK);
}


void MPL_OPF_ParallelSor_MainLaucher::SolveByIterationAndResidue(int i_max_it, double d_max_residue)
{
//    i__=0;
//    d_error__=100;

//    do{
//        //iterator_SOR(d_error__);
//        i__++;
//    }while( d_error__>d_maxError && (i__<i_maxIt) );

//    if(b_log__)//verbose
//    {
//        MPL_SGN_LOG_MSG_START QString("    Iterator - Iterations(%1) - Error(%2)").arg(i__).arg(d_error__) MPL_SGN_LOG_MSG_END;
//    }

//    emit signal_pfSolved(i__, d_error__);


//    return(MPL_ReturnCode_OK);
}







/*
 ____  _       _
/ ___|| | ___ | |_ ___
\___ \| |/ _ \| __/ __|
 ___) | | (_) | |_\__ \
|____/|_|\___/ \__|___/

*/





void MPL_OPF_ParallelSor_MainLaucher::slot_Start( void )
{
    if( b_verbose_)
    {
        emit signal_LogMessage(QString("[MPL_OPF_ParallelSor_MainLaucher::slot_Start]") );
    }
    Start();
}


void MPL_OPF_ParallelSor_MainLaucher::slot_Stop( void )
{
    if(b_verbose_)
    {
        emit signal_LogMessage(QString("[MPL_OPF_ParallelSor_MainLaucher::slot_Stop]") );
    }
    Stop();
}


void MPL_OPF_ParallelSor_MainLaucher::slot_SolveByIteration(int i_max_it)
{
    if(b_verbose_)
    {
        emit signal_LogMessage(QString("MPL_OPF_ParallelSor_MainLaucher::slot_SolveByIteration[ %1 ]").arg(i_max_it) );
    }


    SolveByIteration(i_max_it);
}


//void MPL_OPF_ParallelSor_MainLaucher::slot_initiate()
//{

//    if( b_initialized__){
//        return;
//    }

//    if( NULL == locker__ ){
//        MPL_SGN_ERROR_MSG_START "[MPL_OPF_ParallelSor_MainLaucher::slot_initiate] No Locker have been found" MPL_SGN_ERROR_MSG_END
//        return;
//    }

//    QWriteLocker wrlocker(locker__);

//    if(b_log__)//verbose
//    {
//        MPL_SGN_LOG_MSG_START "[MPL_OPF_ParallelSor_MainLaucher::slot_initiate]" MPL_SGN_LOG_MSG_END
//    }




//    threadRed__ = new QThread();
//    psor_red__ = new MPL_OPF_ParallelSor_RedBlack();
//    psor_red__->moveToThread(threadRed__);


//    threadBlack__ = new QThread();
//    psor_black__ = new MPL_OPF_ParallelSor_RedBlack();
//    psor_black__->moveToThread(threadBlack__);

//}


void MPL_OPF_ParallelSor_MainLaucher::slot_SetOPF(MPL_OrientedPotentialField *opf, QReadWriteLock *locker)
{
    if(b_verbose_)//verbose
    {
        emit signal_LogMessage( "[MPL_OPF_ParallelSor_MainLaucher::slot_SetOPF]" );
    }

    SetOPF(opf, locker);

    return;
}

//void MPL_OPF_ParallelSor_MainLaucher::slot_solveI(int i_maxIt)
//{
//    if( NULL == locker__ ){
//        MPL_SGN_ERROR_MSG_START "[MPL_OPF_ParallelSor_MainLaucher::slot_solveI] No Locker have been found" MPL_SGN_ERROR_MSG_END
//        return;
//    }

//    QWriteLocker wrlocker(locker__);

//    if(b_log__)//verbose
//    {
//        MPL_SGN_LOG_MSG_START "[MPL_OPF_ParallelSor_MainLaucher::slot_solveI]" MPL_SGN_LOG_MSG_END
//    }

//    solveI(i_maxIt);
//    return;
//}

//void MPL_OPF_ParallelSor_MainLaucher::slot_solveE(double d_maxError)
//{
//    if( NULL == locker__ ){
//        MPL_SGN_ERROR_MSG_START "[MPL_OPF_ParallelSor_MainLaucher::slot_solveE] No Locker have been found" MPL_SGN_ERROR_MSG_END
//        return;
//    }

//    QWriteLocker wrlocker(locker__);

//    if(b_log__)//verbose
//    {
//        MPL_SGN_LOG_MSG_START "[MPL_OPF_ParallelSor_MainLaucher::slot_solveE]" MPL_SGN_LOG_MSG_END
//    }

//    solveE(d_maxError);
//    return;
//}

//void MPL_OPF_ParallelSor_MainLaucher::slot_solveIE(int i_maxIt, double d_maxError)
//{
//    if( NULL == locker__ ){
//        MPL_SGN_ERROR_MSG_START "[MPL_OPF_ParallelSor_MainLaucher::slot_solveIE] No Locker have been found" MPL_SGN_ERROR_MSG_END
//        return;
//    }

//    QWriteLocker wrlocker(locker__);

//    if(b_log__)//verbose
//    {
//        MPL_SGN_LOG_MSG_START "[MPL_OPF_ParallelSor_MainLaucher::slot_solveIE]" MPL_SGN_LOG_MSG_END
//    }


//    solveIE(i_maxIt, d_maxError);
//    return;
//}
