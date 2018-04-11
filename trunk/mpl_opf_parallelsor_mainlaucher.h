#ifndef MPL_OPF_PARALLELSOR_MAINLAUCHER_H
#define MPL_OPF_PARALLELSOR_MAINLAUCHER_H

//PSOR

#include <QObject>
#include <QThread>
#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>

#include "mpl_potentialfieldenvironment.h"
#include "mpl_orientedpotentialfield.h"
#include "mpl_opf_parallelsor_redblack.h"


class MPL_OPF_ParallelSor_MainLaucher : public QObject
{
    Q_OBJECT
public:
    explicit MPL_OPF_ParallelSor_MainLaucher(QObject *parent = 0);

    //
    enum MPL_OPFE_PSOR_MainLaucher_State{ kState_PSOR_MainLaucher_Unknown,
                                          kState_PSOR_MainLaucher_NotInitialized,
                                          kState_PSOR_MainLaucher_Initialized,
                                          kState_PSOR_MainLaucher_Waiting,
                                          kState_PSOR_MainLaucher_Solving,
                                          kState_PSOR_MainLaucher_Stopped};



    ~MPL_OPF_ParallelSor_MainLaucher();

private:


    void SetOPF(MPL_OrientedPotentialField *opf, QReadWriteLock *locker);


    void CreateChildProcess( void );


    void SetConnections( void );


    void SolveByIteration(int i_max_it);


    void SolveByResidue(double d_max_residue);


    void SolveByIterationAndResidue(int i_max_it, double d_max_residue);


    void Iterator( void );


    inline void setVerbose(bool b_verbose = true)
    {
        b_verbose_ = b_verbose;
    }


    inline void toggleVerbose()
    {
        b_verbose_ = !b_verbose_;
    }



public:
signals:


    void signal_LogMessage( QString s );


    void signal_ErrorMessage( QString s );


    void signal_PDE_Solved(int n_finalIt, double d_finalError );


    void signal_ChildStart( void );


    void signal_ChildStop( void );


    void signal_ChildQuit( void );


public slots:

    void slot_SetOPF(MPL_OrientedPotentialField *opf, QReadWriteLock *locker);


    void slot_SolveByIteration(int i_max_it);


//    void slot_SolveByResidue(double d_max_residue);


//    void slot_SolveByIterationAndResidue(int i_max_it, double d_max_residue);


    void slot_Start();

    void slot_Stop();

private slots:


    void Start( void );


    void Stop( void );


    void Quit( void );




private:


    MPL_OrientedPotentialField *opf_;


    QReadWriteLock *locker_opf_;



    /**
     * @brief pqv_potential3DMatrix__
     * The potential matrix itself
     */
    MPL_PFE_VarType *p_potential3DMatrix_;

    /**
     * @brief pqvMPLSCGT_occupancy3DMatrix__
     * The occupancy matrix itself
     */
    MPL_StateCellGridType *pMPLSCGT_occupancy3DMatrix_;




    bool b_verbose_;


    MPL_OPFE_PSOR_MainLaucher_State internal_state_;


    int i_iteration_reached_;


    double d_residue_reached_;


    double d_residue_goal_;


    int i_iteration_goal_;





    //RED

    /**
     * @brief n_resource_red_
     * Quantity of resources shared with (or "iterations given to") pPSOR_sor_red_
     */
    int n_resource_red_;

    /**
     * @brief n_auxHalfIteration_red_
     * Tells if Red alread iterated
     */
    int n_auxHalfIteration_red_;

    /**
     * @brief locker_resource_red_
     * Locker of n_resource_red_ shared with pPSOR_sor_red_
     */
    QReadWriteLock locker_resource_red_;


    /**
     * @brief state_red__
     * State of red thread
     */
    MPL_OPF_ParallelSor_RedBlack::MPL_OPF_ParallelSor_ExternalState state_red__;

    /**
     * @brief d_residuered_
     * Residue shared with pPSOR_sor_red_
     */
    double d_residue_red_;

    /**
     * @brief locker_residue_red_
     * Locker of residue d_residue_red_ shared with pPSOR_sor_red_
     */
    QReadWriteLock locker_residue_red_;

    /**
     * @brief threadred__
     * QThread base of pPSOR_sor_red_
     */
    QThread *p_thread_red_;

    /**
     * @brief psor_red_
     * Worker Class red
     */
    MPL_OPF_ParallelSor_RedBlack *pPSOR_sor_red_;


    //BLACK

    /**
     * @brief n_resource_black_
     * Quantity of resources shared with (or "iterations given to") pPSOR_sor_black_
     */
    int n_resource_black_;

    /**
     * @brief n_auxHalfIteration_black_
     * Tells if Black alread iterated
     */
    int n_auxHalfIteration_black_;

    /**
     * @brief locker_resource_black_
     * Locker of n_resource_black_ shared with pPSOR_sor_black_
     */
    QReadWriteLock locker_resource_black_;


    /**
     * @brief state_black__
     * State of black thread
     */
    MPL_OPF_ParallelSor_RedBlack::MPL_OPF_ParallelSor_ExternalState state_black__;

    /**
     * @brief d_residueBlack_
     * Residue shared with pPSOR_sor_black_
     */
    double d_residue_black_;

    /**
     * @brief locker_residue_black_
     * Locker of residue d_residue_black_ shared with pPSOR_sor_black_
     */
    QReadWriteLock locker_residue_black_;

    /**
     * @brief threadBlack__
     * QThread base of pPSOR_sor_black_
     */
    QThread *p_thread_black_;

    /**
     * @brief psor_black_
     * Worker Class Black
     */
    MPL_OPF_ParallelSor_RedBlack *pPSOR_sor_black_;

};

#endif // MPL_OPF_PARALLELSOR_MAINLAUCHER_H
