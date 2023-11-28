#pragma once

#include "plot.hh"
#include "interp.hh"

#include <string>
#include <vector>
#include <utility>
#include <functional>
#include <string_view>

namespace app 
{

/**
 * Initialize a plot configuration.
 */
void
initPopulationPlot( visual::GNUPlot& plot,
                    std::string image_dir,
                    std::string plot_name);

/**
 * Draw a plot with the dependence of the USA population on the year.
 */
void
drawPopulationPlot( interp::Interpolant interpolant,
                    double year_start,
                    double year_end,
                    unsigned points_count,
                    visual::GNUPlot& plot,
                    std::string_view method_name);

/**
 * Create initial data about population of the USA.
 */
std::pair< std::vector<double>, std::vector<double>>
createPopulationData();

} // namespace app
