#pragma once

#include <span>
#include <functional>

namespace interp
{

using Interpolant = std::function<double(double x)>;

// double
// calcInterpolantError( Interpolant interpolant,
//                       std::span<const double> f,
//                       std::span<const double> x);

} // namespace interp
