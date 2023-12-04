#include "trapezoid.hh"

#include <span>
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

} // namespace integr
