#include "mpl_opf_parallelsor_redblack.h"

MPL_OPF_ParallelSor_RedBlack::MPL_OPF_ParallelSor_RedBlack(QObject *parent) : QObject(parent)
{

    locker_resource__ = NULL;
    n_resource__ = NULL;
    state_external__ = NULL;

    pqv_potential3DMatrix__ = NULL;
    pqvMPLSCGT_occupancy3DMatrix__ = NULL;
    color__ = PSor_UnknownColor;
    state__ = PSor_NotInitialized;
    d_omegaSor__ = 1;

    zMax__ = 0;
    yMax__ = 0;
    xMax__ = 0;

    d_lambda_vx__=0;
    d_lambda_vy__=0;
    d_lambda_vz__=0;
    d_denom__=1;

    d_acmResidue__=0;

    n_default_usecSleep__ = 10;

    isRunning = false;
}


void MPL_OPF_ParallelSor_RedBlack::setAll(
        MPL_OPF_ParallelSor_Color color, \
        int *n_resource, \
        MPL_OPF_ParallelSor_ExternalState *state_external,\
        QReadWriteLock *locker_resource, \
        double *d_residue, \
        QReadWriteLock *locker_residue, \
        QVector<MPL_StateCellGridType> *pqvMPLSCGT_occupancy3DMatrix, \
        QVector<MPL_PFE_VarType> *pqv_potential3DMatrix, \
        double d_omegaSor, \
        double d_epsilon, \
        double d_vx, \
        double d_vy, \
        double d_vz, \
        int xMax, \
        int yMax, \
        int zMax)
{

    if( (NULL == pqv_potential3DMatrix) || (NULL == pqvMPLSCGT_occupancy3DMatrix) )
    {
        emit signal_errorMessage("[MPL_OPF_ParallelSor_RedBlack::setOPF] Null vector field or occupancy grid");
        qWarning("[MPL_OPF_ParallelSor_RedBlack::setOPF] Null vector field or occupancy grid");
        return;
    }

    color__ = color;
    qs_color__ =(PSor_red==color? " RED " : (PSor_black==color?"BLACK":"UNKNOWN COLOR") );
    state_external__ = state_external;
    n_resource__ = n_resource;
    locker_resource__ = locker_resource;

    d_residue__ = d_residue;
    locker_residue__ = locker_residue;

    pqv_potential3DMatrix__ = pqv_potential3DMatrix;
    d_omegaSor__ = d_omegaSor;
    pqvMPLSCGT_occupancy3DMatrix__ = pqvMPLSCGT_occupancy3DMatrix;


    zMax__ = zMax;
    yMax__ = yMax;
    xMax__ = xMax;

    d_lambda_vx__=(1 + (d_epsilon*fabs(d_vx)));
    d_lambda_vy__=(1 + (d_epsilon*fabs(d_vy)));
    d_lambda_vz__=(1 + (d_epsilon*fabs(d_vz)));

    d_denom__=(6 + d_epsilon * (fabs(d_vx) + fabs(d_vy) + fabs(d_vz)) );

    //upwinding
    if( d_vx < 0 )
    {//invert
        i_xDown__ = 0;
        i_xUp__ = 1;
    }
    else
    {//usual
        i_xDown__ = 1;
        i_xUp__ = 0;
    }

    if( d_vy < 0 )
    {//invert
        i_yDown__ = 2;
        i_yUp__ = 3;
    }
    else
    {//usual
        i_yDown__ = 3;
        i_yUp__ = 2;
    }

    if( d_vz < 0 )
    {//invert
        i_zDown__ = 4;
        i_zUp__ = 5;
    }
    else
    {//usual
        i_zDown__ = 5;
        i_zUp__ = 4;
    }

    state__ = PSor_Initialized;
    isRunning = false;

}







void MPL_OPF_ParallelSor_RedBlack::Iterate( void )
{
    unsigned int n_aux0=0;
    d_acmResidue__=0;
    for( k = 0; k<zMax__; k+=1)
    {
        if( PSor_black == color__ )
        {
            j_aux__ = ((k)&0x1); //black x-start
        }
        else
        {
            j_aux__ = ((k+1)&0x1); //red x-start
        }
        for(i = 0; i<yMax__; i+=1)
        {
            //using x-start from above
            for(j = j_aux__; j<xMax__; j+=2)
            {

                if( unsafe_isOccupied(j, i, k) )
                {//isnt free or inexplored
                    continue;
                }
/*
                //POG - debug pog
                n_aux0 = i + j + k;
                if( (n_aux0 > 10) && (n_aux0 < 20) && (0 == (k%2) ) && (0 == (i%2) ) )
                {
                    qWarning(QString("[%1](i, j, k) = (%2, %3, %4)").arg(qs_color__).arg(i).arg(j).arg(k).toLatin1() );
                }
                //POG - debug pog
                d_oldPotential__ = (*pqv_potential3DMatrix__)[ unsafe_arrayIndex(j, i, k) ];

                //x - neighborhood
                if(xMax__>1)
                {
                    if(0==j)
                    {
                         d_neighbor__[0] = (*pqv_potential3DMatrix__)[ unsafe_arrayIndex(j+1, i, k) ];
                         d_neighbor__[1] = MPL_PFE_MaxPotential;
                    }
                    else
                    {
                        if((xMax__-1)==j)
                        {
                            d_neighbor__[0] = MPL_PFE_MaxPotential;
                            d_neighbor__[1] = (*pqv_potential3DMatrix__)[ unsafe_arrayIndex(j-1, i, k) ];
                        }
                        else
                        {
                            d_neighbor__[0] = (*pqv_potential3DMatrix__)[ unsafe_arrayIndex(j+1, i, k) ];
                            d_neighbor__[1] = (*pqv_potential3DMatrix__)[ unsafe_arrayIndex(j-1, i, k) ];
                        }
                    }
                }
                else
                {
                    d_neighbor__[0] = MPL_PFE_MaxPotential;
                    d_neighbor__[1] = MPL_PFE_MaxPotential;
                }

                //y - neighborhood
                if(yMax__>1)
                {
                    if(0==i)
                    {
                         d_neighbor__[2] = (*pqv_potential3DMatrix__)[ unsafe_arrayIndex(j, i+1, k) ];
                         d_neighbor__[3] = MPL_PFE_MaxPotential;
                    }
                    else
                    {
                        if((yMax__-1)==i)
                        {
                            d_neighbor__[2] = MPL_PFE_MaxPotential;
                            d_neighbor__[3] = (*pqv_potential3DMatrix__)[ unsafe_arrayIndex(j, i-1, k) ];
                        }
                        else
                        {
                            d_neighbor__[2] = (*pqv_potential3DMatrix__)[ unsafe_arrayIndex(j, i+1, k) ];
                            d_neighbor__[3] = (*pqv_potential3DMatrix__)[ unsafe_arrayIndex(j, i-1, k) ];
                        }
                    }
                }
                else
                {
                    d_neighbor__[2] = MPL_PFE_MaxPotential;
                    d_neighbor__[3] = MPL_PFE_MaxPotential;
                }

                //z - neighborhood
                if(zMax__>1)
                {
                    if(0==k)
                    {
                         d_neighbor__[4] = (*pqv_potential3DMatrix__)[ unsafe_arrayIndex(j, i, k+1) ];
                         d_neighbor__[5] = MPL_PFE_MaxPotential;
                    }
                    else
                    {
                        if((zMax__-1)==k)
                        {
                            d_neighbor__[4] = MPL_PFE_MaxPotential;
                            d_neighbor__[5] = (*pqv_potential3DMatrix__)[ unsafe_arrayIndex(j, i, k-1) ];
                        }
                        else
                        {
                            d_neighbor__[4] = (*pqv_potential3DMatrix__)[ unsafe_arrayIndex(j, i, k+1) ];
                            d_neighbor__[5] = (*pqv_potential3DMatrix__)[ unsafe_arrayIndex(j, i, k-1) ];
                        }
                    }
                }
                else
                {
                    d_neighbor__[4] = MPL_PFE_MaxPotential;
                    d_neighbor__[5] = MPL_PFE_MaxPotential;
                }

                d_newPotential__ = ((d_neighbor__[i_xUp__]*d_lambda_vx__ + d_neighbor__[i_xDown__]+\
                                     d_neighbor__[i_yUp__]*d_lambda_vy__ + d_neighbor__[i_yDown__]+\
                                     d_neighbor__[i_zUp__]*d_lambda_vz__ + d_neighbor__[i_zDown__])/d_denom__);

                //SORing
                d_newPotential__ = ( ((1-d_omegaSor__)*d_oldPotential__) + (d_omegaSor__*d_newPotential__)  );

                (*pqv_potential3DMatrix__)[ unsafe_arrayIndex(j, i, k) ] = d_newPotential__;

                d_acmResidue__ += pow((d_newPotential__ - d_oldPotential__), 2);
*/
                //get neighbors
                //update potential values
                //set error matrix
            }
            j_aux__ = ((j_aux__+1)&0x1);//alternate-hash; one line starts at 0, the following at 1
        }
    }
    emit signal_IterationResidue( d_acmResidue__ );
}


void MPL_OPF_ParallelSor_RedBlack::Start( void )
{
    switch( state__ )
    {
    case PSor_NotInitialized:
        emit signal_errorMessage(QString("[MPL_OPF_ParallelSor_RedBlack::Start][%1 thread] Not Initialized").arg(qs_color__) );
        return;
    case PSor_Running:
        emit signal_errorMessage(QString("[MPL_OPF_ParallelSor_RedBlack::Start][%1 thread] Already Running").arg(qs_color__) );
        return;
    case PSor_Waiting:
        emit signal_errorMessage(QString("[MPL_OPF_ParallelSor_RedBlack::Start][%1 thread] Already Running, but in waiting cycle").arg(qs_color__) );
        return;
    default:
        emit signal_errorMessage(QString("[MPL_OPF_ParallelSor_RedBlack::Start][%1 thread] in Unknown State").arg(qs_color__) );
        return;
    }

    //only state PSor_Stopped or PSor_Initialized here
    state__ = PSor_Running;

    isRunning = true;
    itCount__ = 0;

    int n_aux4=0; //POG REMOVER
    int n_aux=0; //POG REMOVER

    while(isRunning)
    {
        switch (state__)
        {
        case PSor_Running:
            locker_resource__->lockForRead();
            if( PSor_External_Start == (*state_external__) )
            {
                if( (*n_resource__) > 0)
                {
                    itCount__ = *n_resource__;
                    locker_resource__->unlock();
                }
                else
                {
                    itCount__ = 0;
                    state__ = PSor_Waiting;
                    locker_resource__->unlock();
                    break; //force to go back to the switch
                }
                n_aux4+=itCount__;
                for( it__ =0; it__<itCount__; it__ ++)
                {
                    Iterate();
                }
                locker_resource__->lockForWrite();
                    *n_resource__ = 0; //consume, because already have iterated
                locker_resource__->unlock();

                locker_residue__->lockForWrite();
                    *d_residue__ = d_acmResidue__;
                locker_residue__->unlock();
                if( 0 == n_aux4%50 )
                {
                    //qWarning(QString("[ %1 ] Iterating: %2" ).arg(qs_color__).arg(n_aux4).toLatin1());
                }
                //iterate??
                //Update Residue Value without emiting signal?
            }
            else
            {
                if( PSor_External_Stop == (*state_external__) ) //external stop
                {
                    state__ = PSor_Stopped;
                    isRunning = false;
                    locker_resource__->unlock();
                    return;
                }
                else //something VERY wrong
                {
                    emit signal_errorMessage(QString("[MPL_OPF_ParallelSor_RedBlack::Start][%1 thread] Unknown External State").arg(qs_color__) );
                    state__ = PSor_Stopped;
                    isRunning = false;
                    locker_resource__->unlock();
                    continue;
                }
            }
            break;
        case PSor_Waiting:
             //sleep for n_default_usecSleep__ usecs
            //this->thread()->usleep(n_default_usecSleep__);
            state__ = PSor_Running;
            break;
        default: //something VERY wrong
            emit signal_errorMessage("[MPL_OPF_ParallelSor_RedBlack::Start] Invalid Internal State");
            //qWarning(QString("[MPL_OPF_ParallelSor_RedBlack::Start][%1 thread] Invalid Internal State").arg(qs_color__).toLatin1() );
            return;
        }

    }
    return;
}

void MPL_OPF_ParallelSor_RedBlack::Stop( void )
{
    state__ = PSor_Stopped;
    return;
}


void MPL_OPF_ParallelSor_RedBlack::Quit( void )
{
    Stop();
    emit signal_finished();
    return;
}

void MPL_OPF_ParallelSor_RedBlack::slot_Start( void )
{
    QTimer::singleShot(1, this, SLOT(Start()) );
}

void  MPL_OPF_ParallelSor_RedBlack::slot_Stop( void )
{
    QTimer::singleShot(1, this, SLOT(Stop()) );
}

void  MPL_OPF_ParallelSor_RedBlack::slot_Quit( void )
{
    QTimer::singleShot(1, this, SLOT(Quit()) );
}
