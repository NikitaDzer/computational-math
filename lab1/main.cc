#include "app.hh"
#include "plot.hh"
#include "inputfn.hh"
#include "approximation.hh"

#include <vector>
#include <iostream>

int 
main()
{
    constexpr double k_x = 2.0;
    std::string k_dir{ "assets/" };

    visual::GNUPlot plot{};
    app::initDerivativeErrorPlot( plot );

    app::makeDerivativeErrorPlot(
        app::derivativeSinM,
        app::sinM,
        k_x,
        plot,
        "sin(x^2)",
        k_dir + "error1"
    );

    app::makeDerivativeErrorPlot(
        app::derivativeCosSin,
        app::cosSin,
        k_x,
        plot,
        "cos(sin(x))",
        k_dir + "error2"
    );

    app::makeDerivativeErrorPlot(
        app::derivativeExpSinCos,
        app::expSinCos,
        k_x,
        plot,
        "exp(sin(cos(x)))",
        k_dir + "error3"
    );

    app::makeDerivativeErrorPlot(
        app::derivativeLnM,
        app::lnM,
        k_x,
        plot,
        "ln(x+3)",
        k_dir + "error4"
    );

    app::makeDerivativeErrorPlot(
        app::derivativeSqrtM,
        app::sqrtM,
        k_x,
        plot,
        "sqrt(x+3)",
        k_dir + "error5"
    );

    return 0;
}
