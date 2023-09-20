#include "app.hh"
#include "plot.hh"
#include "inputfn.hh"
#include "approximation.hh"

#include <cmath>
#include <format>
#include <vector>
#include <string>
#include <cstdint>
#include <string_view>

namespace app
{

using StepError = std::pair<double, double>;

static StepError
calcDerivativeError( appr::InputFunction ref_f1,
                     appr::InputFunction f,
                     appr::ApprFunction appr_f1,
                     double x,
                     uint32_t n )
{
    double step = 1.0 / std::pow( 2, n );

    double ref_result = ref_f1( x );
    double appr_result = appr_f1( f, x, step );

    double error = std::abs( ref_result - appr_result );

    return { step, error };
} // calcDerivativeError

static std::vector<StepError>
runDerivativeErrorCalculation( appr::InputFunction ref_f1,
                               appr::InputFunction f,
                               appr::ApprFunction appr_f1,
                               double x )
{
    const uint32_t k_iterations_number = 21;

    std::vector<StepError> results{};
    results.reserve( k_iterations_number );

    for ( uint32_t n = 0; n < k_iterations_number; n++ )
    {
        results.push_back(
            calcDerivativeError( ref_f1, f, appr_f1, x, n )
        );
    }

    return results;
} // runDerivativeErrorCalculation

static void
prepareDerivativeErrorPlot( visual::GNUPlot& plot,
                            std::string_view function_name,
                            std::string_view image_name )
{
    std::string title = std::format(
        "Error on step of f(x) = {}.",
        function_name
    );
    
    plot.setTitle( title, 20 );
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

    plot.setKeyTopLeft();

    plot.setAutoscale();
    plot.setRangeX( 1e-7, 10 );

    plot.setLabelX( "step" );
    plot.setLabelY( "error" );
} // prepareDerivativeErrorPlot

void
makeDerivativeErrorPlot( appr::InputFunction ref_f1,
                         appr::InputFunction f,
                         double x,
                         visual::GNUPlot& plot,
                         std::string_view function_name,
                         std::string_view image_name)
{
    prepareDerivativeErrorPlot( plot, function_name, image_name );

    std::vector<StepError> data = runDerivativeErrorCalculation(
        ref_f1,
        f,
        appr::calcDerivative1,
        x
    );
    plot.drawPlot( data, "calcDerivative1" );

    data = runDerivativeErrorCalculation(
        ref_f1,
        f,
        appr::calcDerivative2,
        x
    );
    plot.drawPlot( data, "calcDerivative2" );

    data = app::runDerivativeErrorCalculation(
        ref_f1,
        f,
        appr::calcDerivative3,
        x
    );
    plot.drawPlot( data, "calcDerivative3" );

    data = app::runDerivativeErrorCalculation(
        ref_f1,
        f,
        appr::calcDerivative4,
        x
    );
    plot.drawPlot( data, "calcDerivative4" );

    data = app::runDerivativeErrorCalculation(
        ref_f1,
        f,
        appr::calcDerivative5,
        x
    );
    plot.drawPlot( data, "calcDerivative5" );

    plot.makePlot();
} // makeDerivativeErrorPlot

void
initDerivativeErrorPlot( visual::GNUPlot& plot )
{
    plot.setGrid();
    plot.setLogX();
    plot.setLogY();

    plot.setMTicsX( 1 );
    plot.setMTicsY( 1 );

    plot.setExpFormatX();
    plot.setExpFormatY();

    plot.setKeyTopLeft();

    plot.setAutoscale();
    plot.setRangeX( 1e-7, 10 );

    plot.setLabelX( "step" );
    plot.setLabelY( "error" );

    plot.withLinesPoints();
} // initDerivativeErrorPlot

} // namespace appr
