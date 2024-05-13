cmake_minimum_required(VERSION 3.10)
project(MathLab8)

find_package(MPI REQUIRED)

set(CMAKE_CXX_FLAGS "-Ofast")

# Static GNUPlot library.
set(VISUAL_SOURCES src/plot.cc)
add_library(visual ${VISUAL_SOURCES})
target_include_directories(visual PUBLIC include)
target_compile_features(visual PUBLIC cxx_std_20)

# PDE application.
add_executable(lab8 main.cc src/scheme.cc src/drawer.cc src/solver.cc)
target_include_directories(lab8 PUBLIC include)
target_compile_features(lab8 PUBLIC cxx_std_20)
target_link_libraries(lab8 PRIVATE visual MPI::MPI_CXX)
