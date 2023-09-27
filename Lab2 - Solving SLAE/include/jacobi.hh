#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include "utils.hh"

namespace slae
{

namespace detail
{

template <int M>
Eigen::Matrix<double, M, M>
calcJacobiR( const Eigen::Matrix<double, M, M>& L,
             const Eigen::Matrix<double, M, M>& D,
             const Eigen::Matrix<double, M, M>& U )
{
    return -D.inverse() * (L + U);
} // calcJacobiR

template <int M>
Eigen::Vector<double, M>
calcJacobiF( const Eigen::Matrix<double, M, M>& D,
             const Eigen::Vector<double, M>& f )
{
    return D.inverse() * f;
} // calcJacobiF

} // namespace detail

template <int M>
Eigen::Vector<double, M>
solveSLAEJacobi( const Eigen::Matrix<double, M, M>& slae,
                 const Eigen::Vector<double, M>& rhs,
                 int iterations )
{
    auto [ L, D, U ] = utils::MatricesLDU{ slae };

    Eigen::Matrix<double, M, M> R{ detail::calcJacobiR( L, D, U ) };
    Eigen::Vector<double, M> F{ detail::calcJacobiF( D, rhs ) };

    Eigen::Vector<double, M> uk{ F };

    for ( int i = 0; i < iterations; i++ )
    {
        uk = utils::calcIterativeMethodExpression( R, F, uk );
    }

    return uk;
} // solveSLAEJacobi

} // namespace slae
