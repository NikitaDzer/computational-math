#include "app.hh"
#include "plot.hh"
#include "smi.hh"
#include "utils.hh"
#include "newton.hh"

#include <format>
#include <string>
#include <iostream>

int 
main()
{
    const std::string k_dir{ "assets/" };

    visual::GNUPlot plot_nae{};
    app::initIDPlot( plot_nae, k_dir, "NAE" );

    visual::GNUPlot plot_snae{};
    app::initIDPlot( plot_snae, k_dir, "SNAE" );

    auto [ F_nae_smi, F_nae_newton, J_nae_newton ] = app::getPreparedNAE();
    Eigen::Vector<double, 1> x0_nae{ 0 };

    auto [ F_snae_smi, F_snae_newton, J_snae_newton ] = app::getPreparedSNAE();
    Eigen::Vector<double, 2> x0_snae{ 2.0, -1.0 };

    snae::utils::IterativeMethod<1> nae_smi =
        [&]( const Eigen::Vector<double, 1>& x )
        {
            return snae::solveSMI<1>( F_nae_smi, x, 1 );
        };
    snae::utils::IterativeMethod<1> nae_newton =
        [&]( const Eigen::Vector<double, 1>& x )
        {
            return snae::solveNewton<1>( F_nae_newton, J_nae_newton, x, 1 );
        };

    snae::utils::IterativeMethod<2> snae_smi = 
        [&]( const Eigen::Vector<double, 2>& x )
        {
            return snae::solveSMI<2>( F_snae_smi, x, 1 );
        };
    snae::utils::IterativeMethod<2> snae_newton =
        [&]( const Eigen::Vector<double, 2>& x )
        {
            return snae::solveNewton<2>( F_snae_newton, J_snae_newton, x, 1 );
        };

    app::drawIDPlot(
        nae_smi,
        80,
        x0_nae,
        plot_nae,
        "NAE SMI"
    );
    app::drawIDPlot(
        nae_newton,
        80,
        x0_nae,
        plot_nae,
        "NAE Newton"
    );

    app::drawIDPlot(
        snae_smi,
        30,
        x0_snae,
        plot_snae,
        "SNAE SMI"
    );
    app::drawIDPlot(
        snae_newton,
        30,
        x0_snae,
        plot_snae,
        "SNAE Newton"
    );

    plot_nae.makePlot();
    plot_snae.makePlot();

    Eigen::Vector<double, 1> root_nae_smi =
        snae::solveSMI<1>( F_nae_smi, x0_nae, 1e-5 );
    Eigen::Vector<double, 1> root_nae_newton = 
        snae::solveNewton<1>( F_nae_newton, J_nae_newton, x0_nae, 1e-5 );

    Eigen::Vector<double, 2> root_snae_smi =
        snae::solveSMI<2>( F_snae_smi, x0_snae, 1e-5 );
    Eigen::Vector<double, 2> root_snae_newton = 
        snae::solveNewton<2>( F_snae_newton, J_snae_newton, x0_snae, 1e-5 );

    std::string nae_format = std::format(
        "NAE SMI:     x = {}\n"
        "NAE Newton:  x = {}\n",
        root_nae_smi[ 0 ],
        root_nae_newton[ 0 ]
    );

    std::string snae_format = std::format(
        "SNAE SMI:    x = {}, y = {}\n"
        "SNAE Newton: x = {}, y = {}\n",
        root_snae_smi[ 0 ], root_snae_smi[ 1 ],
        root_snae_newton[ 0 ], root_snae_newton[ 1 ]
    );

    std::cout << nae_format + snae_format << std::endl;

    return 0;
} // main
