#pragma once

#include "plot.hh"
#include "interp.hh"

#include <string>
#include <vector>
#include <utility>
#include <string_view>

namespace app 
{

/**
 * Initialize a plot configuration.
 */
void
initInterpolantPlot( visual::GNUPlot& plot,
                     std::string image_dir,
                     std::string plot_name);

/**
 * Draw a plot with an interpolated graph.
 */
void
drawInterpolantPlot( interp::Interpolant interpolant,
                     double begin,
                     double end,
                     visual::GNUPlot& plot,
                     std::string_view method_name);

/**
 * Create data for a tabulated function.
 */
std::pair< std::vector<double>, double>
createTabulatedFunctionData();

/**
 * Create data to interpolate.
 */
std::pair< std::vector<double>, std::vector<double>>
createInterpolantData();

} // namespace app
