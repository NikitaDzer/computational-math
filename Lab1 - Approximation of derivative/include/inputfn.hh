#pragma once

#include <cmath>

namespace app
{

/**
 * f(x) = sin(x^2)
 */
inline double sinM( double x )
    { return std::sin( x * x ); }

/**
 * f(x) = cos(sin(x))
 */
inline double cosSin( double x )
    { return std::cos( std::sin( x ) ); }

/**
 * f(x) = exp(sin(cos(x)))
 */
inline double expSinCos( double x )
    { return std::exp( std::sin( std::cos( x ) ) ); } 

/**
 * f(x) = ln(x + 3)
 */
inline double lnM( double x )
    { return std::log( x + 3 ); }

/**
 * f(x) = sqrt(x + 3)
 */
inline double sqrtM( double x )
    { return std::sqrt( x + 3 ); }

/**
 * f'(x) = cos(x^2) * (2x)
 */
inline double derivativeSinM( double x )
    { return std::cos( x * x ) * (2 * x); }

/**
 * f'(x) = -sin(sin(x)) * cos(x)
 */
inline double derivativeCosSin( double x )
    { return -std::sin( std::sin( x ) ) * std::cos( x ); }

/**
 * f'(x) = -exp(sin(cos(x))) * cos(cos(x)) * sin(x)
 */
inline double derivativeExpSinCos( double x )
    { return -expSinCos( x ) * std::cos( std::cos( x ) ) * std::sin( x ); }

/**
 * f'(x) = 1 / (x + 3)
 */
inline double derivativeLnM( double x )
    { return 1.0 / (x + 3); }

/**
 * f'(x) = 1 / (2 * sqrt(x + 3))
 */
inline double derivativeSqrtM( double x )
    { return 1.0 / (2 * sqrtM( x )); }

} // namespace app
