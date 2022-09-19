cmake_minimum_required(VERSION 3.5)
project(doctest LANGUAGES CXX VERSION 2.3.8)

add_library(doctest STATIC doctest.cpp)
target_include_directories(doctest PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>)
set_target_properties(doctest PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

add_library(doctest::doctest ALIAS doctest)
