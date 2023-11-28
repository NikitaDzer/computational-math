#include "app.hh"
#include "lsm.hh"
#include "spline.hh"
#include "utils.hh"
#include "newton.hh"

#include <format>
#include <string>
#include <iostream>

namespace
{

void
runInterpolation( interp::Interpolant interpolant,
                  std::string method_name,
                  std::span<const double> f,
                  std::span<const double> x,
                  double year_x)
{
    const double k_year_start = 1900;
    const double k_year_end = 2020;
    const unsigned k_points_num = 20;

    const std::string k_dir = "assets/";

    visual::GNUPlot plot{};

    app::initPopulationPlot( plot, k_dir, method_name); 
    app::drawPopulationPlot(
        interpolant,
        f, x,
        k_year_start, k_year_end, k_points_num,
        plot, method_name
    );
    plot.makePlot();

    std::cout << std::format(
        "Population in {} ({}) is {}.\n",
        year_x, method_name, interpolant( year_x)
    );
} // runInterpolation

} // namespace anonymous

int 
main( const int argc, const char *argv[])
{
    double year_x = 2010;

    if ( argc >= 2 )
    {
        year_x = std::atof( argv[1]);
    }

    auto [years, population] = app::createPopulationData();

    interp::Interpolant lsm = 
        interp::makeInterpolantLsm( population, years, 2);
    interp::Interpolant spline = 
        interp::makeInterpolantSpline( population, years);
    interp::Interpolant newton = 
        interp::makeInterpolantNewton( population, years, years.size() - 2);

    runInterpolation( lsm, "lsm", population, years, year_x);
    runInterpolation( spline, "spline", population, years, year_x);
    runInterpolation( newton, "newton", population, years, year_x);

    std::cout << "Lsm error: " 
              << interp::calcInterpolantErrorLsm( lsm, population, years) 
              << std::endl;

    std::cout << "Newton error: " 
              << interp::calcInterpolantErrorNewton( population, years, years.size() - 2)
              << std::endl;

    return 0;
} // main
