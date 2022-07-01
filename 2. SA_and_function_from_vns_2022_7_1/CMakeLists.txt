cmake_minimum_required(VERSION 3.15)
project(EVRP)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(dependencies 
    FileOperator.cpp
    FileOperator.hpp
    Macro.hpp
    Problem.cpp
    Problem.hpp
    ACO.cpp
    Algorithm.hpp
    DBCA.cpp
    Operators.cpp
    Operators.hpp
    SA.cpp
    )

add_executable(main main.cpp ${dependencies})