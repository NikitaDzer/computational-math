#include "app.hh"

#include "relaxation.hh"
#include "jacobi.hh"
#include "seidel.hh"
#include "gauss.hh"
#include "lu.hh"

#include "utils.hh"

#include <Eigen/Dense>

#include <string>
#include <cstdint>

int 
main()
{
    constexpr uint32_t k_tests_number = 12;
    std::string k_dir{ "assets/" };

    visual::GNUPlot plot{};
    app::initIDPlot( plot, k_dir + "graph" );

    auto [ slae, rhs ] = app::createSLAEEquation();

    app::drawIDPlot(
        slae::solveSLAEJacobi<app::M>,
        k_tests_number,
        slae,
        rhs,
        plot,
        "Jacobi"
    );

    app::drawIDPlot(
        slae::solveSLAESeidel<app::M>,
        k_tests_number,
        slae,
        rhs,
        plot,
        "Seidel"
    );

    app::drawIDPlot(
        slae::solveSLAERelaxation<app::M>,
        k_tests_number,
        slae,
        rhs,
        plot,
        "Relaxation"
    );

    plot.makePlot();

    return 0;
} // main
