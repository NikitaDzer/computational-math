#pragma once

#include <functional>
#include <span>

namespace integr
{

using Integrator = 
    std::function<double( std::span<const double> f, double h)>;

double
calcIntegralTrapezoid( std::span<const double> f,
                       double h);

double
calcIntegralSimpson( std::span<const double> f,
                     double h);

double
calcIntegralRichardson( std::span<const double> f,
                        double h,
                        Integrator integrator,
                        double power);

} // namespace integr
