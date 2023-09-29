#pragma once

#include "utils.hh"

#include <Eigen/Dense>

namespace slae
{

constexpr double k_relaxation_param_optimal = 1.5;

namespace detail
{

template <int M>
Eigen::Matrix<double, M, M>
calcRelaxationR( const Eigen::Matrix<double, M, M>& L,
                 const Eigen::Matrix<double, M, M>& D,
                 const Eigen::Matrix<double, M, M>& U )
{
    constexpr double k_w = k_relaxation_param_optimal;
    return -(D + k_w * L).inverse() * ((k_w - 1) * D + k_w * U);
} // calcRelaxationR

template <int M>
Eigen::Vector<double, M>
calcRelaxationF( const Eigen::Matrix<double, M, M>& L,
                 const Eigen::Matrix<double, M, M>& D,
                 const Eigen::Vector<double, M>& f )
{
    constexpr double k_w = k_relaxation_param_optimal;
    return k_w * (D + k_w * L).inverse() * f;
} // calcRelaxationF

} // namespace detail

template <int M>
Eigen::Vector<double, M>
solveSLAERelaxation( const Eigen::Matrix<double, M, M>& slae,
                     const Eigen::Vector<double, M>& rhs,
                     int iterations )
{
    auto [ L, D, U ] = utils::MatricesLDU{ slae };

    Eigen::Matrix<double, M, M> R{ detail::calcRelaxationR( L, D, U ) };
    Eigen::Vector<double, M> F{ detail::calcRelaxationF( L, D, rhs ) };

    Eigen::Vector<double, M> uk{ rhs };

    for ( int i = 0; i < iterations; i++ )
    {
        uk = R * uk + F;
    }

    return uk;
} // solveSLAERelaxation

} // namespace slae
