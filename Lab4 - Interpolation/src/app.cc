#include "app.hh"
#include "plot.hh"
#include "utils.hh"
#include "interp.hh"

#include <cmath>
#include <format>
#include <vector>
#include <string>
#include <string_view>

namespace app
{

static std::vector<visual::GNUPlot::Point>
runPolulationCalculation( interp::Interpolant interpolant,
                          double year_start,
                          double year_end,
                          unsigned points_count)
{
    std::vector<visual::GNUPlot::Point> results{};
    results.reserve( points_count);

    for ( unsigned i = 0; i < points_count; i++ )
    {
        double year = year_start + i * (year_end - year_start) / points_count;
        double population = interpolant( year);

        results.push_back( {year, population});
    }

    return results;
} // runPolulationCalculation

void
drawPopulationPlot( interp::Interpolant interpolant,
                    double year_start,
                    double year_end,
                    unsigned points_count,
                    visual::GNUPlot& plot,
                    std::string_view method_name)
{
    std::vector<visual::GNUPlot::Point> results = runPolulationCalculation(
        interpolant, year_start, year_end, points_count
    );

    plot.drawPlot( results, method_name );
} // drawPopulationPlot

void
initPopulationPlot( visual::GNUPlot& plot,
                    std::string image_dir,
                    std::string plot_name)
{
    std::string title = std::format(
        "Dependence of the USA population on the year ({} method).",
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

    plot.setLabelX( "year");
    plot.setLabelY( "population");

    plot.withLinesPoints();
} // initPopulationPlot

std::pair< std::vector<double>, std::vector<double>>
createPopulationData()
{
    std::vector<double> years{
        1910., 1920., 1930., 1940., 1950., 
        1960., 1970., 1980., 1990., 2000.
    };

    std::vector<double> population{
        92'228'496., 106'021'537., 123'202'624., 132'164'569., 151'325'798.,
        179'323'175., 203'211'926., 226'545'805., 248'709'873., 281'421'906.
    };

    return { years, population};
} // createPopulationData

} // namespace app
