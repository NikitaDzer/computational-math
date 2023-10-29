#pragma once

#include "snae.hh"
#include "utils.hh"

namespace snae
{

namespace detail
{

template<int M>
Eigen::Matrix<double, M, M>
calcJacobiMatrix( FunctionsMatrix<M>& J,
                  const Eigen::Vector<double, M>& x )
{
    Eigen::Matrix<double, M, M> Jacobi{};

    for ( int i = 0; i < M; i++ )
    {
        for ( int j = 0; j < M; j++ )
        {
            Jacobi( i, j ) = J[ i ][ j ]( x );
        }
    }

    return Jacobi;
} // calcJacobiMatrix

} // namespace detail

template<int M>
Eigen::Vector<double, M>
solveNewton( FunctionsVector<M>& F,
             FunctionsMatrix<M>& J,
             const Eigen::Vector<double, M>& x0,
             int n_iterations )
{
    Eigen::Vector<double, M> x{ x0 };

    for ( int i = 0; i < n_iterations; i++ )
    {
        for ( int j = 0; j < M; j++ )
        {
            Eigen::Vector<double, M> Fv = utils::calcFVector<M>( F, x );
            Eigen::Matrix<double, M, M> Jacobi = 
                detail::calcJacobiMatrix<M>( J, x );
            
            x = x - Jacobi.inverse() * Fv;
        }
    }

    return x;
} // solveNewton

template<int M>
Eigen::Vector<double, M>
solveNewton( FunctionsVector<M>& F,
             FunctionsMatrix<M>& J,
             const Eigen::Vector<double, M>& x0,
             double epsilon )
{
    utils::IterativeMethod<M> method = [ & ]( const Eigen::Vector<double, M>& x )
    {
        return solveNewton<M>( F, J, x, 1 );
    };

    return utils::solveWithIterativeMethod<M>( method, x0, epsilon );
} // solveNewton

} // namespace snae
