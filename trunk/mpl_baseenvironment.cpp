#include "mpl_baseenvironment.h"

MPL_BaseEnvironment::MPL_BaseEnvironment(QObject *parent):QObject(parent)
{
    b_verbose__=false;
}


//enemyrobot
int MPL_BaseEnvironment::getSizeListOfEnemyRobot(int& r_n_ER)
{
    r_n_ER = qlMPLP_enemyRobot__.size();

    return(MPL_ReturnCode_OK);
}

int MPL_BaseEnvironment::addEnemyRobot(MPL_Position sMPLP_ER)
{
    qlMPLP_enemyRobot__.append(sMPLP_ER);

    return(MPL_ReturnCode_OK);
}

int MPL_BaseEnvironment::getEnemyRobotAt(MPL_Position& r_sMPLP_ER, int i_ER)
{
    if( (i_ER<0) || (i_ER>=qlMPLP_enemyRobot__.size()) )
    {
        MPL_ERROR_MSG_START QString("[invalid value] %1 not in [0,%2)").arg(i_ER).arg(qlMPLP_enemyRobot__.size() ) MPL_ERROR_MSG_END

        r_sMPLP_ER.setXYZ(-1,-1,-1);

        return(MPL_BE_ReturnCode_InvalidValue);
    }


    r_sMPLP_ER = qlMPLP_enemyRobot__.at(i_ER);

    return(MPL_ReturnCode_OK);
}

int MPL_BaseEnvironment::deleteEnemyRobotAt(int i_ER)
{
    if( (i_ER<0) || (i_ER>=qlMPLP_enemyRobot__.size()) )
    {
        //MPL_ERROR_MESSAGE("[invalid value] %d not in [0,%d) ", i_ER, qlMPLP_enemyRobot__.size());
        MPL_ERROR_MSG_START QString("[invalid value] %1 not in [0,%2)").arg(i_ER).arg(qlMPLP_enemyRobot__.size() ) MPL_ERROR_MSG_END

        return(MPL_BE_ReturnCode_InvalidValue);
    }
    qlMPLP_enemyRobot__.removeAt(i_ER);

    return(MPL_ReturnCode_OK);
}

int MPL_BaseEnvironment::clearListOfEnemyRobot()
{
    qlMPLP_enemyRobot__.clear();

    return(MPL_ReturnCode_OK);
}


int MPL_BaseEnvironment::getSizeListOfMovableObstacle(int& r_n_MO)
{
    r_n_MO = qlMPLP_movableObstacle__.size();

    return(MPL_ReturnCode_OK);
}

int MPL_BaseEnvironment::addMovableObstacle(MPL_Position sMPLP_MO)
{
    qlMPLP_movableObstacle__.append(sMPLP_MO);

    return(MPL_ReturnCode_OK);
}

int MPL_BaseEnvironment::getMovableObstacleAt(MPL_Position& r_sMPLP_MO, int i_MO)
{
    if( (i_MO<0) || (i_MO>=qlMPLP_movableObstacle__.size()) )
    {
        //MPL_ERROR_MESSAGE("[invalid value] %d not in [0,%d) ", i_MO, qlMPLP_movableObstacle__.size());
        MPL_ERROR_MSG_START QString("[invalid value] %1 not in [0,%2)").arg(i_MO).arg(qlMPLP_movableObstacle__.size() ) MPL_ERROR_MSG_END

        r_sMPLP_MO.setXYZ(-1,-1,-1);

        return(MPL_BE_ReturnCode_InvalidValue);
    }

    r_sMPLP_MO = qlMPLP_movableObstacle__.at(i_MO);

    return(MPL_ReturnCode_OK);
}

int MPL_BaseEnvironment::deleteMovableObstacleAt(int i_MO)
{
    if( (i_MO<0) || (i_MO>=qlMPLP_movableObstacle__.size()) )
    {
        MPL_ERROR_MSG_START QString("[invalid value] %1 not in [0,%2)").arg(i_MO).arg(qlMPLP_movableObstacle__.size() ) MPL_ERROR_MSG_END

        return(MPL_BE_ReturnCode_InvalidValue);
    }


    qlMPLP_movableObstacle__.removeAt(i_MO);

    return(MPL_ReturnCode_OK);
}

int MPL_BaseEnvironment::clearListOfMovableObstacle()
{
    qlMPLP_movableObstacle__.clear();
    return(MPL_ReturnCode_OK);
}


int MPL_BaseEnvironment::getSizeListOfFixedObstacle(int& r_n_FO)
{
    r_n_FO = qlMPLP_fixedObstacle__.size();

    return(MPL_ReturnCode_OK);
}

int MPL_BaseEnvironment::addFixedObstacle(MPL_Position sMPLP_FO)
{
    qlMPLP_fixedObstacle__.append(sMPLP_FO);

    return(MPL_ReturnCode_OK);
}

int MPL_BaseEnvironment::getFixedObstacleAt(MPL_Position& r_sMPLP_FO, int i_FO)
{
    if( (i_FO<0) || (i_FO>=qlMPLP_fixedObstacle__.size()) )
    {
        //MPL_ERROR_MESSAGE("[invalid value] %d not in [0,%d) ", i_FO, qlMPLP_fixedObstacle__.size());
        MPL_ERROR_MSG_START QString("[invalid value] %1 not in [0,%2)").arg(i_FO).arg(qlMPLP_fixedObstacle__.size() ) MPL_ERROR_MSG_END

        r_sMPLP_FO.setXYZ(-1,-1,-1);

        return(MPL_BE_ReturnCode_InvalidValue);
    }

    r_sMPLP_FO = qlMPLP_fixedObstacle__.at(i_FO);

    return(MPL_ReturnCode_OK);
}

int MPL_BaseEnvironment::deleteFixedObstacleAt(int i_FO)
{
    if( (i_FO<0) || (i_FO>=qlMPLP_fixedObstacle__.size()) )
    {

        MPL_ERROR_MSG_START QString("[invalid value] %1 not in [0,%2)").arg(i_FO).arg(qlMPLP_fixedObstacle__.size() ) MPL_ERROR_MSG_END

        return(MPL_BE_ReturnCode_InvalidValue);
    }


    qlMPLP_fixedObstacle__.removeAt(i_FO);

    return(MPL_ReturnCode_OK);
}

int MPL_BaseEnvironment::clearListOfFixedObstacle()
{
    qlMPLP_fixedObstacle__.clear();

    return(MPL_ReturnCode_OK);
}


int MPL_BaseEnvironment::getSizeListOfGoal(int& r_n_G)
{
    r_n_G = qlMPLP_goal__.size();

    return(MPL_ReturnCode_OK);
}

int MPL_BaseEnvironment::addGoal(MPL_Position sMPLP_G)
{
    qlMPLP_goal__.append(sMPLP_G);

    return(MPL_ReturnCode_OK);
}

int MPL_BaseEnvironment::getGoalAt(MPL_Position& r_sMPLP_G, int i_G)
{
    if( (i_G<0) || (i_G>=qlMPLP_goal__.size()) )
    {
        //MPL_ERROR_MESSAGE("[invalid value] %d not in [0,%d) ", i_G, qlMPLP_goal__.size());
        MPL_ERROR_MSG_START QString("[invalid value] %1 not in [0,%2)").arg(i_G).arg(qlMPLP_goal__.size() ) MPL_ERROR_MSG_END

        r_sMPLP_G.setXYZ(-1,-1,-1);

        return(MPL_BE_ReturnCode_InvalidValue);
    }

    r_sMPLP_G = qlMPLP_goal__.at(i_G);

    return(MPL_ReturnCode_OK);
}

int MPL_BaseEnvironment::deleteGoalAt(int i_G)
{
    if( (i_G<0) || (i_G>=qlMPLP_goal__.size()) )
    {
        MPL_ERROR_MSG_START QString("[invalid value] %1 not in [0,%2)").arg(i_G).arg(qlMPLP_goal__.size() ) MPL_ERROR_MSG_END

        return(MPL_BE_ReturnCode_InvalidValue);
    }
    qlMPLP_goal__.removeAt(i_G);

    return(MPL_ReturnCode_OK);
}

int MPL_BaseEnvironment::clearListOfGoal()
{
    qlMPLP_goal__.clear();

    return(MPL_ReturnCode_OK);
}

