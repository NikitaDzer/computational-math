#pragma once

#include "snae.hh"

#include <Eigen/Dense>

#include <cmath>
#include <functional>

namespace snae::utils
{

template<int M>
using IterativeMethod = std::function<
    Eigen::Vector<double, M>( const Eigen::Vector<double, M>& x0 )
>;

template<int M>
Eigen::Vector<double, M>
calcFVector( FunctionsVector<M>& F,
             const Eigen::Vector<double, M>& x )
{
    Eigen::Vector<double, M> Fv{};

    for ( int i = 0; i < M; i++ )
    {
        Fv( i ) = F[ i ]( x ); 
    }

    return Fv;
} // calcFVector

template<int M>
double
calcVectorNorm( const Eigen::Vector<double, M>& x )
{
    double norm = 0.0;

    for ( int i = 0; i < M; i++ )
    {
        norm += std::abs( x[ i ] );
    }

    return norm;
} // calcVectorNorm

template<int M>
Eigen::Vector<double, M>
solveWithIterativeMethod( IterativeMethod<M>& method,
                          const Eigen::Vector<double, M> x0,
                          double epsilon )
{
    Eigen::Vector<double, M> x{ x0 };
    Eigen::Vector<double, M> x_prev{ x0 };
    Eigen::Vector<double, M> delta{};

    double delta_norm = 0.0;

    do
    {
        x = method( x );
        delta = x - x_prev;
        x_prev = x;

        delta_norm = utils::calcVectorNorm( delta );
    } while ( delta_norm > epsilon );

    return x;
} // solveWithIterativeMethod

} // namespace snae::utils
