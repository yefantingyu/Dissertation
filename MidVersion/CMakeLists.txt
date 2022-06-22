cmake_minimum_required(VERSION 3.15)
project(evrp)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(dependencies Problem.hpp FileOperator.hpp utils.hpp Solver.hpp Model/ACO.hpp)

add_executable(main main.cpp ${dependencies})