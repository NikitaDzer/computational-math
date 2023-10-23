#pragma once

#include <Eigen/Dense>

namespace app 
{

/**
 * 2 * x^2 + 5x - 3 = 0;
 */
std::pair< Eigen::Vector<double, 1>, Eigen::Vector<double, 1> >
solvePreparedNAE();

/**
 * sin(x) - y = 1.32;
 * cos(y) - x = -0.85;
 */
std::pair< Eigen::Vector<double, 2>, Eigen::Vector<double, 2> >
solvePreparedSNAE();

} // namespace app
