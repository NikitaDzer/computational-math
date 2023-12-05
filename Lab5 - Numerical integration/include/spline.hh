#pragma once

#include "interp.hh"

#include <span>

namespace interp
{

Interpolant
makeInterpolantSpline( std::span<const double> f,
                       std::span<const double> x);

} // namespace interp
