#pragma once

#include <Eigen/Dense>

#include <cmath>

namespace slae::utils
{

template <int M>
struct MatricesLU
{
    MatricesLU( const Eigen::Matrix<double, M, M>& mat ):
        L{},
        U{}
    {
        L.setZero();
        U = mat;

        for ( int l_diag = 0; l_diag < M; l_diag++ )
        {
            L( l_diag, l_diag ) = 1;
        }

        for ( int mat_diag = 0; mat_diag < M; mat_diag++ )
        {
            for ( int u_row = mat_diag + 1; u_row < M; u_row++ )
            {
                double scaling_coefficient = 
                    U( u_row, mat_diag ) / U( mat_diag, mat_diag );

                U.row( u_row ) -= U.row( mat_diag ) * scaling_coefficient;
                L( u_row, mat_diag ) = scaling_coefficient;
            }

        }
    } // MatricesLU

    Eigen::Matrix<double, M, M> L;
    Eigen::Matrix<double, M, M> U;
}; // struct MatricesLU

template <int M>
struct MatricesLDU
{
    MatricesLDU( const Eigen::Matrix<double, M, M>& mat ):
        L{},
        D{},
        U{}
    {
        L.setZero();
        D.setZero();
        U.setZero();

        for ( int i = 1; i < M; i++ )
        {
            for ( int j = 0; j < i; j++ )
            {
                L( i, j ) = mat( i, j ); 
            }
        }

        for ( int diag = 0; diag < M; diag++ )
        {
            D( diag, diag ) = mat( diag, diag ); 
        }

        for ( int i = 0; i < M; i++ )
        {
            for ( int j = i + 1; j < M; j++ )
            {
                U( i, j ) = mat( i, j );
            }
        }
    } // MatricesLDU

    Eigen::Matrix<double, M, M> L;
    Eigen::Matrix<double, M, M> D;
    Eigen::Matrix<double, M, M> U;
}; // struct LDU

template <int M>
double
calcSolutionDiscrepancy( const Eigen::Matrix<double, M, M>& slae,
                         const Eigen::Vector<double, M>& rhs,
                         const Eigen::Vector<double, M>& root )
{
    Eigen::Vector<double, M> vec_dicrepancy{ slae * root - rhs };
    double discrepancy = 0.0;

    for ( int i = 0; i < M; i++ )
    {
        discrepancy += std::abs( vec_dicrepancy( i ) );
    }

    return discrepancy;
} // calcSolutionDiscrepancy

template <int M>
Eigen::Vector<double, M>
solveSLAEWithIterativeMethod( const Eigen::Matrix<double, M, M>& slae,
                              const Eigen::Matrix<double, M, M>& rhs,
                              const Eigen::Matrix<double, M, M>& R,
                              const Eigen::Vector<double, M>& F,
                              const Eigen::Vector<double, M>& u0,
                              double needed_discrepancy )
{
    Eigen::Vector<double, M> uk{ u0 };

    while ( calcSolutionDiscrepancy( slae, rhs, uk ) > needed_discrepancy )
    {
        uk = R * uk + F;
    }

    return uk;
} // solveSLAEWithIterativeMethod

} // namespace slae::utils
