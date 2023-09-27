#pragma once

#include "utils.hh"
#include <Eigen/Dense>
#include <iostream>

namespace slae
{

namespace detail
{


template <int M>
Eigen::Vector<double, M>
solveSLAELyb( const Eigen::Matrix<double, M, M>& L,
              const Eigen::Vector<double, M>& b )
{
    Eigen::Vector<double, M> y{};

    for ( int i = 0; i < M; i++ )
    {
        double right_sum = b( i );

        for ( int j = 0; j < i; j++ )
        {
            right_sum += -L( i, j ) * y( j );
        }
        
        y( i ) = right_sum;
    }

    return y;
} // solveSLAELyb

template <int M>
Eigen::Vector<double, M>
solveSLAEUxy( const Eigen::Matrix<double, M, M>& U,
              const Eigen::Vector<double, M>& y )
{
    Eigen::Vector<double, M> x{};

    for ( int i = M - 1; i >= 0; i-- )
    {
        double right_sum = y( i );

        for ( int j = i + 1; j < M; j++ )
        {
            right_sum += -U( i, j ) * x( j );
        }
        
        x( i ) = right_sum / U( i, i );
    }

    return x;
} // solveSLAEUxy

} // namespace detail

template <int M>
Eigen::Vector<double, M>
solveSLAELU( const Eigen::Matrix<double, M, M>& slae,
             const Eigen::Vector<double, M>& rhs )
{
    auto [ L, U ] = utils::MatricesLU{ slae };

    Eigen::Vector<double, M> y{ detail::solveSLAELyb( L, rhs ) };
    Eigen::Vector<double, M> x{ detail::solveSLAEUxy( U, y ) };

    return x;
} // solveSLAELU

} // namespace slae
