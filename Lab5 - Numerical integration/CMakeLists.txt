cmake_minimum_required(VERSION 3.10)
project(MathLab5)

set(VISUAL_SOURCES src/plot.cc)
add_library(visual ${VISUAL_SOURCES})
target_include_directories(visual PUBLIC include)
target_compile_features(visual PUBLIC cxx_std_20)

set(LAB5_SOURCES main.cc src/trapezoid.cc)
add_executable(lab5 ${LAB5_SOURCES})
target_compile_features(lab5 PUBLIC cxx_std_20)
target_include_directories(lab5 PUBLIC include)
target_link_libraries(lab5 PRIVATE visual)

