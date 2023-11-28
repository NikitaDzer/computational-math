#pragma once

#include "interp.hh"

#include <span>

namespace interp
{

double
calcDividedDiff( std::span<const double> f,
                 std::span<const double> x,
                 unsigned count,
                 unsigned i);

Interpolant
makeInterpolantNewton( std::span<const double> f,
                       std::span<const double> x,
                       unsigned preferred_order);

double
calcInterpolantErrorNewton( std::span<const double> f,
                            std::span<const double> x,
                            unsigned order);

} // namespace interp
