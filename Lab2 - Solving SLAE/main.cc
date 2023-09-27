#include <glm/ext/matrix_float4x4.hpp>
#include <glm/matrix.hpp>
#include <string>
#include <iostream>
#include <glm/mat4x4.hpp>

#include "include/gauss.hh"
#include "include/jacobi.hh"
#include "lu.hh"
#include <Eigen/Dense>

using namespace Eigen;

int 
main()
{
    // constexpr int M = 4;
    // Eigen::Matrix<double, 4, 4> mat{
    //     { 3, 4, -9, 5 },
    //     { -15, -12, 50, -16 },
    //     { -27, -36, 73, 8 },
    //     { 9, 12, -10, -16 }
    // };
    // Eigen::Vector<double, 4> vec{ -14, 44, 142, -76 };

    constexpr int M = 3;
    Eigen::Matrix<double, M, M> mat{
        { 100, 30, -70 },
        { 15, -50, -5 },
        { 6, 2, 20 }
    };
    Eigen::Vector<double, M> vec{ 60, -40, 28 };

    auto xjacobi = slae::solveSLAEJacobi( mat, vec, 100 );
    std::cout << xjacobi << std::endl;

    auto xlu = slae::solveSLAELU( mat, vec );
    std::cout << xlu << std::endl;

    auto xgauss = slae::solveSLAEGauss( mat, vec );
    std::cout << xgauss << std::endl;

    return 0;
} // main
