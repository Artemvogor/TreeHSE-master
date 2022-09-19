cmake_minimum_required(VERSION 3.22)
project(TreeHSE)

set(CMAKE_CXX_STANDARD 14)

add_executable(TreeHSE main.cpp)

enable_testing()
add_subdirectory(doctest)

add_executable(TreeHSE_test test/test.cpp)
target_link_libraries(TreeHSE_test doctest)