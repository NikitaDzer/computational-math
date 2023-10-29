#include "app.hh"
#include "plot.hh"
#include "snae.hh"
#include "utils.hh"

#include <Eigen/Dense>

#include <cmath>
#include <tuple>
#include <vector>
#include <format>
#include <utility>

namespace app
{

void
initIDPlot( visual::GNUPlot& plot,
            std::string image_dir,
            std::string plot_name )
{
    std::string title = std::format(
        "Delta on iterations number of {} iterative methods.",
        plot_name
    );

    plot.setTitle( title, 20);
    plot.setOutputFile( 
        image_dir + plot_name,
        "svg", 800, 600 
    );

    plot.setGrid();
    plot.setLogY();

    plot.setMTicsX( 1 );
    plot.setMTicsY( 1 );

    plot.setExpFormatY();

    plot.setAutoscale();

    plot.setLabelX( "iterations number" );
    plot.setLabelY( "delta" );

    plot.withLinesPoints();
} // initIDPlot

std::tuple< FNAE, FNAE, JNAE >
getPreparedNAE()
{
    FNAE F_smi{};
    F_smi[ 0 ] = []( const Eigen::Vector<double, 1>& x )
    {
        return -std::sqrt( 0.5 * (3 - 5 * x[ 0 ]) );
    };

    FNAE F_newton{};
    F_newton[ 0 ] = []( const Eigen::Vector<double, 1>& x )
    {
        return 2 * x[ 0 ] * x[ 0 ] + 5 * x[ 0 ] - 3;
    };

    JNAE J_newton{};
    J_newton[ 0 ][ 0 ] = []( const Eigen::Vector<double, 1>& x )
    {
        return 4 * x[ 0 ] - 5;
    };

    return { F_smi, F_newton, J_newton };
} // getPreparedNAE

std::tuple< FSNAE, FSNAE, JSNAE >
getPreparedSNAE()
{
    FSNAE F_smi{};
    F_smi[ 0 ] = []( const Eigen::Vector<double, 2>& x )
    {
        return std::cos( x[ 1 ] ) + 0.85;
    };
    F_smi[ 1 ] = []( const Eigen::Vector<double, 2>& x )
    {
        return std::sin( x[ 0 ] ) - 1.32;
    };

    FSNAE F_newton{};
    F_newton[ 0 ] = []( const Eigen::Vector<double, 2>& x )
    {
        return std::cos( x[ 1 ] ) - x[ 0 ] + 0.85;
    };
    F_newton[ 1 ] = []( const Eigen::Vector<double, 2>& x )
    {
        return std::sin( x[ 0 ] ) - x[ 1 ] - 1.32;
    };

    JSNAE J_newton{};
    J_newton[ 0 ][ 0 ] = []( const Eigen::Vector<double, 2>& x )
    {
        return -1;
    };
    J_newton[ 0 ][ 1 ] = []( const Eigen::Vector<double, 2>& x )
    {
        return -std::sin( x[ 1 ] );
    };
    J_newton[ 1 ][ 0 ] = []( const Eigen::Vector<double, 2>& x )
    {
        return std::cos( x[ 0 ] );
    };
    J_newton[ 1 ][ 1 ] = []( const Eigen::Vector<double, 2>& x )
    {
        return -1;
    };

    return { F_smi, F_newton, J_newton };
} // getPreparedSNAE

} // namespace app
