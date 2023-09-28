#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <fstream>
#include <string_view>

namespace visual
{

class GNUPlot
{

public:
    using Point = std::pair<double, double>;

private:
    std::fstream m_plot_file;
    std::string m_plot_file_name;
    std::string m_connector;
    bool m_drawing_plot;

public:
    GNUPlot();

    GNUPlot& setOutputFile( std::string_view output_file_name,
                            std::string_view output_file_format,
                            uint32_t width,
                            uint32_t height) &;

    GNUPlot& setTitle( std::string_view title,
                       uint32_t font_size = k_default_font_size ) &;

    GNUPlot& setLabelX( std::string_view label,
                        uint32_t font_size = k_default_font_size ) &;
    GNUPlot& setLabelY( std::string_view label,
                        uint32_t font_size = k_default_font_size ) &;

    GNUPlot& setRangeX( double left, double right ) &;
    GNUPlot& setRangeY( double bottom, double top ) &;

    GNUPlot& setLogX() &;
    GNUPlot& setLogY() &;

    GNUPlot& setGrid() &;
    GNUPlot& setAutoscale() &;

    GNUPlot& setKeyOutside() &;
    GNUPlot& setKeyTopLeft() &;
    GNUPlot& setKeyTopRight() &;
    GNUPlot& setKeyBottomRight() &;
    GNUPlot& setKeyBottomLeft() &;

    GNUPlot& setMTicsX( uint32_t mtics ) &;
    GNUPlot& setMTicsY( uint32_t mtics ) &;

    GNUPlot& setExpFormatX() &;
    GNUPlot& setExpFormatY() &;

    GNUPlot& withLines() &;
    GNUPlot& withLinesPoints() &;

    GNUPlot& drawPlot( std::string_view data_file_name,
                       uint32_t plot_index,
                       std::string_view plot_name ) &;

    GNUPlot& drawPlot( std::vector< Point > plot_data,
                       std::string_view plot_name ) &;

    GNUPlot& makePlot() &;

public:
    static constexpr uint32_t k_default_font_size = 15;

}; // class GNUPlot

} // namespace visual
