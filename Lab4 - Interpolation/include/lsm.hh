#pragma once

#include "interp.hh"

#include <span>

namespace interp
{

Interpolant
makeInterpolantLsm( std::span<const double> f,
                    std::span<const double> x,
                    unsigned order);

double
calcInterpolantErrorLsm( Interpolant interpolant,
                         std::span<const double> f,
                         std::span<const double> x);

} // namespace interp
