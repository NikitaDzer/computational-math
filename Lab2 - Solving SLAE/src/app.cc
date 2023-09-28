#include "app.hh"
#include "plot.hh"
#include "utils.hh"

#include <Eigen/Dense>

#include <cmath>
#include <vector>
#include <cstdint>
#include <string_view>

namespace app
{

/**
 * Dependence of the discrepancy 
 * on the number of iterations.
 */
using ID = std::pair<double, double>;

static std::vector<ID>
runIDCalculation( IterativeMethod iterative_method,
                  uint32_t tests_number,
                  const Eigen::Matrix<double, M, M>& slae,
                  const Eigen::Vector<double, M>& rhs )
{
    std::vector<ID> results{};
    results.reserve( tests_number );

    for ( uint32_t i = 0; i < tests_number; i++ )
    {
        int iterations = static_cast<int>( std::pow( 2, i + 1 ) );

        Eigen::Vector<double, M> root = 
            iterative_method( slae, rhs, iterations );
        double discrepancy = 
            slae::utils::calcSolutionDiscrepancy( slae, rhs, root );

        results.push_back( { iterations, discrepancy } );
    }

    return results;
} // runIDCalculation

void
drawIDPlot( IterativeMethod iterative_method,
            uint32_t tests_number,
            Eigen::Matrix<double, M, M>& slae,
            Eigen::Vector<double, M>& rhs,
            visual::GNUPlot& plot,
            std::string_view method_name )
{
    std::vector<ID> data = runIDCalculation(
        iterative_method, tests_number, slae, rhs
    );
    plot.drawPlot( data, method_name );
} // drawIDPlot

void
initIDPlot( visual::GNUPlot& plot,
            std::string_view image_name )
{
    plot.setTitle( 
        "Discrepancy on iterations number of SLAE iterative methods.", 20
    );
    plot.setOutputFile( 
        image_name, 
        "svg", 800, 600 
    );

    plot.setGrid();
    plot.setLogX();
    plot.setLogY();

    plot.setMTicsX( 1 );
    plot.setMTicsY( 1 );

    plot.setExpFormatX();
    plot.setExpFormatY();

    plot.setAutoscale();

    plot.setLabelX( "iterations number" );
    plot.setLabelY( "discrepancy" );

    plot.withLinesPoints();
} // initIDPlot

std::pair< Eigen::Matrix<double, M, M>, Eigen::Vector<double, M> >
createSLAEEquation()
{
    constexpr int a = 10;

    Eigen::Matrix<double, M, M> slae{};
    Eigen::Vector<double, M> rhs{};

    slae.setZero();
    rhs.setZero();

    for ( int i = 0; i < M; i++ )
    {
        int begin = std::clamp( i - 4, 0, i );
        int end = std::clamp( i + 4, 0, M );

        for ( int j = begin; j < end; j++ )
        {
            slae( i, j ) = 1;
        }

        slae( i, i ) = a;
        rhs( i ) = i + 1;
    }

    return {
        slae,
        rhs
    };
} // createSLAEEquation

} // namespace app
