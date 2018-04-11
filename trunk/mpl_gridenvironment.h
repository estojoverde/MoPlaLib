#ifndef MPL_GRIDENVIRONMENT_H
#define MPL_GRIDENVIRONMENT_H

#include <cstdio>
#include <QVector>
#include <QObject>
#include <QString>
#include "mpl_baseenvironment.h"

typedef enum
{
    MPL_OccupancyGrid_CellNotExplored=0,\
    MPL_OccupancyGrid_CellFree=1,\
    MPL_OccupancyGrid_CellOccupiedByOwnRobot=2,\
    MPL_OccupancyGrid_CellOccupiedByEnemyRobot=4,\
    MPL_OccupancyGrid_CellOccupiedByMovableObstacle=8,\
    MPL_OccupancyGrid_CellOccupiedByFixedObstacle=16,\
    MPL_OccupancyGrid_CellOccupiedByGoal=32

} MPL_StateCellGridType;


//#define MPL_GE_InvalidPosition(p) MPL_ERROR_MESSAGE("Invalid Position at (%02.3lf,%02.3lf,%02.3lf)", (p).getX(), (p).getY(), (p).getZ() )
//#define MPL_GE_AlreadyOccupied(p) MPL_ERROR_MESSAGE("Try adding at an already occupied position (%02.3lf,%02.3lf,%02.3lf)", (p).getX(), (p).getY(), (p).getZ() )
//#define MPL_GE_ErrorWhileRemoving(p) MPL_ERROR_MESSAGE("Error while removing position (%02.3lf,%02.3lf,%02.3lf)", (p).getX(), (p).getY(), (p).getZ() )
//#define MPL_GE_ErrorWhileClearing MPL_ERROR_MESSAGE("Error while clearing")

#define MPL_GE_InvalidPosition(p) MPL_ERROR_MSG_START QString("Invalid Position at (%1,%2,%3)").arg((p).getX()).arg((p).getY()).arg((p).getZ()) MPL_ERROR_MSG_END
#define MPL_GE_InvalidPositionLI(x, y, z) MPL_ERROR_MSG_START QString("Invalid Position at (%1,%2,%3)").arg((x)).arg((y)).arg((z)) MPL_ERROR_MSG_END
#define MPL_GE_AlreadyOccupied(p) MPL_ERROR_MSG_START QString("Tryed adding at an already occupied position (%1,%2,%3)").arg((p).getX()).arg((p).getY()).arg((p).getZ()) MPL_ERROR_MSG_END
#define MPL_GE_ErrorWhileRemoving(p) MPL_ERROR_MSG_START QString("Error while removing position (%1,%2,%3)").arg((p).getX()).arg((p).getY()).arg((p).getZ()) MPL_ERROR_MSG_END
#define MPL_GE_ErrorWhileClearing MPL_ERROR_MSG_START QString("Error while clearing") MPL_ERROR_MSG_END

#define MPL_GE_ReturnCode_InvalidPosition 1
#define MPL_GE_ReturnCode_InvalidGridSize 2
#define MPL_GE_ReturnCode_ErrorWhileRemoving 3
#define MPL_GE_ReturnCode_ErrorWhileClearing 4
#define MPL_GE_ReturnCode_AlreadyOccupied 5

class MPL_GridEnvironment : public MPL_BaseEnvironment
{
    Q_OBJECT


    friend class MPL_OPF_ParallelSor_MainLaucher;

public:

#ifdef NAO_COMPILAR
    MPL_GridEnvironment(    double d_sizeX,\
                            double d_sizeY,\
                            double d_sizeZ,\
                            double d_deltaX=1,\
                            double d_deltaY=1,\
                            double d_deltaZ=1);
#endif

    MPL_GridEnvironment(int n_sizeX, int n_sizeY, int n_sizeZ, QObject *parent=0);

    MPL_GridEnvironment(QObject *parent=0);

    ~MPL_GridEnvironment(void)
    {
        qvMPLSCGT_occupancy3DMatrix__.clear();
    }



    //ownrobot
    /**
     * @brief addOwnRobot
     * @param sMPLP_OR
     */
    inline int setOwnRobot(MPL_Position sMPLP_OR)
    {

        if( isValidPosition(sMPLP_OR) ){

            //clear current position of Own Robot
            MPL_Position p;
            getOwnRobot(p);
            unsafe_setCellFree(p);

            MPL_BaseEnvironment::setOwnRobot(sMPLP_OR);

            unsafe_setCellOccupiedByOwnRobot(sMPLP_OR);

            return(MPL_ReturnCode_OK);

        }
        else{
            MPL_GE_InvalidPosition(sMPLP_OR);

            return(MPL_GE_ReturnCode_InvalidPosition);
        }


    }

    /**
     * @brief getOwnRobotAt
     * @param r_sMPLP_ER
     * @param i_ER
     */
    inline int getOwnRobot(MPL_Position& r_sMPLP_OR){

        MPL_BaseEnvironment::getOwnRobot(r_sMPLP_OR);

        return(MPL_ReturnCode_OK);
    }





    /**
     * @brief addEnemyRobot
     * @param sMPLP_ER
     */
    int addEnemyRobot(MPL_Position sMPLP_ER);

    /**
     * @brief deleteEnemyRobotAt
     * @param i_ER
     */
    int deleteEnemyRobotAt(int i_ER);

    /**
     * @brief clearListOfEnemyRobot
     */
    int clearListOfEnemyRobot();

    /**
     * @brief addMovableObstacle
     * @param sMPLP_MO Posicao do robo
     */
    int addMovableObstacle(MPL_Position sMPLP_MO);

    /**
     * @brief deleteMovableObstacleAt
     * @param i_MO
     * @return
     */
    int deleteMovableObstacleAt(int i_MO);

    /**
     * @brief clearListOfMovableObstacle
     */
    int clearListOfMovableObstacle();



    /**
     * @brief addFixedObstacle
     * @param sMPLP_FO
     */
    int addFixedObstacle(MPL_Position sMPLP_FO);

    /**
     * @brief deleteFixedObstacleAt
     * @param i_FO
     */
    int deleteFixedObstacleAt(int i_FO);

    /**
     * @brief clearListOfFixedObstacle
     */
    int clearListOfFixedObstacle();



    /**
     * @brief addGoal
     * @param sMPLP_G
     */
    int addGoal(MPL_Position sMPLP_G);

    /**
     * @brief deleteGoalAt
     * @param i_G
     */
    int deleteGoalAt(int i_G);

    /**
     * @brief clearListOfGoal
     */
    int clearListOfGoal();




    /**
     * @brief setGridSize
     * @param n_sizeX__
     * @param n_sizeY__
     * @param n_sizeZ__
     */
    int setGridSize(int n_sizeX, int n_sizeY, int n_sizeZ);

    inline int getGridSizeX(void)
    {
        return(this->n_sizeX__);
    }

    inline int getGridSizeY(void)
    {
        return(this->n_sizeY__);
    }

    inline int getGridSizeZ(void)
    {
        return(this->n_sizeZ__);
    }


    inline void unsafe_dumpOccupancyGrid(FILE *out)
    {
        int x;
        int y;
        int z;

        for( z=0; z<n_sizeZ__; z++ ){
            fprintf(out, "\n-------------------\nz=%d\n", z);
            for(y=0; y<n_sizeY__; y++){
                for( x=0; x<n_sizeX__; x++){
                    fprintf(out, "%2d ", qvMPLSCGT_occupancy3DMatrix__[(x*n_sizeY__ + y)*n_sizeZ__ + z ]);
                }
                fprintf(out, "\n");
            }
            fprintf(out, "-------------------\n\n");
        }
    }

    inline int getOccupancy(long int x, long int y, long int z, MPL_StateCellGridType &occup)
    {
        if( isValidPosition(x, y, z) )
        {
            occup = unsafe_getOccupancy(x,y,z);
            return( MPL_ReturnCode_OK );
        }

        MPL_GE_InvalidPositionLI(x, y, z);

        return( MPL_GE_ReturnCode_InvalidPosition );
    }


protected:

    inline bool isValidPosition(MPL_Position& sMPLP_p)
    {
        int x, y, z;
        x = MPL_ROUND_INT( n_sizeX__ * ((d_delta*sMPLP_p.getX())/d_sizeX__));
        y = MPL_ROUND_INT( n_sizeY__ * ((d_delta*sMPLP_p.getY())/d_sizeY__));
        z = MPL_ROUND_INT( n_sizeZ__ * ((d_delta*sMPLP_p.getZ())/d_sizeZ__));

        if(  (x>=0) && (x<n_sizeX__) && (y>=0) && (y<n_sizeY__) && (z>=0) && (z<n_sizeZ__) ){
            return true;
        }
        return false;
    }

    inline bool isValidPosition(long int x, long int y, long int z)
    {
        if(  (x>=0) && (x<n_sizeX__) && (y>=0) && (y<n_sizeY__) && (z>=0) && (z<n_sizeZ__) ){
            return true;
        }
        return false;
    }

    inline long arrayIndex(MPL_Position& sMPLP_p)
    {
        long x, y, z;
        x = MPL_ROUND_INT( n_sizeX__ * ((d_delta*sMPLP_p.getX())/d_sizeX__));
        y = MPL_ROUND_INT( n_sizeY__ * ((d_delta*sMPLP_p.getY())/d_sizeY__));
        z = MPL_ROUND_INT( n_sizeZ__ * ((d_delta*sMPLP_p.getZ())/d_sizeZ__));

        if( (x>=0) && (x<n_sizeX__) && (y>=0) && (y<n_sizeY__) && (z>=0) && (z<n_sizeZ__) ){ //isValidPosition(...)
            return( (x*n_sizeY__ + y)*n_sizeZ__ + z );
        }

        MPL_GE_InvalidPosition(sMPLP_p);

        return( -1 );
    }

    inline long unsafe_arrayIndex(MPL_Position& sMPLP_p)
    {
        long x, y, z;
        x = MPL_ROUND_INT( n_sizeX__ * ((d_delta*sMPLP_p.getX())/d_sizeX__));
        y = MPL_ROUND_INT( n_sizeY__ * ((d_delta*sMPLP_p.getY())/d_sizeY__));
        z = MPL_ROUND_INT( n_sizeZ__ * ((d_delta*sMPLP_p.getZ())/d_sizeZ__));

        return( (x*n_sizeY__ + y)*n_sizeZ__ + z );
    }

    inline long unsafe_arrayIndex(long int x, long int y, long int z)
    {
        return( (x*n_sizeY__ + y)*n_sizeZ__ + z );
    }



    inline bool unsafe_isOccupied(MPL_Position& sMPLP_p)
    {
        return( (qvMPLSCGT_occupancy3DMatrix__[unsafe_arrayIndex(sMPLP_p)]==MPL_OccupancyGrid_CellFree) || \
                (qvMPLSCGT_occupancy3DMatrix__[unsafe_arrayIndex(sMPLP_p)]==MPL_OccupancyGrid_CellNotExplored) ?
                    false : true );
    }


    inline void unsafe_setOccupancy(MPL_StateCellGridType sMPLSCGT_newValue, MPL_Position& sMPLP_p)
    {

        qvMPLSCGT_occupancy3DMatrix__[unsafe_arrayIndex(sMPLP_p)] = sMPLSCGT_newValue;

        return;
    }

    inline MPL_StateCellGridType unsafe_getOccupancy(MPL_Position& sMPLP_p){

        return( qvMPLSCGT_occupancy3DMatrix__[unsafe_arrayIndex(sMPLP_p)] );
    }

    inline MPL_StateCellGridType unsafe_getOccupancy(long int x, long int y, long int z){

        return( qvMPLSCGT_occupancy3DMatrix__[unsafe_arrayIndex(x,y,z)] );
    }


    inline void unsafe_setCellNotExplored(MPL_Position& sMPLP_p)
    {
        unsafe_setOccupancy(MPL_OccupancyGrid_CellNotExplored, sMPLP_p);

        return;
    }


    inline void unsafe_setCellFree(MPL_Position& sMPLP_p)
    {
        unsafe_setOccupancy(MPL_OccupancyGrid_CellFree, sMPLP_p);

        return;
    }


    inline void unsafe_setCellOccupiedByOwnRobot(MPL_Position& sMPLP_p)
    {
        unsafe_setOccupancy(MPL_OccupancyGrid_CellOccupiedByOwnRobot, sMPLP_p);

        return;
    }


    inline void unsafe_setCellOccupiedByEnemyRobot(MPL_Position& sMPLP_p)
    {

        unsafe_setOccupancy(MPL_OccupancyGrid_CellOccupiedByEnemyRobot, sMPLP_p);

        return;
    }


    inline void unsafe_setCellOccupiedByMovableObstacle(MPL_Position& sMPLP_p)
    {
        unsafe_setOccupancy(MPL_OccupancyGrid_CellOccupiedByMovableObstacle, sMPLP_p);

        return;
    }


    inline void unsafe_setCellOccupiedByFixedObstacle(MPL_Position& sMPLP_p)
    {
        unsafe_setOccupancy(MPL_OccupancyGrid_CellOccupiedByFixedObstacle, sMPLP_p);

        return;
    }

    inline void unsafe_setCellOccupiedByGoal(MPL_Position& sMPLP_p)
    {
        unsafe_setOccupancy(MPL_OccupancyGrid_CellOccupiedByGoal, sMPLP_p);

        return;
    }

    //Friends...
    friend class MPL_QTHandler_HPF;

private:
    /**
     * @brief qvMPLSCGT_occupancyGrid__
     * The grid of state cells itself
     *
     * Currently, only squared cells are allowed.
     *
     *
     * @todo
     * Allow user to put real world values; and this class do the discretization
     */
    QVector<MPL_StateCellGridType> qvMPLSCGT_occupancy3DMatrix__;


    /**
     * @brief d_delta
     */
    double d_delta;

    /**
     * @brief d_sizeX__
     */
    double d_sizeX__;

    /**
     * @brief d_sizeY__
     */
    double d_sizeY__;

    /**
     * @brief d_sizeZ__
     */
    double d_sizeZ__;


    /**
     * @brief n_sizeX__
     */
    int n_sizeX__;


    /**
     * @brief n_sizeY__
     */
    int n_sizeY__;

    /**
     * @brief n_sizeZ__
     */
    int n_sizeZ__;
};

#endif // MPL_GRIDENVIRONMENT_H
