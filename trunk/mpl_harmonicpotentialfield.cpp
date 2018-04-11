#include "mpl_harmonicpotentialfield.h"

int MPL_HarmonicPotentialField::iterator_GS(double &error)
{
    MPL_PFE_VarType MPL_VT_np[6], MPL_VT_newPotential, MPL_VT_oldPotential;
    int x;
    int y;
    int z;

    int zMax = getGridSizeZ();
    int yMax = getGridSizeY();
    int xMax = getGridSizeX();


    error=0;
    for( z=0; z<zMax; z++ )
    {
        for( x=0; x<xMax; x++)
        {
            for( y=0; y<yMax; y++)
            {
                if(!((MPL_OccupancyGrid_CellFree==unsafe_getOccupancy(x, y, z)) || \
                     (MPL_OccupancyGrid_CellNotExplored==unsafe_getOccupancy(x, y, z)) ||
                     (MPL_OccupancyGrid_CellOccupiedByOwnRobot==unsafe_getOccupancy(x, y, z)) )){
                    continue;
                }

                unsafe_getPotential(x,y,z, MPL_VT_oldPotential);

                getNeighborPotential(x,y,z, MPL_VT_np);

                MPL_VT_newPotential = (MPL_PFE_VarType)((MPL_VT_np[0] + MPL_VT_np[1] + MPL_VT_np[2]+\
                                                         MPL_VT_np[3] + MPL_VT_np[4] + MPL_VT_np[5])/6.0);

                unsafe_setPotential(MPL_VT_newPotential, x, y, z);

                error+=(double)fabs(MPL_VT_newPotential-MPL_VT_oldPotential);
            }
        }
    }

    return(MPL_ReturnCode_OK);
}



int MPL_HarmonicPotentialField::iterator_SOR(double &error){
    MPL_PFE_VarType MPL_VT_np[6], MPL_VT_newPotential, MPL_VT_oldPotential;
    int x;
    int y;
    int z;

    int zMax = getGridSizeZ();
    int yMax = getGridSizeY();
    int xMax = getGridSizeX();


    error=0;
    for( z=0; z<zMax; z++ )
    {
        for( x=0; x<xMax; x++)
        {
            for( y=0; y<yMax; y++)
            {
                if(!((MPL_OccupancyGrid_CellFree==unsafe_getOccupancy(x, y, z)) || \
                     (MPL_OccupancyGrid_CellNotExplored==unsafe_getOccupancy(x, y, z)) ||
                     (MPL_OccupancyGrid_CellOccupiedByOwnRobot==unsafe_getOccupancy(x, y, z)) )){
                    continue;
                }

                unsafe_getPotential(x,y,z, MPL_VT_oldPotential);

                getNeighborPotential(x,y,z, MPL_VT_np);

                MPL_VT_newPotential = (MPL_PFE_VarType)((MPL_VT_np[0] + MPL_VT_np[1] + MPL_VT_np[2]+\
                                                         MPL_VT_np[3] + MPL_VT_np[4] + MPL_VT_np[5])/6.0);

                //SORing
                MPL_VT_newPotential = (MPL_PFE_VarType)( ((1-d_omegaSor__)*MPL_VT_oldPotential) + ((d_omegaSor__)*MPL_VT_newPotential )  );

                unsafe_setPotential(MPL_VT_newPotential, x, y, z);

                error+=(double)fabs(MPL_VT_newPotential-MPL_VT_oldPotential);
            }
        }
    }

    return(MPL_ReturnCode_OK);
}

int MPL_HarmonicPotentialField::solveI(MPL_HPF_IterateModes Mpl_Hpf_itMode, int i_maxIt, int &i_itOut, double &d_errorOut)
{
    int i;
    double error;

    switch(Mpl_Hpf_itMode){
    case MPL_HPF_GS:
        for( i=0; i<i_maxIt; i++ ){
            iterator_GS(error);
        }

        break;

    case MPL_HPF_SOR:
        for( i=0; i<i_maxIt; i++ ){
            iterator_SOR(error);
        }

        break;

    default:
        MPL_ERROR_MESSAGE("Unknow iteration mode");
        return(MPL_HPF_ReturnCode_InvalidIterationMode);
    }

    i_itOut=i;
    d_errorOut=error;

    return(MPL_ReturnCode_OK);
}



int MPL_HarmonicPotentialField::solveE(MPL_HPF_IterateModes Mpl_Hpf_itMode, double d_maxError, int &i_itOut, double &d_errorOut)
{
    int i=0;
    double error=0;

    switch(Mpl_Hpf_itMode){
    case MPL_HPF_GS:
        do{
            iterator_GS(error);
            i++;
        }while( error>d_maxError );

        break;

    case MPL_HPF_SOR:
        do{
            iterator_SOR(error);
            i++;
        }while( error>d_maxError );


        break;

    default:
        MPL_ERROR_MESSAGE("Unknow iteration mode");
        return(MPL_HPF_ReturnCode_InvalidIterationMode);
    }

    i_itOut=i;
    d_errorOut=error;
    return(MPL_ReturnCode_OK);
}


int MPL_HarmonicPotentialField::solveIE(MPL_HPF_IterateModes Mpl_Hpf_itMode, int i_maxIt, double d_maxError, int &i_itOut, double &d_errorOut)
{
    int i=0;
    double error=0;

    switch(Mpl_Hpf_itMode){
    case MPL_HPF_GS:
        do{
            iterator_GS(error);
            i++;
        }while( (error>d_maxError) && (i<i_maxIt) );

        break;

    case MPL_HPF_SOR:
        do{
            iterator_SOR(error);
            i++;
        }while( error>d_maxError && (i<i_maxIt) );

        break;

    default:
        MPL_ERROR_MESSAGE("Unknow iteration mode");
        return(MPL_HPF_ReturnCode_InvalidIterationMode);
    }

    i_itOut=i;
    d_errorOut=error;

    return(MPL_ReturnCode_OK);
}


