#include "mpl_potentialfieldenvironment.h"


int MPL_PotentialFieldEnvironment::addEnemyRobot(MPL_Position sMPLP_ER)
{

    if( MPL_ReturnCode_OK == MPL_GridEnvironment::addEnemyRobot(sMPLP_ER) ){
        unsafe_setPotentialCellOccupiedByEnemyRobot(sMPLP_ER);
        return(MPL_ReturnCode_OK);
    }

    MPL_ERROR_MESSAGE("Error while adding Enemy Robot"); //HERE!!!!
    return( MPL_PFE_ReturnCode_ErrorWhileAddingObject );

}



int MPL_PotentialFieldEnvironment::addMovableObstacle(MPL_Position sMPLP_MO)
{
    if( MPL_ReturnCode_OK == MPL_GridEnvironment::addMovableObstacle(sMPLP_MO) ){
        unsafe_setPotentialCellOccupiedByMovableObstacle(sMPLP_MO);
        return(MPL_ReturnCode_OK);
    }

    MPL_ERROR_MESSAGE("Error while adding Movable Obstacle");
    return( MPL_PFE_ReturnCode_ErrorWhileAddingObject );
}

int MPL_PotentialFieldEnvironment::addFixedObstacle(MPL_Position sMPLP_FO)
{
    if( MPL_ReturnCode_OK == MPL_GridEnvironment::addFixedObstacle(sMPLP_FO) ){
        unsafe_setPotentialCellOccupiedByFixedObstacle(sMPLP_FO);
        return(MPL_ReturnCode_OK);
    }

    MPL_ERROR_MESSAGE("Error while adding Fixed Obstacle");
    return( MPL_PFE_ReturnCode_ErrorWhileAddingObject );

}

int MPL_PotentialFieldEnvironment::addGoal(MPL_Position sMPLP_G)
{

    if( MPL_ReturnCode_OK == MPL_GridEnvironment::addGoal(sMPLP_G) ){
        unsafe_setPotentialCellOccupiedByGoal(sMPLP_G);
        return(MPL_ReturnCode_OK);
    }

    MPL_ERROR_MESSAGE("Error while adding Goal");
    return( MPL_PFE_ReturnCode_ErrorWhileAddingObject );
}




int MPL_PotentialFieldEnvironment::getPath(MPL_Position &pos_start, MPL_Position *pos_path, int n_maxSteps)
{
    bool b_reachedMinima=false;
    int i, i_aux;
    long int li_maxX, li_maxY, li_maxZ, x, y, z;
    MPL_PFE_VarType MPL_VT_np[6];
    double d_aux;
    double d_vx, d_vx_abs;
    double d_vy, d_vy_abs;
    double d_vz, d_vz_abs;

    if (!isValidPosition(pos_start))
    {
        //Do something if position is not valid
        return(-1);
    }



    li_maxX=getGridSizeX();
    li_maxY=getGridSizeY();
    li_maxZ=getGridSizeZ();



    if( !(li_maxX>1 && li_maxY>1 && li_maxZ>1) ){

    }
    else{
        pos_path[0] = pos_start;
        for( i=1; (i<n_maxSteps) && (!b_reachedMinima); i++)
        {

            x = pos_path[i-1].getX();
            y = pos_path[i-1].getY();
            z = pos_path[i-1].getZ();

            //X neighborhood
            if(0==x)
            {
                 MPL_VT_np[0]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x+1,y,z) ];
                 MPL_VT_np[1]=MPL_PFE_MaxPotential;
            }
            else
            {
                if((li_maxX-1)==x)
                {
                    MPL_VT_np[0]=MPL_PFE_MaxPotential;
                    MPL_VT_np[1]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x-1,y,z) ];
                }
                else
                {
                    MPL_VT_np[0]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x+1,y,z) ];
                    MPL_VT_np[1]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x-1,y,z) ];
                }
            }

            //Y neighborhood
            if(0==y)
            {
                 MPL_VT_np[2]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x,y+1,z) ];
                 MPL_VT_np[3]=MPL_PFE_MaxPotential;
            }
            else
            {
                if((li_maxY-1)==y)
                {
                    MPL_VT_np[2]=MPL_PFE_MaxPotential;
                    MPL_VT_np[3]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x,y-1,z) ];
                }
                else
                {
                    MPL_VT_np[2]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x,y+1,z) ];
                    MPL_VT_np[3]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x,y-1,z) ];
                }
            }


            //Z neighborhood
            if(0==z){
                 MPL_VT_np[4]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x,y,z+1) ];
                 MPL_VT_np[5]=MPL_PFE_MaxPotential;
            }
            else{
                if((li_maxZ-1)==z){
                    MPL_VT_np[4]=MPL_PFE_MaxPotential;
                    MPL_VT_np[5]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x,y,z-1) ];
                }
                else{
                    MPL_VT_np[4]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x,y,z+1) ];
                    MPL_VT_np[5]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x,y,z-1) ];
                }
            }


            d_aux = sqrt(   ((MPL_VT_np[1]-MPL_VT_np[0])*(MPL_VT_np[1]-MPL_VT_np[0])) +\
                            ((MPL_VT_np[3]-MPL_VT_np[2])*(MPL_VT_np[3]-MPL_VT_np[2])) +\
                            ((MPL_VT_np[5]-MPL_VT_np[4])*(MPL_VT_np[5]-MPL_VT_np[4])) );

            if( d_aux>0.00000001 )//10^-8
            {
                //sMPLP_grad.setXYZ((MPL_VT_np[1]-MPL_VT_np[0])/abs_aux, (MPL_VT_np[3]-MPL_VT_np[2])/abs_aux, (MPL_VT_np[5]-MPL_VT_np[4])/abs_aux);
                d_vx = -(MPL_VT_np[1]-MPL_VT_np[0])/d_aux;
                d_vy = -(MPL_VT_np[3]-MPL_VT_np[2])/d_aux;
                d_vz = -(MPL_VT_np[5]-MPL_VT_np[4])/d_aux;

                d_vx_abs = fabs(d_vx);
                d_vy_abs = fabs(d_vy);
                d_vz_abs = fabs(d_vz);
                d_aux = MPL_MAX3(d_vx_abs, d_vy_abs, d_vz_abs);

                d_vx /= d_aux;
                d_vy /= d_aux;
                d_vz /= d_aux;

                if(d_vx >= 0.5)
                {
                    d_vx=1;
                }
                else
                {
                    if(d_vx <= -0.5)
                    {
                        d_vx=-1;
                    }
                    else
                    {
                        d_vx=0;
                    }
                }

                if(d_vy >= 0.5)
                {
                    d_vy=1;
                }
                else
                {
                    if(d_vy <= -0.5)
                    {
                        d_vy=-1;
                    }
                    else
                    {
                        d_vy=0;
                    }
                }


                if(d_vz >= 0.5)
                {
                    d_vz=1;
                }
                else
                {
                    if(d_vz <= -0.5)
                    {
                        d_vz=-1;
                    }
                    else
                    {
                        d_vz=0;
                    }
                }

                pos_path[i].setXYZ( (int)(x + d_vx),\
                                    (int)(y + d_vy),\
                                    (int)(z + d_vz));

                for(i_aux = i-1; (i>=0) && (!b_reachedMinima); i--)
                {
                    if( MPL_Position::manhattanDist(pos_path[i], pos_path[i_aux]) < 0.5 )
                    {
                        b_reachedMinima=true;
                    }
                }

            }
            else
            {
                b_reachedMinima = true; //maybe trapped in a local minima?
            }


        }
    }

    return(MPL_ReturnCode_OK);
}

int MPL_PotentialFieldEnvironment::getPathFromOwnRobot(MPL_Position *pos_path, int n_maxSteps)
{
    MPL_Position p_aux;
    getOwnRobot(p_aux);
    return( getPath(p_aux, pos_path, n_maxSteps) );
}
