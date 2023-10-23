cmake_minimum_required(VERSION 3.10)
project(MathLab3)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
FetchContent_Declare(
    eigen_lib 
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG master)
FetchContent_MakeAvailable(eigen_lib)

set(LAB3_SOURCES main.cc src/app.cc )
add_executable(lab3 ${LAB3_SOURCES})
target_compile_features(lab3 PUBLIC cxx_std_20)
target_include_directories(lab3 PUBLIC include)
target_link_libraries(lab3 PRIVATE Eigen3::Eigen)

