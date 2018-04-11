#ifndef MPL_BASEENVIRONMENT_H
#define MPL_BASEENVIRONMENT_H

#include <cstdio>
#include <QObject>
#include <QString>
#include <QList>
#include "mpl_position.h"
#include "mpl_utils.h"


#define MPL_BE_ReturnCode_InvalidValue 1

#define MPL_ERROR_MSG_START emit signal_errorMessage(QString("\n----\n[ERROR] ") +
#define MPL_ERROR_MSG_END + QString("\n  File: ") + __FILE__ + ":" + S__LINE__ + "\n  Func: " + __PRETTY_FUNCTION__ + "\n----\n" );

#define MPL_LOG_MSG_START emit signal_logMessage(QString("") +
#define MPL_LOG_MSG_END );


/**
 * @brief The MPL_BaseEnvironment class
 * This class in responsible for maintaining data from the environment
 *
 */
class MPL_BaseEnvironment : public QObject
{

    Q_OBJECT

public:
    MPL_BaseEnvironment(QObject *parent=0);



    //ownrobot
    /**
     * @brief addOwnRobot
     * Sets own robot position
     *
     * @param[in] sMPLP_OR
     * Position of the robot
     *
     * @return error code
     */
    inline int setOwnRobot(MPL_Position sMPLP_OR)
    {
        MPLP_ownRobot__ = sMPLP_OR;

        return(0);
    }

    /**
     * @brief getOwnRobotAt
     * Returns position own robot
     *
     * @param[out] r_sMPLP_OR
     * Position of the robot
     *
     * @return error code
     */
    inline int getOwnRobot(MPL_Position& r_sMPLP_OR)
    {
        r_sMPLP_OR = MPLP_ownRobot__;

        return(0);
    }



    /**
     * @brief getSizeListOfEnemyRobot
     * This method returns current size of list of enemy robots
     *
     * @param[out] r_n_ER
     *
     * @return error code
     */
    int getSizeListOfEnemyRobot(int& r_n_ER);

    /**
     * @brief addEnemyRobot
     * Adds a robot to enemy robots list
     *
     * @param[in] sMPLP_ER
     *
     * @return error code
     */
    int addEnemyRobot(MPL_Position sMPLP_ER);

    /**
     * @brief getEnemyRobotAt
     * Returns position (x,y) of a enemy robot in a specific index inside list
     *
     * @param[out] r_sMPLP_ER
     * Position of enemy robot
     *
     * @param[in] i_ER
     * Index at list
     *
     * @return error code
     */
    int getEnemyRobotAt(MPL_Position& r_sMPLP_ER, int i_ER);

    /**
     * @brief deleteEnemyRobotAt
     * Remove a robot from a specific index
     *
     * @param[in] i_ER
     * Index at list
     *
     * @return error code
     */
    int deleteEnemyRobotAt(int i_ER);

    /**
     * @brief clearListOfEnemyRobot
     * This method just remove all enemy robots from list, without resizing it
     *
     * @note
     * From Qt Docs: "Note that the internal array only ever gets bigger over the life of the list. It never shrinks."
     *
     * @return error code
     */
    int clearListOfEnemyRobot();


    /**
     * @brief getSizeListOfMovableObstacle
     * This method returns current size of list of movable obstacles
     *
     * @param[out] r_n_MO
     *
     * @return error code
     */
    int getSizeListOfMovableObstacle(int& r_n_MO);

    /**
     * @brief addMovableObstacle
     * Adds a obstacle to movable obstacle list
     *
     * @param[in] sMPLP_MO
     *
     * @return error code
     */
    int addMovableObstacle(MPL_Position sMPLP_MO);

    /**
     * @brief getMovableObstacleAt
     * Returns position (x,y) of a movable obstacle in a specific index inside list
     *
     * @param[out] r_sMPLP_MO
     * Position of movable obstacle
     *
     * @param[in] i_MO
     * Index at list
     *
     * @return error code
     */
    int getMovableObstacleAt(MPL_Position& r_sMPLP_MO, int i_MO);

    /**
     * @brief deleteMovableObstacleAt
     * Remove a movable obstacle from a specific index
     *
     * @param[in] i_MO
     * Index at list
     *
     * @return error code
     */
    int deleteMovableObstacleAt(int i_MO);

    /**
     * @brief clearListOfMovableObstacle
     * This method just remove all movable obstacles from list, without resizing it
     *
     * @note
     * From Qt Docs: "Note that the internal array only ever gets bigger over the life of the list. It never shrinks."
     *
     * @return error code
     */
    int clearListOfMovableObstacle();


    /**
     * @brief getSizeListOfFixedObstacle
     * This method returns current size of list of fixed obstacles
     *
     * @param[out] r_n_FO
     *
     * @return error code
     */
    int getSizeListOfFixedObstacle(int& r_n_FO);

    /**
     * @brief addFixedObstacle
     * Adds a obstacle to fixed obstacle list
     *
     * @param[in] sMPLP_FO
     *
     * @return error code
     */
    int addFixedObstacle(MPL_Position sMPLP_FO);

    /**
     * @brief getFixedObstacleAt
     * Returns position (x,y) of a fixed obstacle in a specific index inside list
     *
     * @param[out] r_sMPLP_FO
     * Position of movable obstacle
     *
     * @param[in] i_FO
     * Index at list
     *
     * @return error code
     */
    int getFixedObstacleAt(MPL_Position& r_sMPLP_FO, int i_FO);

    /**
     * @brief deleteFixedObstacleAt
     * Remove a fixed obstacle from a specific index
     *
     * @param[in] i_FO
     * Index at list
     *
     * @return error code
     */
    int deleteFixedObstacleAt(int i_FO);

    /**
     * @brief clearListOfFixedObstacle
     * This method just remove all fixed obstacles from list, without resizing it
     *
     * @note
     * From Qt Docs: "Note that the internal array only ever gets bigger over the life of the list. It never shrinks."
     *
     * @return error code
     */
    int clearListOfFixedObstacle();


    /**
     * @brief getSizeListOfGoal
     * This method returns current size of list of goals
     *
     * @param[out] r_n_G
     *
     * @return error code
     */
    int getSizeListOfGoal(int& r_n_G);

    /**
     * @brief addGoal
     * Adds a goal to goals list
     *
     * @param[in] sMPLP_G
     *
     * @return error code
     */
    int addGoal(MPL_Position sMPLP_G);

    /**
     * @brief getGoalAt
     * Returns position (x,y) of a goal in a specific index inside list
     *
     * @param[out] r_sMPLP_G
     *
     * @param[in] i_G
     *
     * @return error code
     */
    int getGoalAt(MPL_Position& r_sMPLP_G, int i_G);

    /**
     * @brief deleteGoalAt
     * Remove a goal from a specific index
     *
     * @param[in] i_G
     * Index at list
     *
     * @return error code
     */
    int deleteGoalAt(int i_G);

    /**
     * @brief clearListOfGoal
     * This method just remove all goals from list, without resizing it
     *
     * @note
     * From Qt Docs: "Note that the internal array only ever gets bigger over the life of the list. It never shrinks."
     *
     * @return error code
     */
    int clearListOfGoal();

    inline void setVerbose(bool b_verbose=true){
        b_verbose__=b_verbose;
    }

    inline void setSilent(void)
    {
        b_verbose__=false;
    }

    inline void toggleVerbose(void)
    {
        b_verbose__ = !b_verbose__;
    }


public:
signals:
    void signal_logMessage( QString s );
    void signal_errorMessage( QString s );

public:
    bool b_verbose__;

private:
    MPL_Position MPLP_ownRobot__;
    QList<MPL_Position> qlMPLP_enemyRobot__;
    QList<MPL_Position> qlMPLP_movableObstacle__;
    QList<MPL_Position> qlMPLP_fixedObstacle__;
    QList<MPL_Position> qlMPLP_goal__;


};

#endif // MPL_BASEENVIRONMENT_H
