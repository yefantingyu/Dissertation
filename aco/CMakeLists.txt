cmake_minimum_required(VERSION 3.15)
project(evrp)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(dependencies Problem.hpp Problem.cpp Macro.hpp Solver.cpp Solver.hpp 
Model/ACO.hpp 
Model/ACO.cpp 
Model/Random.hpp
Model/Solution.cpp
Model/Solution.hpp 
Model/Clock.h)

add_executable(main main.cpp ${dependencies})