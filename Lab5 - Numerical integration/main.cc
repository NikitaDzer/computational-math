#include "trapezoid.hh"

#include <array>
#include <iostream>

int 
main()
{
    auto f = std::to_array({
        1.0, 0.989616, 0.958851, 0.908852, 0.841471,
        0.759188, 0.664997, 0.562278, 0.454649
    });
    double h = 0.25;

    std::cout << "Integral " << integr::calcIntegralTrapezoid( f, h);

    return 0;
} // main
