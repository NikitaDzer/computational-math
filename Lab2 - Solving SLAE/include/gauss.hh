#pragma once

#include <Eigen/Dense>

#include <cassert>
#include <utility>

namespace slae
{

namespace detail
{

template <int M>
std::pair<int, double>
getSubmatrixMaxElement( const Eigen::Matrix<double, M, M>& mat, 
                        int diag )
{
    assert( diag < M );

    int max_elem_row = diag;
    double abs_max_elem = 0;

    for ( int row = diag; row < M; row++ )
    {
        double elem = std::abs( mat( row, diag ) );

        if ( abs_max_elem < elem )
        {
            max_elem_row = row;
            abs_max_elem = elem;
        }
    }

    return { 
        max_elem_row,
        mat( max_elem_row, diag )
    };
} // getSubmatrixMaxElement 

template <int M>
void
makeGaussStraightStep( Eigen::Matrix<double, M, M>& mat,
                       Eigen::Vector<double, M>& vec,
                       int diag )
{
    auto [ max_elem_row, max_elem ] = getSubmatrixMaxElement( mat, diag );

    mat.row( diag ).swap( mat.row( max_elem_row ) );
    std::swap( vec( diag ), vec( max_elem_row ) );

    for ( int row = diag + 1; row < M; row++ )
    {
        double scaling_coefficient = mat( row, diag ) / max_elem;

        mat.row( row ) -= mat.row( diag ) * scaling_coefficient;
        vec( row ) -= vec( diag ) * scaling_coefficient;
    }
} // makeGaussStraightStep

template <int M>
void
makeGaussReverseStep( Eigen::Matrix<double, M, M>& mat,
                      Eigen::Vector<double, M>& vec,
                      int diag )
{
    double right_sum = vec( diag );

    for ( int column = diag + 1; column < M; column++ )
    {
        right_sum += -mat( diag, column ) * vec( column );
        mat( diag, column ) = 0;
    }

    double root = right_sum / mat( diag, diag );

    vec( diag ) = root;
    mat( diag, diag ) = 1;
} // makeGaussReverseStep

} // namespace detail

template <int M>
Eigen::Vector<double, M>
solveSLAEGauss( const Eigen::Matrix<double, M, M>& slae,
                const Eigen::Vector<double, M>& rhs )
{
    Eigen::Matrix<double, M, M> gauss_mat{ slae };
    Eigen::Vector<double, M> gauss_vec{ rhs };

    for ( int i = 0; i < M; i++ )
    {
        detail::makeGaussStraightStep( gauss_mat, gauss_vec, i );
    }

    for ( int i = M - 1; i >= 0; i-- )
    {
        detail::makeGaussReverseStep( gauss_mat, gauss_vec, i );
    }

    return gauss_vec;
} // solveSLAEGauss

} // namespace slae
