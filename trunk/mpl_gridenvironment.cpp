#include "mpl_gridenvironment.h"

MPL_GridEnvironment::MPL_GridEnvironment(int n_sizeX, int n_sizeY, int n_sizeZ, QObject *parent):MPL_BaseEnvironment(parent)
{
    setGridSize(n_sizeX, n_sizeY, n_sizeZ);
}

MPL_GridEnvironment::MPL_GridEnvironment(QObject *parent):MPL_BaseEnvironment(parent)
{
    setGridSize(1, 1, 1);
}


int MPL_GridEnvironment::setGridSize(int n_sizeX, int n_sizeY, int n_sizeZ)
{
    if( (n_sizeX<=0) || (n_sizeY<=0) || (n_sizeZ<=0) ){

        //MPL_ERROR_MESSAGE("Invalid Grid Size (%d,%d,%d)", n_sizeX, n_sizeY, n_sizeZ );
        MPL_ERROR_MSG_START QString("Invalid Grid Size (%1,%2,%3)").arg(n_sizeX).arg(n_sizeY).arg(n_sizeZ) MPL_ERROR_MSG_END

        n_sizeX__ = n_sizeY__ = n_sizeZ__ = 0;
        return(MPL_GE_ReturnCode_InvalidGridSize);
    }

    n_sizeX__ = n_sizeX;
    n_sizeY__ = n_sizeY;
    n_sizeZ__ = n_sizeZ;


    d_delta=1;
    d_sizeX__=d_delta*n_sizeX;
    d_sizeY__=d_delta*n_sizeY;
    d_sizeZ__=d_delta*n_sizeZ;

    qvMPLSCGT_occupancy3DMatrix__.clear();
    qvMPLSCGT_occupancy3DMatrix__.resize(n_sizeX__*n_sizeY__*n_sizeZ__);


    return(MPL_ReturnCode_OK);
}




int MPL_GridEnvironment::addEnemyRobot(MPL_Position sMPLP_ER)
{

    int returnCode=MPL_ReturnCode_OK;

    if( isValidPosition(sMPLP_ER) ){
        if( !unsafe_isOccupied(sMPLP_ER)  ){
            MPL_BaseEnvironment::addEnemyRobot(sMPLP_ER);
            unsafe_setCellOccupiedByEnemyRobot(sMPLP_ER);
        }
        else{
            MPL_GE_AlreadyOccupied(sMPLP_ER);
            returnCode=MPL_GE_ReturnCode_AlreadyOccupied;
        }
    }
    else{
        MPL_GE_InvalidPosition(sMPLP_ER);
        returnCode=MPL_GE_ReturnCode_InvalidPosition;
    }

    return(returnCode);
}


int MPL_GridEnvironment::deleteEnemyRobotAt(int i_ER)
{
    MPL_Position p;


    if( MPL_ReturnCode_OK == MPL_BaseEnvironment::getEnemyRobotAt(p, i_ER) ){
        if( MPL_ReturnCode_OK == MPL_BaseEnvironment::deleteEnemyRobotAt(i_ER) ){
            unsafe_setCellFree(p);
            return(MPL_ReturnCode_OK);
        }
    }

    MPL_GE_ErrorWhileRemoving(p);
    return(MPL_GE_ReturnCode_ErrorWhileRemoving);
}

int MPL_GridEnvironment::clearListOfEnemyRobot()
{
    int i, nEnemyRobot, returnCode;
    MPL_Position p;

    returnCode=MPL_ReturnCode_OK;

    if( MPL_ReturnCode_OK == getSizeListOfEnemyRobot(nEnemyRobot) ){
        for(i=0; i<nEnemyRobot; i++){
            if( MPL_ReturnCode_OK == MPL_BaseEnvironment::getEnemyRobotAt(p, 0) ){
                if( MPL_ReturnCode_OK == MPL_BaseEnvironment::deleteEnemyRobotAt(0) ){
                    unsafe_setCellFree(p);
                }
                else{
                    returnCode=MPL_GE_ReturnCode_ErrorWhileClearing;
                    MPL_GE_ErrorWhileClearing;
                    qWarning("Point 1");
                }
            }
            else{
                returnCode=MPL_GE_ReturnCode_ErrorWhileClearing;
                MPL_GE_ErrorWhileClearing;
                qWarning("Point 2");
            }
        }
    }
    else{
        returnCode=MPL_GE_ReturnCode_ErrorWhileClearing;
        MPL_GE_ErrorWhileClearing;
        qWarning("Point 3");
    }

    return(returnCode);
}



int MPL_GridEnvironment::addMovableObstacle(MPL_Position sMPLP_MO)
{
    int returnCode=MPL_ReturnCode_OK;

    if( isValidPosition(sMPLP_MO) ){
        if( !unsafe_isOccupied(sMPLP_MO)  ){
            MPL_BaseEnvironment::addMovableObstacle(sMPLP_MO);
            unsafe_setCellOccupiedByMovableObstacle(sMPLP_MO);
        }
        else{
            MPL_GE_AlreadyOccupied(sMPLP_MO);
            returnCode=MPL_GE_ReturnCode_AlreadyOccupied;
        }
    }
    else{
        MPL_GE_InvalidPosition(sMPLP_MO);
        returnCode=MPL_GE_ReturnCode_InvalidPosition;
    }

    return(returnCode);
}



int MPL_GridEnvironment::deleteMovableObstacleAt(int i_MO)
{
    MPL_Position p;


    if( MPL_ReturnCode_OK == MPL_BaseEnvironment::getMovableObstacleAt(p, i_MO) ){
        if( MPL_ReturnCode_OK == MPL_BaseEnvironment::deleteMovableObstacleAt(i_MO) ){
            unsafe_setCellFree(p);
            return(MPL_ReturnCode_OK);
        }
    }

    MPL_GE_ErrorWhileRemoving(p);
    return(MPL_GE_ReturnCode_ErrorWhileRemoving);
}

int MPL_GridEnvironment::clearListOfMovableObstacle()
{
    int i, nMovableObstacle, returnCode;
    MPL_Position p;

    returnCode=MPL_ReturnCode_OK;

    if( MPL_ReturnCode_OK == getSizeListOfMovableObstacle(nMovableObstacle) ){
        for(i=0; i<nMovableObstacle; i++){
            if( MPL_ReturnCode_OK == MPL_BaseEnvironment::getMovableObstacleAt(p, 0) ){
                if( MPL_ReturnCode_OK == MPL_BaseEnvironment::deleteMovableObstacleAt(0) ){
                    unsafe_setCellFree(p);
                }
                else{
                    returnCode=MPL_GE_ReturnCode_ErrorWhileClearing;
                    MPL_GE_ErrorWhileClearing;
                }
            }
            else{
                returnCode=MPL_GE_ReturnCode_ErrorWhileClearing;
                MPL_GE_ErrorWhileClearing;
            }
        }
    }
    else{
        returnCode=MPL_GE_ReturnCode_ErrorWhileClearing;
        MPL_GE_ErrorWhileClearing;
    }

    return(returnCode);
}



int MPL_GridEnvironment::addFixedObstacle(MPL_Position sMPLP_FO)
{
    int returnCode=MPL_ReturnCode_OK;

    if( isValidPosition(sMPLP_FO) ){
        if( !unsafe_isOccupied(sMPLP_FO)  ){
            MPL_BaseEnvironment::addFixedObstacle(sMPLP_FO);
            unsafe_setCellOccupiedByFixedObstacle(sMPLP_FO);
        }
        else{
            MPL_GE_AlreadyOccupied(sMPLP_FO);
            returnCode=MPL_GE_ReturnCode_AlreadyOccupied;
        }
    }
    else{
        MPL_GE_InvalidPosition(sMPLP_FO);
        returnCode=MPL_GE_ReturnCode_InvalidPosition;
    }

    return(returnCode);
}



int MPL_GridEnvironment::deleteFixedObstacleAt(int i_FO)
{
    MPL_Position p;


    if( MPL_ReturnCode_OK == MPL_BaseEnvironment::getFixedObstacleAt(p, i_FO) ){
        if( MPL_ReturnCode_OK == MPL_BaseEnvironment::deleteFixedObstacleAt(i_FO) ){
            unsafe_setCellFree(p);
            return(MPL_ReturnCode_OK);
        }
    }

    MPL_GE_ErrorWhileRemoving(p);
    return(MPL_GE_ReturnCode_ErrorWhileRemoving);
}

int MPL_GridEnvironment::clearListOfFixedObstacle()
{
    int i, nFixedObstacle, returnCode;
    MPL_Position p;

    returnCode=MPL_ReturnCode_OK;

    if( MPL_ReturnCode_OK == getSizeListOfFixedObstacle(nFixedObstacle) ){
        for(i=0; i<nFixedObstacle; i++){
            if( MPL_ReturnCode_OK == MPL_BaseEnvironment::getFixedObstacleAt(p, 0) ){
                if( MPL_ReturnCode_OK == MPL_BaseEnvironment::deleteFixedObstacleAt(0) ){
                    unsafe_setCellFree(p);
                }
                else{
                    returnCode=MPL_GE_ReturnCode_ErrorWhileClearing;
                    MPL_GE_ErrorWhileClearing;
                }
            }
            else{
                returnCode=MPL_GE_ReturnCode_ErrorWhileClearing;
                MPL_GE_ErrorWhileClearing;
            }
        }
    }
    else{
        returnCode=MPL_GE_ReturnCode_ErrorWhileClearing;
        MPL_GE_ErrorWhileClearing;
    }

    return(returnCode);
}


int MPL_GridEnvironment::addGoal(MPL_Position sMPLP_G)
{
    int returnCode=MPL_ReturnCode_OK;

    if( isValidPosition(sMPLP_G) ){
        if( !unsafe_isOccupied(sMPLP_G)  ){
            MPL_BaseEnvironment::addGoal(sMPLP_G);
            unsafe_setCellOccupiedByGoal(sMPLP_G);
        }
        else{
            MPL_GE_AlreadyOccupied(sMPLP_G);
            returnCode=MPL_GE_ReturnCode_AlreadyOccupied;
        }
    }
    else{
        MPL_GE_InvalidPosition(sMPLP_G);
        returnCode=MPL_GE_ReturnCode_InvalidPosition;
    }

    return(returnCode);
}



int MPL_GridEnvironment::deleteGoalAt(int i_G)
{
    MPL_Position p;


    if( MPL_ReturnCode_OK == MPL_BaseEnvironment::getGoalAt(p, i_G) ){
        if( MPL_ReturnCode_OK == MPL_BaseEnvironment::deleteGoalAt(i_G) ){
            unsafe_setCellFree(p);
            return(MPL_ReturnCode_OK);
        }
    }

    MPL_GE_ErrorWhileRemoving(p);
    return(MPL_GE_ReturnCode_ErrorWhileRemoving);
}

int MPL_GridEnvironment::clearListOfGoal()
{
    int i, nGoal, returnCode;
    MPL_Position p;

    returnCode=MPL_ReturnCode_OK;

    if( MPL_ReturnCode_OK == getSizeListOfGoal(nGoal) ){
        for(i=0; i<nGoal; i++){
            if( MPL_ReturnCode_OK == MPL_BaseEnvironment::getGoalAt(p, 0) ){
                if( MPL_ReturnCode_OK == MPL_BaseEnvironment::deleteGoalAt(0) ){
                    unsafe_setCellFree(p);
                }
                else{
                    returnCode=MPL_GE_ReturnCode_ErrorWhileClearing;
                    MPL_GE_ErrorWhileClearing;
                }
            }
            else{
                returnCode=MPL_GE_ReturnCode_ErrorWhileClearing;
                MPL_GE_ErrorWhileClearing;
            }
        }
    }
    else{
        returnCode=MPL_GE_ReturnCode_ErrorWhileClearing;
        MPL_GE_ErrorWhileClearing;
    }

    return(returnCode);
}
