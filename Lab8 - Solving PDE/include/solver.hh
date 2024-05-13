#pragma once

#include "grid.hh"
#include "scheme.hh"

namespace pde
{

void
solveGridSeq( pde::Grid& grid,
              pde::Scheme scheme);

void
solveGridPar( pde::Grid& grid,
              pde::Scheme scheme,
              int rank);

} // namespace pde
