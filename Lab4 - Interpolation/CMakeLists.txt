cmake_minimum_required(VERSION 3.10)
project(MathLab4)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
FetchContent_Declare(
    eigen_lib 
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG master)
FetchContent_MakeAvailable(eigen_lib)

set(VISUAL_SOURCES src/plot.cc)
add_library(visual ${VISUAL_SOURCES})
target_include_directories(visual PUBLIC include)
target_compile_features(visual PUBLIC cxx_std_20)

set(LAB4_SOURCES main.cc src/newton.cc src/spline.cc src/lsm.cc src/app.cc)
add_executable(lab4 ${LAB4_SOURCES})
target_compile_features(lab4 PUBLIC cxx_std_20)
target_include_directories(lab4 PUBLIC include)
target_link_libraries(lab4 PRIVATE visual Eigen3::Eigen)

