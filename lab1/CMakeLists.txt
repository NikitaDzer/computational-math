cmake_minimum_required(VERSION 3.10)
project(MathLab1)

set(VISUAL_SOURCES src/plot.cc)
add_library(visual ${VISUAL_SOURCES})
target_compile_features(visual PUBLIC cxx_std_20)
target_include_directories(visual PUBLIC include)

set(APPR_SOURCES src/approximation.cc)
add_library(appr ${APPR_SOURCES})
target_compile_features(appr PUBLIC cxx_std_20)
target_include_directories(appr PUBLIC include)

set(LAB1_SOURCES main.cc src/app.cc)
add_executable(lab1 ${LAB1_SOURCES})
target_compile_features(lab1 PUBLIC cxx_std_20)
target_include_directories(lab1 PUBLIC include)
target_link_libraries(lab1 PRIVATE visual appr)

