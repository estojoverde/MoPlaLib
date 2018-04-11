#include "mpl_orientedpotentialfield.h"


int MPL_OrientedPotentialField::iterator_GS(double &error)
{
    MPL_PFE_VarType MPL_VT_np[6], MPL_VT_newPotential, MPL_VT_oldPotential;
    int x;
    int y;
    int z;

    int zMax = getGridSizeZ();
    int yMax = getGridSizeY();
    int xMax = getGridSizeX();

    double d_lambda_vx=(1 + (d_epsilon__*fabs(d_vx__)));
    double d_lambda_vy=(1 + (d_epsilon__*fabs(d_vy__)));
    double d_lambda_vz=(1 + (d_epsilon__*fabs(d_vz__)));

    double d_denom=(6 + d_epsilon__ * (fabs(d_vx__) + fabs(d_vy__) + fabs(d_vz__)) );


    int i_xUp, i_xDown, i_yUp, i_yDown, i_zUp, i_zDown;

    //upwinding
    if( d_vx__ < 0 )
    {//invert
        i_xDown = 0;
        i_xUp = 1;
    }
    else
    {//usual
        i_xDown = 1;
        i_xUp = 0;
    }

    if( d_vy__ < 0 )
    {//invert
        i_yDown = 2;
        i_yUp = 3;
    }
    else
    {//usual
        i_yDown = 3;
        i_yUp = 2;
    }

    if( d_vz__ < 0 )
    {//invert
        i_zDown = 4;
        i_zUp = 5;
    }
    else
    {//usual
        i_zDown = 5;
        i_zUp = 4;
    }

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

                MPL_VT_newPotential = (MPL_PFE_VarType)((MPL_VT_np[i_xUp]*d_lambda_vx + MPL_VT_np[i_xDown]+\
                                                         MPL_VT_np[i_yUp]*d_lambda_vy + MPL_VT_np[i_yDown]+\
                                                         MPL_VT_np[i_zUp]*d_lambda_vz + MPL_VT_np[i_zDown])/d_denom);



                unsafe_setPotential(MPL_VT_newPotential, x, y, z);

                error+=(double)fabs(MPL_VT_newPotential-MPL_VT_oldPotential);
            }
        }
    }

    return(MPL_ReturnCode_OK);
}

//int MPL_OrientedPotentialField::pseudo_iterator_GS(double &error){}



int MPL_OrientedPotentialField::iterator_SOR(double &error){
    MPL_PFE_VarType MPL_VT_np[6], MPL_VT_newPotential, MPL_VT_oldPotential;
    int x;
    int y;
    int z;

    int zMax = getGridSizeZ();
    int yMax = getGridSizeY();
    int xMax = getGridSizeX();

    double d_lambda_vx=(1 + (d_epsilon__*fabs(d_vx__)));
    double d_lambda_vy=(1 + (d_epsilon__*fabs(d_vy__)));
    double d_lambda_vz=(1 + (d_epsilon__*fabs(d_vz__)));

    double d_denom=(6 + d_epsilon__ * (fabs(d_vx__) + fabs(d_vy__) + fabs(d_vz__)) );


    int i_xUp, i_xDown, i_yUp, i_yDown, i_zUp, i_zDown;

    //upwinding
    if( d_vx__ < 0 )
    {//invert
        i_xDown = 0;
        i_xUp = 1;
    }
    else
    {//usual
        i_xDown = 1;
        i_xUp = 0;
    }

    if( d_vy__ < 0 )
    {//invert
        i_yDown = 2;
        i_yUp = 3;
    }
    else
    {//usual
        i_yDown = 3;
        i_yUp = 2;
    }

    if( d_vz__ < 0 )
    {//invert
        i_zDown = 4;
        i_zUp = 5;
    }
    else
    {//usual
        i_zDown = 5;
        i_zUp = 4;
    }

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

                MPL_VT_newPotential = (MPL_PFE_VarType)((MPL_VT_np[i_xUp]*d_lambda_vx + MPL_VT_np[i_xDown]+\
                                                         MPL_VT_np[i_yUp]*d_lambda_vy + MPL_VT_np[i_yDown]+\
                                                         MPL_VT_np[i_zUp]*d_lambda_vz + MPL_VT_np[i_zDown])/d_denom);

                //SORing
                MPL_VT_newPotential = (MPL_PFE_VarType)( ((1-d_omegaSor__)*MPL_VT_oldPotential) + ((d_omegaSor__)*MPL_VT_newPotential )  );

                unsafe_setPotential(MPL_VT_newPotential, x, y, z);

                error+=(double)fabs(MPL_VT_newPotential-MPL_VT_oldPotential);
            }
        }
    }

    return(MPL_ReturnCode_OK);
}





int MPL_OrientedPotentialField::solveI(MPL_OPF_IterateModes Mpl_Opf_itMode, int i_maxItm, int &i_itOut, double &d_errorOut)
{
    int i;
    double error;

    switch(Mpl_Opf_itMode){
    case MPL_OPF_GS:
        for( i=0; i<i_maxItm; i++ ){
            iterator_GS(error);
        }

        if(b_verbose__){
            MPL_INFORMATION_MESSAGE("Iterator_GS - Iterations(%d) - Error(%lf)", i, error);
        }

        break;

    case MPL_OPF_SOR:
        for( i=0; i<i_maxItm; i++ ){
            iterator_SOR(error);
        }

        if(b_verbose__){
            MPL_INFORMATION_MESSAGE("Iterator_SOR - Iterations(%d) - Error(%lf)", i, error);
        }

        break;

    default:
        MPL_ERROR_MESSAGE("Unknow iteration mode");
        return(MPL_OPF_ReturnCode_InvalidIterationMode);
    }

    i_itOut=i;
    d_errorOut=error;

    return(MPL_ReturnCode_OK);
}



int MPL_OrientedPotentialField::solveE(MPL_OPF_IterateModes Mpl_Opf_itMode, double d_maxError, int &i_itOut, double &d_errorOut)
{
    int i=0;
    double error=0;

    switch(Mpl_Opf_itMode){
    case MPL_OPF_GS:
        do{
            iterator_GS(error);
            i++;
        }while( error>d_maxError );

        if(b_verbose__){
            MPL_INFORMATION_MESSAGE("Iterator_GS - Iterations(%d) - Error(%lf)", i, error);
        }

        break;

    case MPL_OPF_SOR:
        do{
            iterator_SOR(error);
            i++;
        }while( error>d_maxError );

        if(b_verbose__){
            MPL_INFORMATION_MESSAGE("Iterator_SOR - Iterations(%d) - Error(%lf)", i, error);
        }

        break;

    default:
        MPL_ERROR_MESSAGE("Unknow iteration mode");
        return(MPL_OPF_ReturnCode_InvalidIterationMode);
    }

    i_itOut=i;
    d_errorOut=error;

    return(MPL_ReturnCode_OK);
}


int MPL_OrientedPotentialField::solveIE(MPL_OPF_IterateModes Mpl_Opf_itMode, int i_maxIt, double d_maxError, int &i_itOut, double &d_errorOut)
{
    int i=0;
    double error=0;


    emit signal_logMessage("bla");


    switch(Mpl_Opf_itMode){
    case MPL_OPF_GS:
        do{
            iterator_GS(error);
            i++;
        }while( (error>d_maxError) && (i<i_maxIt) );

        if(b_verbose__){
            MPL_INFORMATION_MESSAGE("Iterator_GS - Iterations(%d) - Error(%lf)", i, error);
        }

        break;

    case MPL_OPF_SOR:
        do{
            iterator_SOR(error);
            i++;
        }while( error>d_maxError && (i<i_maxIt) );

        if(b_verbose__){
            MPL_INFORMATION_MESSAGE("Iterator_SOR - Iterations(%d) - Error(%lf)", i, error);
        }

        break;

    default:
        MPL_ERROR_MESSAGE("Unknow iteration mode");
        return(MPL_OPF_ReturnCode_InvalidIterationMode);
    }

    i_itOut=i;
    d_errorOut=error;

    return(MPL_ReturnCode_OK);
}


