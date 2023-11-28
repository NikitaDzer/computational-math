#include "lsm.hh"
#include "gauss.hh"
#include "interp.hh"

#include <Eigen/Dense>

#include <span>
#include <cmath>
#include <vector>
#include <cassert>
#include <numeric>

namespace interp
{

namespace
{

Eigen::MatrixXd
createLsmSLAE( std::span<const double> x,
               unsigned order)
{
    Eigen::MatrixXd matrix{};
    matrix.resize( order + 1, order + 1);

    unsigned coeffs_num = order * 2 + 1;

    std::vector<double> coeffs{};
    coeffs.reserve( coeffs_num);

    for ( unsigned power = 0; power < coeffs_num; power++ )
    {
        double coeff = std::accumulate( x.begin(), x.end(), 0.0, 
            [power]( double acc, double value) {
                return acc + std::pow( value, power);
            }
        );

        coeffs.push_back( coeff);
    }

    for ( unsigned row = 0; row < (order + 1); row++ )
    {
        for ( unsigned col = 0; col < (order + 1); col++ )
        {
            matrix( row, col) = coeffs[ coeffs_num - row - col - 1 ];
        }
    }

    return matrix;
} // createLsmSLAE

Eigen::VectorXd
createLsmRhs( std::span<const double> f,
              std::span<const double> x,
              unsigned order)
{
    Eigen::VectorXd vector{};
    vector.resize( order + 1);

    for ( unsigned row = 0; row < (order + 1); row++ )
    {
        double coeff = std::transform_reduce( 
            f.begin(), f.end(), x.begin(), 0.0,
            std::plus{},
            [row, order]( double yi, double xi) {
                return yi * std::pow( xi, order - row);
            }
        );

        vector( row) = coeff;
    }

    return vector;
} // createLsmRhs

Eigen::VectorXd
solveLsmSLAE( std::span<const double> f,
              std::span<const double> x,
              unsigned order)
{
    Eigen::MatrixXd slae{ createLsmSLAE( x, order)};
    Eigen::VectorXd rhs{ createLsmRhs( f, x, order)};

    return slae::solveSLAEGauss( slae, rhs);
} // solveLsmSLAE

} // namespace anonymous

Interpolant
makeInterpolantLsm( std::span<const double> f,
                    std::span<const double> x,
                    unsigned order)
{
    assert( f.size() >= 2);
    assert( f.size() == x.size());
    assert( order >= 1);

    Eigen::VectorXd coeffs = solveLsmSLAE( f, x, order);

    return [coeffs]( double xi) {
        double yi = 0;

        for ( unsigned i = 0; i < coeffs.size(); i++ )
        {
            yi += coeffs[i] * std::pow( xi, coeffs.size() - i - 1);
        }

        return yi;
    };
} // makeInterpolantLsm

double
calcInterpolantErrorLsm( Interpolant interpolant,
                         std::span<const double> f,
                         std::span<const double> x)
{
    assert( interpolant);

    double sum = 0;

    for ( unsigned i = 0; i < f.size(); i++ )
    {
        sum += std::pow( f[i] - interpolant( x[i]), 2);
    }

    return std::sqrt( sum / f.size());
} // calcInterpolantErrorLsm

} // namespace interp
