#pragma once

#include <functional>

namespace appr
{

using InputFunction = std::function<double( double x )>;
using ApprFunction = 
    std::function<double( InputFunction f, double x, double h )>;

/**
 * Calculate approximately:
 * f'(x) = (f(x + h) - f(x)) / h 
 */
double 
calcDerivative1( InputFunction f,
                 double x,
                 double h );

/**
 * Calculate approximately:
 * f'(x) = (f(x) - f(x - h)) / h
 */
double 
calcDerivative2( InputFunction f,
                 double x,
                 double h );

/**
 * Calculate approximately:
 * f'(x) = (f(x + h) - f(x - h)) / (2h)
 */
double 
calcDerivative3( InputFunction f,
                 double x,
                 double h );

/**
 * Calculate approximately:
 * f'(x) = (4/3) * (f(x + h) - f(x - h)) / (2h) - 
 *       - (1/3) * (f(x + 2h) - f(x - 2h)) / (4h)
 */
double 
calcDerivative4( InputFunction f,
                 double x,
                 double h );

/**
 * Calculate approximately:
 * f'(x) = (3/2) * (f(x + h) - f(x - h)) / (2h) 
 *       - (3/5) * (f(x + 2h) - f(x - 2h)) / (4h)
 *       + (1/10) * (f(x + 3h) - f(x - 3h)) / (6h)
 */
double 
calcDerivative5( InputFunction f,
                 double x,
                 double h );

} // namespace appr
