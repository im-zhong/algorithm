cmake_minimum_required(VERSION 3.31)

project(HelloWorld)

find_package(doctest CONFIG REQUIRED)

add_executable(HelloWorld helloworld.cpp)

target_link_libraries(HelloWorld PRIVATE doctest::doctest)
