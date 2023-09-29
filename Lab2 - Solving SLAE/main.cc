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
#include <iostream>

int 
main()
{
    constexpr uint32_t k_tests_number = 12;
    std::string k_dir{ "assets/" };

    auto [ slae, rhs ] = app::createSLAEEquation();

    Eigen::Vector<double, app::M> gauss_root = 
        slae::solveSLAEGauss( slae, rhs );

    Eigen::Vector<double, app::M> lu_root = 
        slae::solveSLAELU( slae, rhs );

    double gauss_discrepancy = 
        slae::utils::calcSolutionDiscrepancy( slae, rhs, gauss_root );
        
    double lu_discrepancy = 
        slae::utils::calcSolutionDiscrepancy( slae, rhs, lu_root );

    std::cout << "Gauss discrepancy: " << gauss_discrepancy << "\n";
    std::cout << "LU    discrepancy: " << lu_discrepancy << "\n";

    visual::GNUPlot plot{};
    app::initIDPlot( plot, k_dir + "graph" );

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
