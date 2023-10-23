#pragma once

#include "snae.hh"
#include "utils.hh"

namespace snae
{

template <int M>
Eigen::Vector<double, M>
solveSMI( FunctionsVector<M>& F,
          const Eigen::Vector<double, M>& x0,
          int n_iterations )
{
    Eigen::Vector<double, M> x{ x0 };
    
    for ( int i = 0; i < n_iterations; i++ )
    {
        x = utils::calcFVector<M>( F, x );
    }

    return x;
} // solveSMI

template <int M>
Eigen::Vector<double, M>
solveSMI( FunctionsVector<M>& F,
          const Eigen::Vector<double, M>& x0,
          double epsilon )
{
    utils::IterativeMethod<M> method = [ & ]( const Eigen::Vector<double, M>& x )
    {
        return solveSMI<M>( F, x, 1 );
    };

    return utils::solveWithIterativeMethod<M>( method, x0, epsilon );
} // solveSMI

} // namespace snae
