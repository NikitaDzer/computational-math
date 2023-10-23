#include "app.hh"

#include <format>
#include <string>
#include <iostream>

int 
main()
{
    auto [ nae_smi, nae_newton ] = app::solvePreparedNAE();
    auto [ snae_smi, snae_newton ] = app::solvePreparedSNAE();

    std::string nae_format = std::format(
        "NAE SMI:     x = {}\n"
        "NAE Newton:  x = {}\n",
        nae_smi[ 0 ],
        nae_newton[ 0 ]
    );

    std::string snae_format = std::format(
        "SNAE SMI:    x = {}, y = {}\n"
        "SNAE Newton: x = {}, y = {}\n",
        snae_smi[ 0 ], snae_smi[ 1 ],
        snae_newton[ 0 ], snae_newton[ 1 ]
    );

    std::cout << nae_format + snae_format << std::endl;

    return 0;
} // main
