#pragma once

#include <span>

namespace integr
{

double
calcIntegralTrapezoid( std::span<const double> f,
                       double h);

} // namespace integr
