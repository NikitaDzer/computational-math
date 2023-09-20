#include "approximation.hh"

namespace appr
{

double 
calcDerivative1( InputFunction f,
                 double x,
                 double h )
{
    return (f(x + h) - f(x)) / h;
} // calcDerivative1

double 
calcDerivative2( InputFunction f,
                 double x,
                 double h )
{
    return (f(x) - f(x - h)) / h;
} // calcDerivative2

double 
calcDerivative3( InputFunction f,
                 double x,
                 double h )
{
    return (f(x + h) - f(x - h)) / (2*h);
} // calcDerivative3

double 
calcDerivative4( InputFunction f,
                 double x,
                 double h )
{
    double summand_1 = (4.0/3.0) * (f(x + h) - f(x - h)) / (2*h);
    double summand_2 = -(1.0/3.0) * (f(x + 2*h) - f(x - 2*h)) / (4*h); 

    return summand_1 + summand_2;
} // calcDerivative4

double 
calcDerivative5( InputFunction f,
                 double x,
                 double h )
{
    double summand_1 = (3.0/2.0) * (f(x + h) - f(x - h)) / (2*h);
    double summand_2 = -(3.0/5.0) * (f(x + 2*h) - f(x - 2*h)) / (4*h); 
    double summand_3 = (1.0/10.0) * (f(x + 3*h) - f(x - 3*h)) / (6*h);

    return summand_1 + summand_2 + summand_3;
} // calcDerivative5

} // namespace appr
