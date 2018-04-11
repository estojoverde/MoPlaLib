#ifndef MPL_POTENTIALFIELDENVIRONMENT_H
#define MPL_POTENTIALFIELDENVIRONMENT_H

#include <limits.h>
#include <math.h>

#include "mpl_gridenvironment.h"

#define MPL_PFE_DOUBLE

#if defined(MPL_PFE_FLOAT)
    #define MPL_PFE_VarType float
    #define MPL_PFE_MaxPotential 1.0f
    #define MPL_PFE_MinPotential 0.0f
    #define MPL_PFE_ResetPotential 0.9f
    #define MPL_PFE_VarType_CharCode "%2.05f"
#elif defined(MPL_PFE_DOUBLE)
    #define MPL_PFE_VarType double
    #define MPL_PFE_MaxPotential 1.0
    #define MPL_PFE_MinPotential 0.0
    #define MPL_PFE_ResetPotential 0.9
    #define MPL_PFE_VarType_CharCode "%2.08lf"
#elif defined(MPL_PFE_UINT)
    #define MPL_PFE_VarType unsigned int
    #define MPL_PFE_MaxPotential (UINT_MAX>>3) //allows summing up to 8 max potentials without overflow. In 16bit has 8192 possible potentials (enough, right?)
    #define MPL_PFE_MinPotential 0.0
    #define MPL_PFE_ResetPotential ((UINT_MAX>>4))
    #define MPL_PFE_VarType_CharCode "%2u"
#else
    #error Var Type Required
#endif



#define MPL_PFE_ReturnCode_InvalidGridSize 2
#define MPL_PFE_ReturnCode_ErrorWhileAddingObject 3
#define MPL_PFE_ReturnCode_InvalidPosition 4

class MPL_PotentialFieldEnvironment : public MPL_GridEnvironment
{
    Q_OBJECT


    friend class MPL_OPF_ParallelSor_MainLaucher;

public:
    MPL_PotentialFieldEnvironment(int n_sizeX=1, int n_sizeY=1, int n_sizeZ=1, QObject *parent=0):MPL_GridEnvironment(parent)
    {
        if( MPL_ReturnCode_OK != MPL_PotentialFieldEnvironment::setGridSize(n_sizeX, n_sizeY, n_sizeZ) ){
            MPL_ERROR_MSG_START QString("Could not set Potential Grid Size while creating it") MPL_ERROR_MSG_END
        }
    }

    ~MPL_PotentialFieldEnvironment()
    {
        qvMPL_PFE_VarType_potential3DMatrix__.clear();
    }


    /**
     * @brief addEnemyRobot
     * @param sMPLP_ER
     */
    int addEnemyRobot(MPL_Position sMPLP_ER);

    /**
     * @brief addMovableObstacle
     * @param sMPLP_MO
     */
    int addMovableObstacle(MPL_Position sMPLP_MO);

    /**
     * @brief addFixedObstacle
     * @param sMPLP_FO
     */
    int addFixedObstacle(MPL_Position sMPLP_FO);

    /**
     * @brief addGoal
     * @param sMPLP_G
     */
    int addGoal(MPL_Position sMPLP_G);



    int getPath(MPL_Position &pos_start, MPL_Position *pos_path, int n_maxSteps=300);

    int getPathFromOwnRobot(MPL_Position *pos_path, int n_maxSteps=300);

    /**
     * @brief setGridSize
     * @param n_sizeX__
     * @param n_sizeY__
     * @param n_sizeZ__
     * @TODO if actual size is equal to n_sizeX*n_sizeY*n_sizeZ, no need to resize... or do need it?
     */
    inline int setGridSize(int n_sizeX, int n_sizeY, int n_sizeZ)
    {

        if( MPL_ReturnCode_OK == MPL_GridEnvironment::setGridSize(n_sizeX, n_sizeY, n_sizeZ) ){
            qvMPL_PFE_VarType_potential3DMatrix__.clear();
            qvMPL_PFE_VarType_potential3DMatrix__.resize(n_sizeX*n_sizeY*n_sizeZ);
            return(MPL_ReturnCode_OK);
        }
        else{
            qvMPL_PFE_VarType_potential3DMatrix__.clear();
            MPL_ERROR_MSG_START QString("Could not set Potential Grid Size") MPL_ERROR_MSG_END
            return(MPL_PFE_ReturnCode_InvalidGridSize);
        }

    }

    inline int setAllPotentials(MPL_PFE_VarType MPL_VT_newValue){

        long int i, iMax;

        iMax=getGridSizeX()*getGridSizeY()*getGridSizeZ();

        for( i=0; i<iMax; i++){
            qvMPL_PFE_VarType_potential3DMatrix__[i] = MPL_VT_newValue;
        }

        return(MPL_ReturnCode_OK);

    }


    inline void unsafe_getNeighborPotential(MPL_Position &sMPLP_p, MPL_PFE_VarType MPL_VT_np[])
    {
        long int x, y, z;

        x = (long int) floor(sMPLP_p.getX() +0.5);
        y = (long int) floor(sMPLP_p.getY() +0.5);
        z = (long int) floor(sMPLP_p.getZ() +0.5);

        MPL_VT_np[0]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x+1,y,z) ];
        MPL_VT_np[1]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x-1,y,z) ];
        MPL_VT_np[2]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x,y+1,z) ];
        MPL_VT_np[3]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x,y-1,z) ];
        MPL_VT_np[4]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x,y,z+1) ];
        MPL_VT_np[5]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x,y,z-1) ];


    }

    inline void unsafe_getNeighborPotential(long int x, long int y, long int z, MPL_PFE_VarType MPL_VT_np[])
    {
        MPL_VT_np[0]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x+1,y,z) ];
        MPL_VT_np[1]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x-1,y,z) ];
        MPL_VT_np[2]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x,y+1,z) ];
        MPL_VT_np[3]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x,y-1,z) ];
        MPL_VT_np[4]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x,y,z+1) ];
        MPL_VT_np[5]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x,y,z-1) ];

    }

    inline int getNeighborPotential(long int x, long int y, long int z, MPL_PFE_VarType MPL_VT_np[])
    {
        long int li_maxX, li_maxY, li_maxZ;

        li_maxX=getGridSizeX();
        li_maxY=getGridSizeY();
        li_maxZ=getGridSizeZ();


        if(! ((x>=0)&&(x<li_maxX) && (y>=0)&&(y<li_maxY) && (z>=0)&&(z<li_maxZ)) ){
            MPL_ERROR_MESSAGE("Invalid position");
            return(MPL_PFE_ReturnCode_InvalidPosition);
        }

        //BUG: When dimension is 1, it falls outside matrix
        //X
        if(li_maxX>1){
            if(0==x){
                 MPL_VT_np[0]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x+1,y,z) ];
                 MPL_VT_np[1]=MPL_PFE_MaxPotential;
            }
            else{
                if((li_maxX-1)==x){
                    MPL_VT_np[0]=MPL_PFE_MaxPotential;
                    MPL_VT_np[1]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x-1,y,z) ];
                }
                else{
                    MPL_VT_np[0]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x+1,y,z) ];
                    MPL_VT_np[1]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x-1,y,z) ];
                }
            }
        }
        else{
            MPL_VT_np[0]=MPL_PFE_MaxPotential;
            MPL_VT_np[1]=MPL_PFE_MaxPotential;
        }

        //Y
        if(li_maxY>1){
            if(0==y){
                 MPL_VT_np[2]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x,y+1,z) ];
                 MPL_VT_np[3]=MPL_PFE_MaxPotential;
            }
            else{
                if((li_maxY-1)==y){
                    MPL_VT_np[2]=MPL_PFE_MaxPotential;
                    MPL_VT_np[3]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x,y-1,z) ];
                }
                else{
                    MPL_VT_np[2]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x,y+1,z) ];
                    MPL_VT_np[3]=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x,y-1,z) ];
                }
            }
        }
        else{
            MPL_VT_np[2]=MPL_PFE_MaxPotential;
            MPL_VT_np[3]=MPL_PFE_MaxPotential;
        }

        //Z
        if(li_maxZ>1){
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
        }
        else{
            MPL_VT_np[4]=MPL_PFE_MaxPotential;
            MPL_VT_np[5]=MPL_PFE_MaxPotential;
        }

        return(MPL_ReturnCode_OK);

    }


    inline void unsafe_getPotential(long int x, long int y, long int z, MPL_PFE_VarType& MPL_VT_np)
    {

        MPL_VT_np=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x,y,z) ];

    }

    inline void unsafe_getPotential(MPL_Position &sMPLP_p, MPL_PFE_VarType& MPL_VT_np)
    {
        long int x, y, z;


        x = (long int) floor(sMPLP_p.getX() +0.5);
        y = (long int) floor(sMPLP_p.getY() +0.5);
        z = (long int) floor(sMPLP_p.getZ() +0.5);

        MPL_VT_np=qvMPL_PFE_VarType_potential3DMatrix__[ unsafe_arrayIndex(x,y,z) ];

    }


    inline void unsafe_setPotential(MPL_PFE_VarType MPL_VT_newValue, MPL_Position& sMPLP_p)
    {
        qvMPL_PFE_VarType_potential3DMatrix__[unsafe_arrayIndex(sMPLP_p)] = MPL_VT_newValue;

        return;
    }

    inline void unsafe_setPotential(MPL_PFE_VarType MPL_VT_newValue, long int x, long int y, long int z)
    {

        qvMPL_PFE_VarType_potential3DMatrix__[unsafe_arrayIndex(x,y,z)] = MPL_VT_newValue;

        return;
    }


    inline void getGradient(long int x, long int y, long int z, double &vx, double &vy, double &vz)
    {
        MPL_PFE_VarType MPL_VT_np[6];
        double abs_aux;

        getNeighborPotential(x, y, z, MPL_VT_np);

        abs_aux = sqrt( ((MPL_VT_np[1]-MPL_VT_np[0])*(MPL_VT_np[1]-MPL_VT_np[0])) +\
                        ((MPL_VT_np[3]-MPL_VT_np[2])*(MPL_VT_np[3]-MPL_VT_np[2])) +\
                        ((MPL_VT_np[5]-MPL_VT_np[4])*(MPL_VT_np[5]-MPL_VT_np[4])) );


        if( abs_aux>MPL_Position_DELTA )
        {
            //sMPLP_grad.setXYZ((MPL_VT_np[1]-MPL_VT_np[0])/abs_aux, (MPL_VT_np[3]-MPL_VT_np[2])/abs_aux, (MPL_VT_np[5]-MPL_VT_np[4])/abs_aux);
            vx = (MPL_VT_np[1]-MPL_VT_np[0])/abs_aux;
            vy = (MPL_VT_np[3]-MPL_VT_np[2])/abs_aux;
            vz = (MPL_VT_np[5]-MPL_VT_np[4])/abs_aux;
        }
        else
        {
            vx = 0;
            vy = 0;
            vz = 0;
        }

    }

    inline void getGradientDescent(long int x, long int y, long int z, double &vx, double &vy, double &vz)
    {
        getGradient(x, y, z, vx, vy, vz);
        vx = -vx;
        vy = -vy;
        vz = -vz;
        return;
    }



    inline void unsafe_getGradient(MPL_Position &sMPLP_p, MPL_Position &sMPLP_grad)
    {
        MPL_PFE_VarType MPL_VT_np[6];
        double abs_aux;

        unsafe_getNeighborPotential(sMPLP_p, MPL_VT_np);

        abs_aux = sqrt( ((MPL_VT_np[1]-MPL_VT_np[0])*(MPL_VT_np[1]-MPL_VT_np[0])) +\
                        ((MPL_VT_np[3]-MPL_VT_np[2])*(MPL_VT_np[3]-MPL_VT_np[2])) +\
                        ((MPL_VT_np[5]-MPL_VT_np[4])*(MPL_VT_np[5]-MPL_VT_np[4])) );


        if( fabs(abs_aux)>MPL_Position_DELTA )
            sMPLP_grad.setXYZ((MPL_VT_np[1]-MPL_VT_np[0])/abs_aux, (MPL_VT_np[3]-MPL_VT_np[2])/abs_aux, (MPL_VT_np[5]-MPL_VT_np[4])/abs_aux);
        else
            sMPLP_grad.setXYZ(0,0,0);

    }

    /**
     * @brief unsafe_getGradientCube
     * Get gradient inside a 2-cube
     *
     * @param sMPLP_p
     * Position where gradient will be evaluated
     *
     * @param sMPLP_grad
     * Gradient out
     *
     * @todo There is a bug. When robot, obstacle and goal are aligned, resultant gradient can point towards an obstacle
     */
    inline void unsafe_getGradientCube(MPL_Position &sMPLP_p, MPL_Position &sMPLP_grad)
    {
        MPL_PFE_VarType MPL_VT_np[10];
        double abs_aux;

        unsafe_getNeighborPotential(sMPLP_p, MPL_VT_np);

        MPL_VT_np[6]=MPL_VT_np[1]-MPL_VT_np[0];
        MPL_VT_np[7]=MPL_VT_np[3]-MPL_VT_np[2];
        MPL_VT_np[8]=MPL_VT_np[5]-MPL_VT_np[4];

        abs_aux = sqrt( (MPL_VT_np[6]*MPL_VT_np[6]) +\
                        (MPL_VT_np[7]*MPL_VT_np[7]) +\
                        (MPL_VT_np[8]*MPL_VT_np[8]) );


        if( abs(abs_aux)>MPL_Position_DELTA ){
            MPL_VT_np[9]=MPL_MAX3(MPL_VT_np[6], MPL_VT_np[7], MPL_VT_np[8]);

            sMPLP_grad.setXYZ(MPL_VT_np[6]/MPL_VT_np[9], MPL_VT_np[7]/MPL_VT_np[9], MPL_VT_np[8]/MPL_VT_np[9]);
        }
        else
            sMPLP_grad.setXYZ(0,0,0);

    }

    inline void unsafe_getGradientDescent(MPL_Position &sMPLP_p, MPL_Position &sMPLP_grad)
    {
        unsafe_getGradient(sMPLP_p, sMPLP_grad);

        sMPLP_grad.inverse();

    }

    inline void unsafe_nextCellFrom(MPL_Position &sMPLP_p, MPL_Position &sMPLP_next)
    {
        MPL_Position sMPLP_decGrad;

        unsafe_getGradientCube(sMPLP_p, sMPLP_decGrad);

        MPL_PFE_VarType MPL_VT_aux[3];

        MPL_VT_aux[0]=sMPLP_decGrad.getX();
        MPL_VT_aux[0]=(MPL_VT_aux[0]>0.5? 1: (MPL_VT_aux[0]<-0.5?-1:0)) + sMPLP_p.getX();

        MPL_VT_aux[1]=sMPLP_decGrad.getY();
        MPL_VT_aux[1]=(MPL_VT_aux[1]>0.5? 1: (MPL_VT_aux[1]<-0.5?-1:0)) + sMPLP_p.getY();

        MPL_VT_aux[2]=sMPLP_decGrad.getZ();
        MPL_VT_aux[2]=(MPL_VT_aux[2]>0.5? 1: (MPL_VT_aux[2]<-0.5?-1:0)) + sMPLP_p.getZ();

        sMPLP_next.setXYZ(MPL_VT_aux[0], MPL_VT_aux[1], MPL_VT_aux[2]);

    }


//    inline int getNeighborPotentialZNext(MPL_PFE_VarType& MPL_VT_np, MPL_Position sMPLP_p)
//    {

//        return(MPL_ReturnCode_OK);
//    }

//    inline int getNeighborPotentialZBack(MPL_PFE_VarType& MPL_VT_np, MPL_Position sMPLP_p)
//    {

//        return(MPL_ReturnCode_OK);
//    }

//    inline int getNeighborPotentialYNext(MPL_PFE_VarType& MPL_VT_np, MPL_Position sMPLP_p)
//    {

//        return(MPL_ReturnCode_OK);
//    }
//    inline int getNeighborPotentialYBack(MPL_PFE_VarType& MPL_VT_np, MPL_Position sMPLP_p)
//    {

//        return(MPL_ReturnCode_OK);
//    }

//    inline int getNeighborPotentialXNext(MPL_PFE_VarType& MPL_VT_np, MPL_Position sMPLP_p)
//    {

//        return(MPL_ReturnCode_OK);
//    }
//    inline int getNeighborPotentialXBack(MPL_PFE_VarType& MPL_VT_np, MPL_Position sMPLP_p)
//    {

//        return(MPL_ReturnCode_OK);
//    }


    inline void unsafe_dumpPotentialGrid(FILE *out)
    {
        int x;
        int y;
        int z;

        int zMax = getGridSizeZ();
        int yMax = getGridSizeY();
        int xMax = getGridSizeX();

        for( z=0; z<zMax; z++ ){
            fprintf(out, "\n-------------------\nz=%d\n", z);
            for(y=0; y<yMax; y++){
                for( x=0; x<xMax; x++){
                    fprintf(out, MPL_PFE_VarType_CharCode " ", qvMPL_PFE_VarType_potential3DMatrix__[unsafe_arrayIndex(x,y,z)]);
                }
                fprintf(out, "\n");
            }
            fprintf(out, "-------------------\n\n");
        }
    }


protected:

    inline void unsafe_setPotentialCellOccupiedByEnemyRobot(MPL_Position& sMPLP_p)
    {
        unsafe_setPotential(MPL_PFE_MaxPotential, sMPLP_p);

        return;
    }


    inline void unsafe_setPotentialCellOccupiedByMovableObstacle(MPL_Position& sMPLP_p)
    {
        unsafe_setPotential(MPL_PFE_MaxPotential, sMPLP_p);

        return;
    }


    inline void unsafe_setPotentialCellOccupiedByFixedObstacle(MPL_Position& sMPLP_p)
    {
        unsafe_setPotential(MPL_PFE_MaxPotential, sMPLP_p);

        return;
    }


    inline void unsafe_setPotentialCellOccupiedByGoal(MPL_Position& sMPLP_p)
    {
        unsafe_setPotential(MPL_PFE_MinPotential, sMPLP_p);

        return;
    }


private:
    /**
     * @brief qvMPL_PE_VarType_potential3DMatrix__
     * The grid of potential cells itself
     *
     */
    QVector<MPL_PFE_VarType> qvMPL_PFE_VarType_potential3DMatrix__;


};

#endif // MPL_POTENTIALFIELDENVIRONMENT_H
