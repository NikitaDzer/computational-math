#include "grid.hh"
#include "plot.hh"

#include <format>

namespace
{

void
drawLineScheme( const pde::Grid& grid,
                int t,
                visual::GNUPlot& plot)
{
    auto [x_size, t_size] = grid.getSize();
    auto [h, a] = grid.getPitch();

    std::vector<visual::GNUPlot::Point> results{};
    results.push_back( {0, grid.getLeftNeighbour()->getAbs( 0, t)});

    for ( int x = 0; x < x_size; x++ )
    {
        results.push_back( {h*(x + 1), grid.getAbs( x, t)});
    }

    std::string fmt = std::format( "t = {}", a*t);
    plot.drawPlot( results, fmt);
} // drawLineScheme

} // anonymous namespace

namespace app
{

void
initPlot( visual::GNUPlot& plot,
          std::string image_dir,
          std::string plot_name,
          std::string grid_name,
          std::string scheme_name)
{
    std::string title = std::format(
        "u(x, t = const) for various t ({}).", scheme_name
    );

    plot.setTitle( title, 20);
    plot.setOutputFile( 
        image_dir + plot_name + "-" + grid_name + "-" + scheme_name,
        "svg", 500, 300
    );

    plot.setGrid();

    plot.setMTicsX( 1);
    plot.setMTicsY( 1);

    plot.setAutoscale();

    plot.setLabelX( "x" );
    plot.setLabelY( "u(x)" );

    // Yep, it is crutch.
    if ( grid_name == "(x-t)" )
    {
        plot.setRangeY( -20, 20);
    } else if ( grid_name == "(x*x+t*t)" )
    {
        plot.setRangeY( 0, 250);
    } else
    {
        assert( 0 && "unreachable");
        std::abort();
    }

    plot.withLines();
} // initPlot

void
drawGrid( const pde::Grid& grid,
          visual::GNUPlot& plot)
{
    auto [x_size, t_size] = grid.getSize();
    auto [h, a] = grid.getPitch();

    int t_step = static_cast<int>( 1.0 / a);

    drawLineScheme( grid, 0 * t_step, plot);
    drawLineScheme( grid, 5 * t_step, plot);
    drawLineScheme( grid, 10 * t_step, plot);
} // drawGrid

} // namespace app
