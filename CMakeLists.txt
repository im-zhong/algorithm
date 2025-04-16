cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_EXTENSIONS OFF)

project(Algorithm
        VERSION 0.1.0
        DESCRIPTION "Algorithm"
        LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)

find_package(doctest CONFIG REQUIRED)

add_subdirectory(algorithm)
