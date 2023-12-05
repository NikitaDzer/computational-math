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
    const double k_h1 = 1.0e-10;

    auto [ f1, x ] = app::createInterpolantData();
    auto [ f2, h2 ] = app::createTabulatedFunctionData();

    interp::Interpolant spline = interp::makeInterpolantSpline( f1, x);
    interp::Interpolant osc_spline = [spline]( double xi) {
        return spline( xi) * std::sin( 30.0 * xi);
    };
    
    visual::GNUPlot plot{};

    app::initInterpolantPlot( plot, k_dir, "spline");
    app::drawInterpolantPlot(
        spline, x.front(), x.back(), plot, "f(x)"
    );
    app::drawInterpolantPlot(
        osc_spline, x.front(), x.back(), plot, "f(x)sin(30x)"
    );
    plot.makePlot();

    std::cout << "VII 9.5a. Integral: " 
              << integr::calcIntegralTrapezoid( f2, h2) 
              << "\n";

    std::cout << "VII 9.13d. Integral: " 
              << integr::calcIntegralTrapezoid( f1, k_h1)
              << "\n";

    return 0;
} // main
