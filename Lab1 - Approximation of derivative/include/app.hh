#pragma once

#include "plot.hh"
#include "approximation.hh"

#include <string_view>

namespace app 
{

/**
 * Make plot with error on step of all approximate methods.
 */
void
makeDerivativeErrorPlot( appr::InputFunction ref_f1,
                         appr::InputFunction f,
                         double x,
                         visual::GNUPlot& plot,
                         std::string_view function_name,
                         std::string_view image_name );

/**
 * Init plot settings.
 */
void
initDerivativeErrorPlot( visual::GNUPlot& plot );

} // namespace app
