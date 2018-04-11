#ifndef MPL_OPF_PARALLELSOR_REDBLACK_H
#define MPL_OPF_PARALLELSOR_REDBLACK_H

#include <math.h>

#include <QObject>
#include <QVector>
#include <QString>
#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>
#include <QThread>
#include <QTimer>
#include <QTime>

#include "mpl_potentialfieldenvironment.h"
#include "mpl_orientedpotentialfield.h"

class MPL_OPF_ParallelSor_RedBlack : public QObject
{
    Q_OBJECT

    enum MPL_OPF_ParallelSor_Color{ PSor_UnknownColor, PSor_red, PSor_black};

    enum MPL_OPF_ParallelSor_State{ PSor_Unknown, PSor_NotInitialized, PSor_Initialized, PSor_Running, PSor_Waiting, PSor_Stopped };

    enum MPL_OPF_ParallelSor_ExternalState{ PSor_UnknownExternalState, PSor_External_Start, PSor_External_Stop };

    friend class MPL_OPF_ParallelSor_MainLaucher;

public:
    explicit MPL_OPF_ParallelSor_RedBlack(QObject *parent = 0);

    void setAll(MPL_OPF_ParallelSor_Color color, \
        int *n_resource, \
        MPL_OPF_ParallelSor_ExternalState *state_external, \
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
        int zMax);

private:

    inline bool isValidPosition(long int x, long int y, long int z)
    {
        if(  (x>=0) && (x<xMax__) && (y>=0) && (y<yMax__) && (z>=0) && (z<zMax__) ){
            return true;
        }
        return false;
    }

    inline long unsafe_arrayIndex(long int x, long int y, long int z)
    {
        return( (x*yMax__ + y)*zMax__ + z );
    }

    inline bool unsafe_isOccupied(long int x, long int y, long int z)
    {
        int i_array;

        i_array = unsafe_arrayIndex(x,y,z);
        MPL_StateCellGridType stateaux;
        stateaux = pqvMPLSCGT_occupancy3DMatrix__->at(i_array);

        return( ((stateaux==MPL_OccupancyGrid_CellFree) ||\
                (stateaux==MPL_OccupancyGrid_CellNotExplored) ||\
                (stateaux==MPL_OccupancyGrid_CellOccupiedByOwnRobot)) ? false : true );

    }

    inline long arrayIndex(long int x, long int y, long int z)
    {
        if( (x>=0) && (x<xMax__) && (y>=0) && (y<yMax__) && (z>=0) && (z<zMax__) ){ //isValidPosition(...)
            return( (x*yMax__ + y)*zMax__ + z );
        }
        emit signal_errorMessage("[MPL_OPF_ParallelSor_RedBlack::arrayIndex] Invalid Position");
        return( -1 );
    }

    void Iterate( void );

public:
signals:
    void signal_logMessage( QString s );
    void signal_errorMessage( QString s );
    void signal_IterationResidue( double d_err );
    void signal_finished( void );

public slots:

    void slot_Start( void );

    void slot_Stop( void );

    void slot_Quit( void );

private slots:

    void Start( void );
    void Stop( void );
    void Quit( void );

private:
//Parallel controls
    /**
     * @brief state__
     * Stores this object internal state
     */
    MPL_OPF_ParallelSor_State state__;

    /**
     * @brief state_external__
     * Stores this object external state
     */
    MPL_OPF_ParallelSor_ExternalState *state_external__;

    /**
     * @brief color__
     * Stores if this object is red or black.
     */
    MPL_OPF_ParallelSor_Color color__;

    QString qs_color__;

    /**
     * @brief d_residue__
     * Total residue
     */
    volatile double *d_residue__;

    /**
     * @brief locker_residue__
     * Locker of d_residue__
     */
    QReadWriteLock *locker_residue__;

    /**
     * @brief n_resource__
     * Qtd of "resources", i.e., iterarions, being allowed by the mainLauncher class
     */
    volatile int *n_resource__;

    /**
     * @brief locker_resource__
     * Locker of n_resource__
     */
    QReadWriteLock *locker_resource__;

    /**
     * @brief n_default_msecSleep__
     * @todo getter and setter
     *
     * 5ms
     */
    int n_default_usecSleep__;

    /**
     * @brief isRunning
     * to avoid while(true)
     */
    bool isRunning;

//OPF
    /**
     * @brief pqv_potential3DMatrix__
     * The potential matrix itself
     */
    QVector<MPL_PFE_VarType> *pqv_potential3DMatrix__;

    /**
     * @brief pqvMPLSCGT_occupancy3DMatrix__
     * The occupancy matrix itself
     */
    QVector<MPL_StateCellGridType> *pqvMPLSCGT_occupancy3DMatrix__;

    /**
     * @brief d_omegaSor__
     * SOR weighting
     */
    double d_omegaSor__;

    /**
     * @brief zMax__
     * Max Z-axis value
     */
    int zMax__;

    /**
     * @brief yMax__
     * Max Y-axis value
     */
    int yMax__;

    /**
     * @brief xMax__
     * Max X-axis value
     */
    int xMax__;

    //upwind index
    int i_xUp__, i_xDown__, i_yUp__, i_yDown__, i_zUp__, i_zDown__;
    double d_neighbor__[6];

    double d_newPotential__;

    double d_oldPotential__;

    /**
     * @brief d_lambda_vx__
     * d_lambda_vx__ is a better storage than "(1 + (d_epsilon__*fabs(d_vx__)))"
     * since the calculations are done just once
     */
    double d_lambda_vx__;

    /**
     * @brief d_lambda_vy__
     * d_lambda_vy__ is a better storage than "(1 + (d_epsilon__*fabs(d_vy__)))"
     * since the calculations are done just once
     */
    double d_lambda_vy__;

    /**
     * @brief d_lambda_vz__
     * d_lambda_vz__ is a better storage than "(1 + (d_epsilon__*fabs(d_vz__)))"
     * since the calculations are done just once
     */
    double d_lambda_vz__;

    /**
     * @brief d_denom__
     * d_lambda_vx__ is a better storage than "(6 + d_epsilon__ * (fabs(d_vx__) + fabs(d_vy__) + fabs(d_vz__)) );"
     * since the calculations are done just once
     */
    double d_denom__;

    /**
     * @brief i,
     * Iterator on Y-Axis. No need to redeclare it everytime.
     */
    int i;

    /**
     * @brief j
     * Iterator on X-Axis. No need to redeclare it everytime.
     */
    int j;

    /**
     * @brief k
     * Iterator on Z-Axis. No need to redeclare it everytime.
     */
    int k;

    /**
     * @brief j_aux__
     * Initial color of (0,0,Z), since it change every Z.
     */
    int j_aux__;

    /**
     * @brief it__
     * It counter
     *
     */
    int itCount__;

    /**
     * @brief it__
     * Iterator of "iterations" of the relaxation algorithm
     *
     */
    int it__;

    /**
     * @brief d_residue__
     * Acumulator of residues gathered through iterations
     */
    double d_acmResidue__;

};

#endif // MPL_OPF_PARALLELSOR_REDBLACK_H
