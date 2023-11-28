#include "interp.hh"
#include "newton.hh"

#include <span>
#include <vector>
#include <cassert>
#include <algorithm>

namespace interp
{

namespace
{

double
calcBij( double b00, double b01, 
         double x0,  double x1)
{
    return (b01 - b00) / (x1 - x0);
} // calcBij

std::vector<double>
calcBi( std::span<const double> bprev, 
        std::span<const double> x,
        unsigned order)
{
    std::vector<double> bi{};
    bi.reserve( bprev.size() - 1);

    for ( unsigned i = 0; i < (bprev.size() - 1); i++ )
    {
        bi.push_back(
            calcBij( bprev[i], bprev[i + 1], x[i], x[i + order])
        );
    }

    return bi;
} // calcBi


std::vector< std::vector<double>>
calcBArray( std::span<const double> f,
            std::span<const double> x,
            unsigned max_order)
{
    std::vector< std::vector<double>> bi_array{};
    bi_array.reserve( max_order + 1);

    /**
     * Fill b0i coefficients with f(xi).
     */
    bi_array.resize( 1);
    bi_array[0].reserve( f.size());
    std::copy( f.begin(), f.end(), std::back_inserter( bi_array[0]));

    for ( unsigned i = 1; i < (max_order + 1); i++ )
    {
        bi_array.push_back( calcBi( bi_array[i - 1], x, i));
    }

    return bi_array;
} // calcBArray

std::vector<double>
calcB( std::span<const double> f,
       std::span<const double> x,
       unsigned preferred_order)
{
    unsigned max_order = std::max( 
        static_cast<unsigned>( x.size() - 1), 
        preferred_order
    );

    std::vector< std::vector<double>> bi_array{
        calcBArray(f, x, max_order)
    };

    std::vector<double> b{};
    b.reserve( max_order + 1);

    for ( unsigned i = 0; i < (max_order + 1); i++ )
    {
        b.push_back( bi_array[i][0]);
    }

    return b;
} // calcB

double
calcFi( std::span<const double> b,
        std::span<const double> x,
        double xi)
{
    double fi = 0;

    std::vector<double> multi{};
    multi.reserve( b.size());

    multi.push_back( 1);

    for ( unsigned i = 1; i < b.size(); i++ )
    {
        multi.push_back( multi[i - 1] * (xi - x[i - 1]));
    }

    for ( unsigned i = 0; i < b.size(); i++ )
    {
        fi += b[i] * multi[i];
    }

    return fi;
} // calcFi

} // namespace anonymous

double
calcDividedDiff( std::span<const double> f,
                 std::span<const double> x,
                 unsigned count,
                 unsigned i)
{
    assert( count >= 1);
    assert( f.size() == x.size());

    std::vector< std::vector<double>> bi_array{
        calcBArray( f, x, count - 1)
    };

    return bi_array[count - 1][i];
} // calcDividedDiff

Interpolant
makeInterpolantNewton( std::span<const double> f,
                       std::span<const double> x,
                       unsigned preferred_order)
{
    assert( f.size() >= 2);
    assert( f.size() == x.size());

    std::vector<double> b = calcB( f, x, preferred_order);

    return [b, x] (double xi) {
        return calcFi( b, x, xi);
    };
} // makeInterpolantNewton

double
calcInterpolantErrorNewton( std::span<const double> f,
                            std::span<const double> x,
                            unsigned order)
{
    assert( f.size() >= 2);
    assert( f.size() == x.size());
    assert( order + 1 < f.size());

    std::vector<double> b = calcB( f, x, order);
    double b0 = std::abs( b[b.size() - 1]);

    double max = 0;

    for ( unsigned i = 0; i < order; i++ )
    {
        double xi = x[i] + (x[i + 1] - x[i]) / 2;
        double multi = 1;

        for ( unsigned j = 0; j < (order + 1); j++ )
        {
            multi *= (xi - x[i]);
        }

        max = std::max( max, std::abs( multi));
    }

    return b0 * max;
} // calcInterpolantErrorNewton

} // namespace interp
