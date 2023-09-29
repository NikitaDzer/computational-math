#include "plot.hh"

#include <cstdio>
#include <format>
#include <string>
#include <fstream>
#include <cstdint>
#include <exception>
#include <string_view>

namespace visual
{

static std::string
getTempFileName()
{
    std::string tmp_file_name = std::tmpnam( nullptr );
    if ( tmp_file_name.empty() )
    {
        throw std::runtime_error{ "Cannot create temp file name.\n" };
    }

    return tmp_file_name;
} // GNUPlot::getTempFileName

GNUPlot::GNUPlot():
    m_plot_file{},
    m_plot_file_name{ "" },
    m_connector{ "" },
    m_drawing_plot{ false }
{
    m_plot_file_name = getTempFileName();
    m_plot_file.open( m_plot_file_name, std::ios_base::out );
} // GNUPlot::GNUPlot

GNUPlot&
GNUPlot::setLabelX( std::string_view label,
                    uint32_t font_size ) &
{
    m_plot_file << std::format(
        "set xlabel '{}' font ', {}'\n", 
        label, font_size
    );

    return *this;
} // GNUPlot::setLabelX

GNUPlot&
GNUPlot::setLabelY( std::string_view label,
                    uint32_t font_size ) &
{
    m_plot_file << std::format(
        "set ylabel '{}' font ', {}'\n", 
        label, font_size
    );

    return *this;
} // GNUPlot::setLabelY

GNUPlot&
GNUPlot::setRangeX( double left,
                    double right ) &
{
    m_plot_file << std::format( 
        "set xrange [{}:{}]\n", 
        left, right 
    );

    return *this;
} // GNUPlot::setRangeX

GNUPlot&
GNUPlot::setRangeY( double bottom,
                    double top ) &
{
    m_plot_file << std::format(
        "set yrange [{}:{}]\n", 
        bottom, top 
    );

    return *this;
} // GNUPlot::setRangeY

GNUPlot& 
GNUPlot::setAutoscale() &
{
    m_plot_file << "set autoscale\n";
    return *this;
} // GNUPlot::setAutoscale

GNUPlot&
GNUPlot::setGrid() &
{
    m_plot_file << "set grid\n";
    return *this;
} // GNUPlot::setGrid

GNUPlot&
GNUPlot::setLogX() &
{
    m_plot_file << "set logscale x\n";
    return *this;
} // GNUPlot::setLogX

GNUPlot&
GNUPlot::setLogY() &
{
    m_plot_file << "set logscale y\n";
    return *this;
} // GNUPlot::setLogY

GNUPlot&
GNUPlot::setKeyOutside() &
{
    m_plot_file << "set key outside\n";
    return *this;
} // GNUPlot::setKeyOutside

GNUPlot&
GNUPlot::setKeyTopLeft() &
{
    m_plot_file << "set key top left\n";
    return *this;
} // GNUPlot::setKeyTopLeft

GNUPlot&
GNUPlot::setKeyTopRight() &
{
    m_plot_file << "set key top right\n";
    return *this;
} // GNUPlot::setKeyTopRight

GNUPlot&
GNUPlot::setKeyBottomRight() &
{
    m_plot_file << "set key bottom right\n";
    return *this;
} // GNUPlot::setKeyBottomRight

GNUPlot&
GNUPlot::setKeyBottomLeft() &
{
    m_plot_file << "set key bottom left\n";
    return *this;
} // GNUPlot::setKeyBottomLeft

GNUPlot&
GNUPlot::setMTicsX( uint32_t mtics ) &
{
    m_plot_file << std::format(
        "set mxtics {}\n",
        mtics
    );
    
    return *this;
} // GNUPlot::setMTicX

GNUPlot&
GNUPlot::setMTicsY( uint32_t mtics ) &
{
    m_plot_file << std::format(
        "set mytics {}\n",
        mtics
    );
    
    return *this;
} // GNUPlot::setMTicsY

GNUPlot&
GNUPlot::setExpFormatX() &
{
    m_plot_file << "set format x '10^{%L}'\n";
    return *this;
} // GNUPlot::setExpFormatX

GNUPlot&
GNUPlot::setExpFormatY() &
{
    m_plot_file << "set format y '10^{%L}'\n";
    return *this;
} // GNUPlot::setExpFormatY

GNUPlot&
GNUPlot::setTitle( std::string_view title,
                   uint32_t font_size ) &
{
    m_plot_file << std::format( 
        "set title '{}' font ', {}' \n", 
        title, font_size
    );

    return *this;
} // GNUPlot::setTitle

GNUPlot&
GNUPlot::withLines() &
{
    m_connector = " with lines ";
    return *this;
} // GNUPlot::withLines

GNUPlot&
GNUPlot::withLinesPoints() &
{
    m_connector = " with linespoints ";
    return *this;
} // GNUPlot::withLinesPoints

GNUPlot&
GNUPlot::setOutputFile( std::string_view output_file_name,
                        std::string_view output_file_format,
                        uint32_t width,
                        uint32_t height) &
{
    m_plot_file << std::format(
        "set terminal {} size {} {} background rgb 'white'\n"
        "set output '{}.{}'\n",
        output_file_format, width, height,
        output_file_name, output_file_format
    );

    return *this;
} // GNUPlot::setOutputFileName

GNUPlot&
GNUPlot::drawPlot( std::string_view data_file_name,
                   uint32_t plot_index,
                   std::string_view plot_name ) &
{
    if ( !m_drawing_plot )
    {
        m_drawing_plot = true;
        m_plot_file << "plot ";
    } else
    {
        m_plot_file << ", ";
    }

    m_plot_file << std::format( 
        "'{}' using 1:{} title '{}' {}",
        data_file_name,
        plot_index + 2,
        plot_name,
        m_connector
    );
 
    return *this;
} // GNUPlot::drawPlot

GNUPlot&
GNUPlot::drawPlot( std::vector< GNUPlot::Point > plot_data,
                   std::string_view plot_name ) &
{
    std::string data_file_name{ getTempFileName() };
    std::fstream data_file{ data_file_name, std::ios_base::out };

    for ( auto&& point : plot_data )
    {
        data_file << std::format(
            "{}, {}\n",
            point.first, point.second
        );
    }

    return drawPlot( data_file_name, 0, plot_name );
} // GNUPlot::drawPlot

GNUPlot&
GNUPlot::makePlot() &
{
    m_drawing_plot = false;

    m_plot_file << "\n";
    m_plot_file.flush();

    std::string command = std::format(
        "gnuplot < {}",
        m_plot_file_name
    );
    std::system( command.c_str() );

    return *this;
} // GNUPlot::makePlot

} // namespace visual
