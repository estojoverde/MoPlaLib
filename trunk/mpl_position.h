#ifndef MPL_POSITION_H
#define MPL_POSITION_H

#include <cmath>
#include <qmetatype.h>
#include "mpl_utils.h"

#define MPL_Position_DELTA 0.0000000001 //smallest distance (that can be choosed from bellow methods) between two positions


class MPL_Position
{
public:

    MPL_Position(double d_x__, double d_y__, double d_z__)
    {
        setX(d_x__);
        setY(d_y__);
        setZ(d_z__);
    }

    MPL_Position()
    {
        setX(0);
        setY(0);
        setZ(0);
    }


    //getters and setters
    inline void setX(double d_x)
    {
        d_x__=d_x;
    }

    inline void setY(double d_y)
    {
        d_y__=d_y;
    }

    inline void setZ(double d_z)
    {
        d_z__=d_z;
    }

    inline void setXYZ(double d_x, double d_y, double d_z)
    {
        d_x__=d_x;
        d_y__=d_y;
        d_z__=d_z;

    }


    inline double getX()
    {
        return(this->d_x__);
    }

    inline double getY()
    {
        return(this->d_y__);
    }

    inline double getZ()
    {
        return(this->d_z__);
    }



    //distances (a-b)
    inline static double euclideanDist( MPL_Position &r_sMPLP_a,  MPL_Position &r_sMPLP_b)
    {
        return ( sqrt(pow((r_sMPLP_a.d_x__ - r_sMPLP_b.d_x__), 2) + pow((r_sMPLP_a.d_y__ - r_sMPLP_b.d_y__), 2) + pow((r_sMPLP_a.d_z__ - r_sMPLP_b.d_z__), 2) ) );
    }

    inline static double manhattanDist( MPL_Position &r_sMPLP_a,  MPL_Position &r_sMPLP_b)
    {
        return( fabs(r_sMPLP_a.d_x__ - r_sMPLP_b.d_x__) + fabs(r_sMPLP_a.d_y__ - r_sMPLP_b.d_y__) + fabs(r_sMPLP_a.d_z__ - r_sMPLP_b.d_z__));
    }

    static double chebyshevDist( MPL_Position &r_sMPLP_a,  MPL_Position &r_sMPLP_b)
    {
        return( MPL_MAX3(fabs(r_sMPLP_a.d_x__ - r_sMPLP_b.d_x__), fabs(r_sMPLP_a.d_y__ - r_sMPLP_b.d_y__), fabs(r_sMPLP_a.d_z__ - r_sMPLP_b.d_z__)) );
    }



    //distances (this-other)
    inline double euclideanDist( MPL_Position &r_sMPLP_other)
    {
        return( euclideanDist(*this, r_sMPLP_other) );
    }

    inline double manhattanDist( MPL_Position &r_sMPLP_other)
    {
        return( manhattanDist(*this, r_sMPLP_other) );
    }

    inline double chebyshevDist( MPL_Position &r_sMPLP_other)
    {
        return( chebyshevDist(*this, r_sMPLP_other) );
    }


    //operators

    inline MPL_Position& operator=( MPL_Position &r_sMPLP_other)
    {

        if (this != &r_sMPLP_other) { // Only do assignment if r_sMPLP_other is a different object from this.
            this->d_x__ = r_sMPLP_other.d_x__;
            this->d_y__ = r_sMPLP_other.d_y__;
            this->d_z__ = r_sMPLP_other.d_z__;
        }

        return (*this);
    }

    inline const MPL_Position& operator=(const MPL_Position &cr_sMPLP_other)
    {
        if (this != &cr_sMPLP_other) { // Only do assignment if r_sMPLP_other is a different object from this.
            this->d_x__ = cr_sMPLP_other.d_x__;
            this->d_y__ = cr_sMPLP_other.d_y__;
            this->d_z__ = cr_sMPLP_other.d_z__;
        }

        return (*this);
    }

    inline MPL_Position& operator+=( MPL_Position &r_sMPLP_other)
    {
        this->d_x__ += r_sMPLP_other.d_x__;
        this->d_y__ += r_sMPLP_other.d_y__;
        this->d_z__ += r_sMPLP_other.d_z__;

        return (*this);
    }

    inline MPL_Position& operator-=( MPL_Position &r_sMPLP_other)
    {
        this->d_x__ -= r_sMPLP_other.d_x__;
        this->d_y__ -= r_sMPLP_other.d_y__;
        this->d_z__ -= r_sMPLP_other.d_z__;

        return (*this);
    }

    inline MPL_Position operator+( MPL_Position &r_sMPLP_other)
    {
        MPL_Position result = *this;
        result += r_sMPLP_other;

        return (result);
    }

    inline MPL_Position operator-( MPL_Position &r_sMPLP_other)
    {
        MPL_Position result = *this;
        result -= r_sMPLP_other;

        return (result);
    }

    inline bool operator==( MPL_Position &r_sMPLP_other)
    {
        if( manhattanDist(r_sMPLP_other) < MPL_Position_DELTA )
            return true;
        else
            return false;
    }

    inline bool operator!=( MPL_Position &r_sMPLP_other)
    {
        return( !(operator==(r_sMPLP_other)) );
    }


    inline void reciprocal( void )
    {
        d_x__ = ( fabs(d_x__)>MPL_Position_DELTA? 1/d_x__ : 1/MPL_Position_DELTA );
        d_y__ = ( fabs(d_y__)>MPL_Position_DELTA? 1/d_y__ : 1/MPL_Position_DELTA );
        d_z__ = ( fabs(d_z__)>MPL_Position_DELTA? 1/d_z__ : 1/MPL_Position_DELTA );
    }

    inline void inverse( void )
    {
        d_x__ = -d_x__;
        d_y__ = -d_y__;
        d_z__ = -d_z__;
    }


private:
    double d_x__;
    double d_y__;
    double d_z__;

};


Q_DECLARE_METATYPE(MPL_Position)


#endif // MPL_POSITION_H
