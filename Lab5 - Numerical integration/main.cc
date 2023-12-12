#include "app.hh"
#include "plot.hh"
#include "interp.hh"
#include "spline.hh"
#include "integral.hh"

#include <cmath>
#include <string>
#include <format>
#include <iostream>

namespace
{

void
runIntegration( std::span<const double> f,
                double h,
                std::string_view description)
{
    double trapezoid = integr::calcIntegralTrapezoid( f, h);
    double simpson = integr::calcIntegralSimpson( f, h);
    double richardson = integr::calcIntegralRichardson( 
        f, h, integr::calcIntegralTrapezoid, 2
    );
    double delta = std::abs( richardson - simpson);

    std::string format = std::format(
        "{} Integral:           \n"
        "\t trapezoid:  {}      \n"
        "\t simpson:    {}      \n"
        "\t richardson: {}      \n"
        "\t delta:      {}      \n",
        description, trapezoid, simpson, richardson, delta
    );

    std::cout << format;
} // runIntegration

} // namespace anonymous

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

    runIntegration( f2, h2, "VII 9.5a.");
    runIntegration( f1, k_h1, "VII 9.13d.");

    return 0;
} // main
