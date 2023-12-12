#include "integral.hh"

#include <span>
#include <cmath>
#include <vector>
#include <numeric>
#include <cassert>

namespace integr
{

double
calcIntegralTrapezoid( std::span<const double> f,
                       double h)
{
    assert( f.size() >= 2);

    double sum = 
        std::accumulate( std::next( f.begin()), std::prev( f.end()), 0.0);

    return h * ( sum + (f.front() + f.back()) / 2);
} // calcIntegralTrapezoid

double
calcIntegralSimpson( std::span<const double> f,
                     double h)
{
    assert( f.size() >= 2);

    double sum = 0;

    for ( int i = 1; i < (f.size() - 1); i += 2 )
    {
        sum += f[i - 1] + 4 * f[i] + f[i + 1];
    }

    return (h * sum) / 3;
} // calcIntegralSimpson

double
calcIntegralRichardson( std::span<const double> f,
                        double h,
                        Integrator integrator,
                        double power)
{
    assert( f.size() >= 4);

    std::vector<double> f2h{};
    f2h.reserve( f.size() / 2);

    for ( int i = 0; i < f.size(); i += 2 )
    {
        f2h.push_back( f[i]);
    }

    double Ih = integrator( f, h);
    double I2h = integrator( f2h, 2 * h);

    return Ih + (Ih - I2h) / (std::pow( 2, power) - 1);
} // calcIntegralRichardson

} // namespace integr
