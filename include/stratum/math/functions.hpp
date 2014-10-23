#ifndef STRATUM_MATH_FUNCTIONS
#define STRATUM_MATH_FUNCTIONS

#include "stratum/macro/namespaces.hpp"

NAMESPACE_BEGIN(stratum, math)


inline double round_digits(double value, uint precision)
{
    constexpr double BASE = 10.0;
    constexpr double HALF = 5.0;

    double adjustHalf = value * ::pow(BASE, double(precision + 1) );

    if ( value < 0.0 )
        adjustHalf -= HALF;
    else
        adjustHalf += HALF;

    adjustHalf /= BASE;

    double intpart = 0.0;
    ::modf(adjustHalf, &intpart);

    return intpart / ::pow(BASE, double(precision) );
} // round_digits()


NAMESPACE_END(stratum, math)

#endif // STRATUM_MATH_FUNCTIONS