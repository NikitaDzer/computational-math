#include "snae.hh"
#include "smi.hh"
#include "newton.hh"

#include <Eigen/Dense>

#include <cmath>
#include <utility>

namespace app
{

std::pair< Eigen::Vector<double, 1>, Eigen::Vector<double, 1> >
solvePreparedNAE()
{
    Eigen::Vector<double, 1> x0{ 0 };

    snae::FunctionsVector<1> F{};
    F[ 0 ] = []( const Eigen::Vector<double, 1>& x )
    {
        return -std::sqrt( 0.5 * (3 - 5 * x[ 0 ]) );
    };

    Eigen::Vector<double, 1> root_smi = snae::solveSMI<1>( F, x0, 10e-4 );

    F[ 0 ] = []( const Eigen::Vector<double, 1>& x )
    {
        return 2 * x[ 0 ] * x[ 0 ] + 5 * x[ 0 ] - 3;
    };

    snae::FunctionsMatrix<1> J{};
    J[ 0 ][ 0 ] = []( const Eigen::Vector<double, 1>& x )
    {
        return 4 * x[ 0 ] - 5;
    };

    Eigen::Vector<double, 1> root_newton = snae::solveNewton<1>( F, J, x0, 10e-4 );

    return { root_smi, root_newton };
} // solvePreparedNAE

std::pair< Eigen::Vector<double, 2>, Eigen::Vector<double, 2> >
solvePreparedSNAE()
{
    Eigen::Vector<double, 2> x0{ 2.0, -1.0 };

    snae::FunctionsVector<2> F{};
    F[ 0 ] = []( const Eigen::Vector<double, 2>& x )
    {
        return std::cos( x[ 1 ] ) + 0.85;
    };
    F[ 1 ] = []( const Eigen::Vector<double, 2>& x )
    {
        return std::sin( x[ 0 ] ) - 1.32;
    };

    Eigen::Vector<double, 2> root_smi = snae::solveSMI<2>( F, x0, 10e-6 );

    F[ 0 ] = []( const Eigen::Vector<double, 2>& x )
    {
        return std::cos( x[ 1 ] ) - x[ 0 ] + 0.85;
    };
    F[ 1 ] = []( const Eigen::Vector<double, 2>& x )
    {
        return std::sin( x[ 0 ] ) - x[ 1 ] - 1.32;
    };

    snae::FunctionsMatrix<2> J{};
    J[ 0 ][ 0 ] = []( const Eigen::Vector<double, 2>& x )
    {
        return -1;
    };
    J[ 0 ][ 1 ] = []( const Eigen::Vector<double, 2>& x )
    {
        return -std::sin( x[ 1 ] );
    };
    J[ 1 ][ 0 ] = []( const Eigen::Vector<double, 2>& x )
    {
        return std::cos( x[ 0 ] );
    };
    J[ 1 ][ 1 ] = []( const Eigen::Vector<double, 2>& x )
    {
        return -1;
    };

    Eigen::Vector<double, 2> root_newton = snae::solveNewton<2>( F, J, x0, 10e-6 );

    return { root_smi, root_newton };
} // solvePreparedSNAE

} // namespace app
