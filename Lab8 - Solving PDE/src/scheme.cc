#include "grid.hh"

namespace
{

/**
 *       to calculate
 *            │
 *            ▼
 *           ┌─┐
 *           │1│
 *           └▲┘
 *            │
 *    ┌─┐    ┌─┐
 *    │2◄────►3│
 *    └─┘    └─┘
 *
 */
double
calcDSLeftCorner( double u2, 
                  double u3,
                  double f,
                  double h,
                  double a)
{
    return f*a - a/h * (u3 - u2) + u3;
} // calcDSLeftCorner

/**
 *       to calculate
 *            │
 *            ▼
 *           ┌─┐
 *           │1│
 *           └▲┘
 *            │
 *           ┌─┐    ┌─┐
 *           │3◄────►4│
 *           └─┘    └─┘
 *
 */
double
calcDSRightCorner( double u3,
                   double u4,
                   double f,
                   double h,
                   double a)
{
    return f*a - a/h * (u4 - u3) + u3;
} // calcDSRightCorner

/**
 *       to calculate
 *            │
 *            ▼
 *           ┌─┐
 *           │1│
 *           └▲┘
 *            │
 *    ┌─┐     │     ┌─┐
 *    │2◄─────┴─────►4│
 *    └─┘           └─┘
 *
 */
double
calcDSCentral( double u2,
               double u4,
               double f,
               double h,
               double a)
{
    return f*a - a/(2*h) * (u4 - u2) + 0.5 * (u4 + u2);
} // calcDSCentral

/**
 *       to calculate
 *            │
 *            ▼
 *           ┌─┐
 *           │1│
 *           └▲┘
 *            │
 *    ┌─┐    ┌─┐    ┌─┐
 *    │2◄────►3◄────►4│
 *    └─┘    └─┘    └─┘
 *
 */
double
calcDSLax( double u2,
           double u3,
           double u4,
           double f,
           double h,
           double a)
{
    return f*a + u3 - (u4-u2)*a/(2*h) + (u4-2*u3+u2)*(a*a)/(2*h*h);
} // calcDSLax

/**
 *       to calculate
 *            │
 *            ▼
 *    ┌─┐    ┌─┐
 *    │0◄────►1│
 *    └▲┘    └▲┘
 *     │      │
 *    ┌─┐    ┌─┐
 *    │2◄────►3│
 *    └─┘    └─┘
 *
 */
double
calcDSRectangle( double u0,
                 double u2,
                 double u3,
                 double f,
                 double h,
                 double a)
{
    return (2*f*h*a)/(h+a) - (u0-u2-u3)*h/(h+a) - (u3-u0-u2)*a/(h+a);
} // calcDSRectangle

} // namespace anonymous

namespace pde
{

double
schemeCentral( const pde::Grid& grid)
{
    auto [h, a] = grid.getPitch();

    double u2 = 0.0;
    double u3 = grid.get();
    double u4 = 0.0;

    auto [x, t] = grid.getCurrentNode();

    double f = grid.getFunction()( h*x, a*t);

    if ( grid.isCurrentNodeLeft() )
    {
        if ( !grid.hasLeftNeighbour() )
        {
            u4 = grid.get( 1);
            return calcDSRightCorner( u3, u4, f, h, a);
        }

        u2 = grid.getLeftNeighbour()->getRight( t);
        u4 = grid.get( 1);
    } else if ( grid.isCurrentNodeRight() )
    {
        if ( !grid.hasRightNeighbour() )
        {
            u2 = grid.get( -1);
            return calcDSLeftCorner( u2, u3, f, h, a);
        }

        u2 = grid.get( -1);
        u4 = grid.getRightNeighbour()->getLeft( t);
    } else
    {
        u2 = grid.get( -1);
        u4 = grid.get( 1);
    }

    return calcDSCentral( u2, u4, f, h, a);
} // schemeCentral

double
schemeLax( const pde::Grid& grid)
{
    auto [h, a] = grid.getPitch();

    double u2 = 0.0;
    double u3 = grid.get();
    double u4 = 0.0;

    auto [x, t] = grid.getCurrentNode();

    double f = grid.getFunction()( h*x, a*t);

    if ( grid.isCurrentNodeLeft() )
    {
        if ( !grid.hasLeftNeighbour() )
        {
            u4 = grid.get( 1);
            return calcDSRightCorner( u3, u4, f, h, a);
        }

        u2 = grid.getLeftNeighbour()->getRight( t);
        u4 = grid.get( 1);
    } else if ( grid.isCurrentNodeRight() )
    {
        if ( !grid.hasRightNeighbour() )
        {
            u2 = grid.get( -1);
            return calcDSLeftCorner( u2, u3, f, h, a);
        }

        u2 = grid.get( -1);
        u4 = grid.getRightNeighbour()->getLeft( t);
    } else
    {
        u2 = grid.get( -1);
        u4 = grid.get( 1);
    }

    return calcDSLax( u2, u3, u4, f, h, a);
} // schemeLax

double
schemeRectangle( const pde::Grid& grid)
{
    auto [h, a] = grid.getPitch();

    double u0 = 0.0;
    double u2 = 0.0;
    double u3 = grid.get();
    double u4 = 0.0;

    auto [x, t] = grid.getCurrentNode();

    double f = 0.0;

    if ( grid.isCurrentNodeLeft() )
    {
        if ( !grid.hasLeftNeighbour() )
        {
            u4 = grid.get( 1);
            f = grid.getFunction()( h*x, a*t);

            return calcDSRightCorner( u3, u4, f, h, a);
        }

        u0 = grid.getLeftNeighbour()->getRight( t + 1);
        u2 = grid.getLeftNeighbour()->getRight( t);
        f = grid.getFunction()( h*(x+0.5), a*(t+0.5));
    } else
    {
        u0 = grid.get( -1, 1);
        u2 = grid.get( -1);
        f = grid.getFunction()( h*(x+0.5), a*(t+0.5));
    }

    return calcDSRectangle( u0, u2, u3, f, h, a);
} // schemeRectangle

} // namespace pde
