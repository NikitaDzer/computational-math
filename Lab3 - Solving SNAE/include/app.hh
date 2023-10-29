#pragma once

#include "snae.hh"
#include "utils.hh"
#include "plot.hh"

#include <Eigen/Dense>

#include <tuple>
#include <vector>
#include <utility>

namespace app 
{

/**
 * Dependence of the (Xn - X(n-1)) norm
 * on the number of iterations.
 */
using ID = std::pair<double, double>;

/**
 * NAE's functions vector and Jacobi matrix.
 */
using FNAE = snae::FunctionsVector<1>;
using JNAE = snae::FunctionsMatrix<1>;

/**
 * SNAE's functions vector and Jacobi matrix.
 */
using FSNAE = snae::FunctionsVector<2>;
using JSNAE = snae::FunctionsMatrix<2>;

namespace detail
{

template<int M>
std::vector<ID>
runIDCalculation( snae::utils::IterativeMethod<M>& iterative_method,
                  uint32_t tests_number,
                  const Eigen::Vector<double, M>& x0 )
{
    std::vector<ID> results{};
    results.reserve( tests_number );

    Eigen::Vector<double, M> x{ x0 };
    Eigen::Vector<double, M> prev_x{};

    for ( uint32_t i = 0; i < tests_number; i++ )
    {
        prev_x = x;
        x = iterative_method( x );

        double delta = snae::utils::calcVectorNorm<M>( x - prev_x );
        if ( delta == 0.0 )
        {
            delta = 1e-16;
        }

        results.push_back( { i, delta } );
    }

    return results;
} // runIDCalculation

} // namespace detail

template<int M>
void
drawIDPlot( snae::utils::IterativeMethod<M>& iterative_method,
            uint32_t tests_number,
            const Eigen::Vector<double, M>& x0,
            visual::GNUPlot& plot,
            std::string_view method_name )
{
    std::vector<ID> data = detail::runIDCalculation(
        iterative_method, tests_number, x0
    );
    plot.drawPlot( data, method_name );
} // drawIDPlot

void
initIDPlot( visual::GNUPlot& plot,
            std::string image_dir,
            std::string plot_name );

/**
 * 2 * x^2 + 5x - 3 = 0;
 *
 * Returns functions vector for smi,
 *         functions vector for newton's method,
 *         Jacobi matrix for newton's method.
 */
std::tuple< FNAE, FNAE, JNAE >
getPreparedNAE();

/**
 * sin(x) - y = 1.32;
 * cos(y) - x = -0.85;
 *
 * Returns functions vector for smi,
 *         functions vector for newton's method,
 *         Jacobi matrix for newton's method.
 */
std::tuple< FSNAE, FSNAE, JSNAE >
getPreparedSNAE();

} // namespace app
