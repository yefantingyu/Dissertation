cmake_minimum_required(VERSION 3.15)
project(evrp)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(dependencies Problem.hpp Problem.cpp Solver.hpp Solver.cpp Macro.hpp FileOperator.cpp FileOperator.hpp)

add_executable(main main.cpp ${dependencies})