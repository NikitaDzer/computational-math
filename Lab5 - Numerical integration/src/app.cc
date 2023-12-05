#include "app.hh"
#include "plot.hh"
#include "interp.hh"

#include <cmath>
#include <format>
#include <vector>
#include <string>
#include <string_view>

namespace app
{

namespace
{

std::vector<visual::GNUPlot::Point>
runInterpolantCalculation( interp::Interpolant interpolant,
                           double begin,
                           double end,
                           unsigned points_count)
{
    std::vector<visual::GNUPlot::Point> results{};
    results.reserve( points_count);

    for ( unsigned i = 0; i < points_count; i++ )
    {
        double xi = begin + i * (end - begin) / points_count;
        double fi = interpolant( xi);

        results.push_back( {xi, fi});
    }

    return results;
} // runInterpolantCalculation

} // namespace anonymous

void
drawInterpolantPlot( interp::Interpolant interpolant,
                     double begin,
                     double end,
                     visual::GNUPlot& plot,
                     std::string_view method_name)
{
    unsigned points_count = 300;

    std::vector<visual::GNUPlot::Point> results = runInterpolantCalculation(
        interpolant, begin, end, points_count
    );

    plot.drawPlot( results, method_name );
} // drawInterpolantPlot

void
initInterpolantPlot( visual::GNUPlot& plot,
                     std::string image_dir,
                     std::string plot_name)
{
    std::string title = std::format(
        "Tabulated function interpolated by the {} method.",
        plot_name
    );

    plot.setTitle( title, 20);
    plot.setOutputFile( 
        image_dir + plot_name,
        "svg", 800, 600 
    );

    plot.setGrid();

    plot.setMTicsX( 1);
    plot.setMTicsY( 1);

    plot.setAutoscale();

    plot.setLabelX( "x");
    plot.setLabelY( "y");

    plot.withLines();
} // initInterpolantPlot

std::pair< std::vector<double>, double>
createTabulatedFunctionData()
{
    std::vector<double> f{
        1.0, 0.989616, 0.958851, 0.908852, 0.841471,
        0.759188, 0.664997, 0.562278, 0.454649
    };
    double h = 0.25;

    return { f, h};
} // createTabulatedFunctionData

std::pair< std::vector<double>, std::vector<double>>
createInterpolantData()
{
    std::vector<double> f{
        1.0, 1.5403, 1.5839, 2.01, 3.3464
    };

    std::vector<double> x{
        0.0, 1.0, 2.0, 3.0, 4.0
    };

    return { f, x};
} // createInterpolantData

} // namespace app
