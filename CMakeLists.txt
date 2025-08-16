cmake_minimum_required(VERSION 3.20)
project(cpp_features_demo LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(demo_cpp20 src/demo_cpp20.cpp)
target_compile_features(demo_cpp20 PRIVATE cxx_std_20)

add_executable(demo_cpp23 src/demo_cpp23.cpp)
target_compile_features(demo_cpp23 PRIVATE cxx_std_23)

find_package(Threads REQUIRED)
target_link_libraries(demo_cpp20 PRIVATE Threads::Threads)
target_link_libraries(demo_cpp23 PRIVATE Threads::Threads)