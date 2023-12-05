#include "app.hh"
#include "plot.hh"
#include "interp.hh"
#include "spline.hh"
#include "trapezoid.hh"

#include <cmath>
#include <string>
#include <iostream>

int 
main()
{
    const std::string k_dir = "assets/";

    auto [ f1, h1 ] = app::createTabulatedFunctionData();
    auto [ f2, x ] = app::createInterpolantData();

    interp::Interpolant spline = interp::makeInterpolantSpline( f2, x);
    interp::Interpolant osc_spline = [&spline]( double xi) {
        return spline( xi) * std::sin( 30 * xi);
    };
    
    visual::GNUPlot plot{};

    app::initInterpolantPlot( plot, k_dir, "splines");
    app::drawInterpolantPlot(
        spline, x.front(), x.back(), plot, "tabulated function"
    );
    app::drawInterpolantPlot(
        osc_spline, x.front(), x.back(), plot, "oscillating function"
    );
    plot.makePlot();

    std::cout << "VII 9.5a. Integral: " 
              << integr::calcIntegralTrapezoid( f1, h1) 
              << "\n";

    std::cout << "VII 9.13d. Integral: " 
              << integr::calcIntegralTrapezoid( f2, 0.01)
              << "\n";

    return 0;
} // main
