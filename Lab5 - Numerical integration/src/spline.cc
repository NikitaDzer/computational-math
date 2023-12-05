#include "interp.hh"
#include "spline.hh"
#include "gauss.hh"
#include "newton.hh"

#include <Eigen/Dense>

#include <span>
#include <cmath>
#include <utility>
#include <cassert>

namespace interp
{

namespace
{

double
calcMu( std::span<const double> x,
        unsigned i)
{
    return (x[i] - x[i-1]) / (x[i+1] - x[i-1]);
} // calcMu

double
calcLambda( std::span<const double> x,
            unsigned i)
{
    return 1 - calcMu( x, i);
} // calcLambda

Eigen::MatrixXd
createSplineSLAE( std::span<const double> x)
{
    Eigen::MatrixXd matrix{};
    matrix.resize( x.size(), x.size());
    matrix.setZero();

    matrix( 0, 0) = 2;
    matrix( 0, 1) = 0;

    matrix( x.size() - 1, x.size() - 2) = 0;
    matrix( x.size() - 1, x.size() - 1) = 2;

    for ( unsigned i = 1; i < (x.size() - 1); i++ )
    {
        matrix( i, i - 1) = calcMu( x, i);
        matrix( i, i) = 2;
        matrix( i, i + 1) = calcLambda( x, i);
    }

    return matrix;
} // createSplineSLAE

Eigen::VectorXd
createSplineRhs( std::span<const double> f,
                 std::span<const double> x)
{
    Eigen::VectorXd vector{};
    vector.resize( x.size());
    vector.setZero();

    vector( 0) = 0;
    vector( x.size() - 1) = 0;

    for ( unsigned i = 1; i < (x.size() - 1); i++ )
    {
        vector( i) = 6 * calcDividedDiff( f, x, 3, i - 1);
    }

    return vector;
} // createSplineRhs

Eigen::VectorXd
solveSplineSLAE( std::span<const double> f,
                 std::span<const double> x)
{
    Eigen::MatrixXd slae{ createSplineSLAE( x)};
    Eigen::VectorXd rhs{ createSplineRhs( f, x)};

    return slae::solveSLAEGauss( slae, rhs);
} // solveSplineSLAE

} // namespace anonymous 

struct Spline
{
    double x0 = 0;
    double x1 = 0;

    double y0 = 0;
    double y1 = 0;

    double m0 = 0;
    double m1 = 0;

    bool contains( double xi) const
    { 
        return x0 <= xi && xi <= x1; 
    } // contains

    double operator () ( double xi) const
    {
        double h1 = x1 - x0;

        double summand1 = (m0 / (6*h1)) * std::pow( x1 - xi, 3);
        double summand2 = (m1 / (6*h1)) * std::pow( xi - x0, 3);
        double summand3 = ((y0 - m0 * h1 * h1 / 6) / h1) * (x1 - xi);
        double summand4 = ((y1 - m1 * h1 * h1 / 6) / h1) * (xi - x0);

        return summand1 + summand2 + summand3 + summand4;
    } // operator ()

}; // struct Spline

class Splines
{

private:
    std::vector<Spline> splines;

private:
    const auto findSpline( double xi) const
    {
        return std::find_if( 
            splines.begin(), 
            splines.end(), 
            [xi]( const auto& spline) { 
                return spline.contains( xi); 
            }
        );
    } // findSpline

public:
    Splines( std::span<const double> f,
             std::span<const double> x)
    {
        assert( f.size() >= 2);
        assert( f.size() == x.size());

        Eigen::VectorXd m = solveSplineSLAE( f, x);

        for ( unsigned i = 0; i < (x.size() - 1); i++ )
        {
            splines.push_back({
                x[i], x[i + 1],
                f[i], f[i + 1],
                m[i], m[i + 1]
            });
        }
    } // Splines

    bool contains( double xi) const
    {
        return findSpline( xi) != splines.end();
    } // contains

    double operator () ( double xi) const
    {
        assert( splines.size() >= 1);

        const auto spline_iter = findSpline( xi);

        if ( spline_iter != splines.end() )
        {
            return (*spline_iter)( xi);
        }

        if ( xi < splines[0].x0 )
        {
            return splines[0]( xi);
        }

        if ( xi > splines[splines.size() - 1].x1 )
        {
            return splines[splines.size() - 1]( xi);
        }

        return 0;
    } // operator ()

}; // class Splines

Interpolant
makeInterpolantSpline( std::span<const double> f,
                       std::span<const double> x)
{
    Splines splines{ f, x};

    return [splines] ( double xi) {
        return splines( xi);
    };
} // makeInterpolantSpline

} // namespace interp
