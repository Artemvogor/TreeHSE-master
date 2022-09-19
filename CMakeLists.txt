cmake_minimum_required(VERSION 3.21.1)
project(TreeHSE-master)

set(CMAKE_CXX_STANDARD 11)

add_executable(TreeHSE-master Visual.cpp)

enable_testing()
add_subdirectory(doctest)

add_executable(TreeHSE-master_test test/test.cpp)
target_link_libraries(TreeHSE-master_test doctest)