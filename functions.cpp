#include "functions.hpp"
//#include "network.hpp"
#include <algorithm>

/* This file will contain all mathematical functions to be called by network and spline */

Scalar silu(Scalar)
{
    // silu function (basis function).
    return 0;
}

Scalar sumScalars(std::vector<Scalar> const &vector)
{
    Scalar sum_of_elems = 0.0;
    std::for_each(vector.begin(), vector.end(), [&](Scalar n) {
        sum_of_elems += n;
    });
    return sum_of_elems;
}