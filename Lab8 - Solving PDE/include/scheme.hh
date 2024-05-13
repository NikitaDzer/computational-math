#pragma once

#include "grid.hh"
#include <functional>

namespace pde
{

using Scheme = std::function<double( const pde::Grid&)>;

double
schemeCentral( const pde::Grid& grid);

double
schemeLax( const pde::Grid& grid);

double
schemeRectangle( const pde::Grid& grid);

} // namespace pde
