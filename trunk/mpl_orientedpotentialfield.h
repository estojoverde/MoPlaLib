#ifndef MPL_ORIENTEDPOTENTIALFIELD_H
#define MPL_ORIENTEDPOTENTIALFIELD_H

#include "mpl_potentialfieldenvironment.h"

#define MPL_OPF_ReturnCode_InvalidOmega 1
#define MPL_OPF_ReturnCode_InvalidIterationMode 2
#define MPL_OPF_ReturnCode_InvalidEpsilon 3
#define MPL_OPF_ReturnCode_InvalidVector 4

enum MPL_OPF_IterateModes{ MPL_OPF_GS, MPL_OPF_SOR};

class MPL_OrientedPotentialField : public MPL_PotentialFieldEnvironment
{
    Q_OBJECT

    friend class MPL_OPF_ParallelSor_MainLaucher;
public:
    MPL_OrientedPotentialField(int n_sizeX=1, int n_sizeY=1, int n_sizeZ=1, QObject *parent=0):MPL_PotentialFieldEnvironment(n_sizeX, n_sizeY, n_sizeZ, parent)
    {
        setOmega(1);
        setEpsilon(0);
        setVector(0, 0, 0);
    }



    int iterator_GS(double &error);
    int iterator_SOR(double &error);
    int iterator_QThread_GS(double &error);

    int solveI(MPL_OPF_IterateModes Mpl_Opf_itMode, int i_maxItm, int &i_itOut, double &d_errorOut);
    int solveE(MPL_OPF_IterateModes Mpl_Opf_itMode,double d_maxError, int &i_itOut, double &d_errorOut);
    int solveIE(MPL_OPF_IterateModes Mpl_Opf_itMode,int i_maxIt, double d_maxError, int &i_itOut, double &d_errorOut);


    inline int setOmega(double d_omega){
        if(d_omega>0 && d_omega < 2){
            d_omegaSor__ = d_omega;
            return(MPL_ReturnCode_OK);
        }
        else{
            MPL_ERROR_MESSAGE("Invalid Omega-Sor Range %lf not in (0,2)", d_omega );
            return(MPL_OPF_ReturnCode_InvalidOmega);
        }

    }

    inline int getOmega(double &d_omega ){

        d_omega=d_omegaSor__;
        return(MPL_ReturnCode_OK);
    }

    inline int setEpsilon(double d_epsilon){
        if(d_epsilon>=0){
            d_epsilon__ = d_epsilon;
            return(MPL_ReturnCode_OK);
        }
        else{
            MPL_ERROR_MESSAGE("Invalid Epsilon Range %lf not in (0,+00)", d_epsilon );
            return(MPL_OPF_ReturnCode_InvalidEpsilon);
        }
    }

    inline int getEpsilon(double &d_epsilon){
        d_epsilon=d_epsilon__;
        return(MPL_ReturnCode_OK);
    }


    inline int setVector(double d_vx, double d_vy, double d_vz){

        double d_modV = (d_vx*d_vx) + (d_vy*d_vy) + (d_vz*d_vz);

        //@TODO Fix magical numbers along the code
        if( d_modV < 1e-5 ){
            d_vx__=0;
            d_vy__=0;
            d_vz__=0;
        }else{
            d_vx__=d_vx/d_modV;
            d_vy__=d_vy/d_modV;
            d_vz__=d_vz/d_modV;
        }

        return(MPL_ReturnCode_OK);

    }

    inline int getVector(double &d_vx, double &d_vy, double &d_vz){
        d_vx=d_vx__;
        d_vy=d_vy__;
        d_vz=d_vz__;
        return(MPL_ReturnCode_OK);
    }






protected:



    /**
     * @brief d_omegaSor__
     * SOR weighting
     */
    double d_omegaSor__;

    /**
     * @brief d_epsilon__
     * epsilon
     */
    double d_epsilon__;



    double d_vx__;
    double d_vy__;
    double d_vz__;
};

#endif // MPL_ORIENTEDPOTENTIALFIELD_H

