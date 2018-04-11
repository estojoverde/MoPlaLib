#ifndef MPL_HARMONICPOTENTIALFIELD_H
#define MPL_HARMONICPOTENTIALFIELD_H

#include "mpl_potentialfieldenvironment.h"

#define MPL_HPF_ReturnCode_InvalidOmega 1
#define MPL_HPF_ReturnCode_InvalidIterationMode 2

enum MPL_HPF_IterateModes{ MPL_HPF_GS, MPL_HPF_SOR};

class MPL_HarmonicPotentialField : public MPL_PotentialFieldEnvironment
{
public:
    MPL_HarmonicPotentialField(int n_sizeX=1, int n_sizeY=1, int n_sizeZ=1, QObject *parent=0):MPL_PotentialFieldEnvironment(n_sizeX, n_sizeY, n_sizeZ, parent)
    {

        d_omegaSor__=1;
    }



    int iterator_GS(double &error);
    int iterator_SOR(double &error);

    int solveI(MPL_HPF_IterateModes Mpl_Hpf_itMode, int i_maxIt, int &i_itOut, double &d_errorOut);
    int solveE(MPL_HPF_IterateModes Mpl_Hpf_itMode, double d_maxError, int &i_itOut, double &d_errorOut);
    int solveIE(MPL_HPF_IterateModes Mpl_Hpf_itMode, int i_maxIt, double d_maxError, int &i_itOut, double &d_errorOut);


    inline int setOmega(double d_omega){
        if(d_omega>0 && d_omega < 2){
            d_omegaSor__ = d_omega;
            return(MPL_ReturnCode_OK);
        }
        else{
            MPL_ERROR_MESSAGE("Invalid Omega-Sor Range %lf not in (0,2)", d_omega );
            return(MPL_HPF_ReturnCode_InvalidOmega);
        }

    }

    inline int getOmega(double &d_omega ){

        d_omega=d_omegaSor__;
        return(MPL_ReturnCode_OK);
    }




protected:
    double d_omegaSor__;





};

#endif // MPL_HARMONICPOTENTIALFIELD_H
