#pragma once

#include "plot.hh"

#include <Eigen/Dense>

#include <string_view>
#include <functional>


namespace app 
{

/**
 * Dimension of the matrix.
 */
constexpr int M = 100;

/**
 * Iterative method for solving SLAE.
 */
using IterativeMethod = std::function< Eigen::Vector<double, M>(
    const Eigen::Matrix<double, M, M>& slae,
    const Eigen::Vector<double, M>& rhs,
    int iterations
)>;

/**
 * Initialize a plot configuration.
 */
void
initIDPlot( visual::GNUPlot& plot,
            std::string_view image_name );

/**
 * Draw a plot with discrepancy on iterations number of a method.
 */
void
drawIDPlot( IterativeMethod iterative_method,
            uint32_t tests_number,
            Eigen::Matrix<double, M, M>& slae,
            Eigen::Vector<double, M>& rhs,
            visual::GNUPlot& plot,
            std::string_view method_name );

/**
 * Create a prepared SLAE equation.
 */
std::pair< Eigen::Matrix<double, M, M>, Eigen::Vector<double, M> >
createSLAEEquation();

} // namespace app
