#pragma once

#include "utils.hh"

#include <Eigen/Dense>

namespace slae
{

namespace detail
{

template <int M>
Eigen::Matrix<double, M, M>
calcSeidelR( const Eigen::Matrix<double, M, M>& L,
             const Eigen::Matrix<double, M, M>& D,
             const Eigen::Matrix<double, M, M>& U )
{
    return -(L + D).inverse() * U;
} // calcSeidelR

template <int M>
Eigen::Vector<double, M>
calcSeidelF( const Eigen::Matrix<double, M, M>& L,
             const Eigen::Matrix<double, M, M>& D,
             const Eigen::Vector<double, M>& f )
{
    return (L + D).inverse() * f;
} // calcSeidelF

} // namespace detail

template <int M>
Eigen::Vector<double, M>
solveSLAESeidel( const Eigen::Matrix<double, M, M>& slae,
                 const Eigen::Vector<double, M>& rhs,
                 int iterations )
{
    auto [ L, D, U ] = utils::MatricesLDU{ slae };

    Eigen::Matrix<double, M, M> R{ detail::calcSeidelR( L, D, U ) };
    Eigen::Vector<double, M> F{ detail::calcSeidelF( L, D, rhs ) };

    Eigen::Vector<double, M> uk{ F };

    for ( int i = 0; i < iterations; i++ )
    {
        uk = utils::calcIterativeMethodExpression( R, F, uk );
    }

    return uk;
} // solveSLAESeidel

} // namespace slae
