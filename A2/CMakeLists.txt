cmake_minimum_required(VERSION 3.13)
project(EC_A2)

set(CMAKE_CXX_STANDARD 14)

set(GCC_COVERAGE_COMPILE_FLAGS "-static")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
add_executable(Solver_HC_EvalCPP main.cpp evalCPP.o HillClimber.cpp Solution.cpp Globals.cpp )
add_executable(Solver_HC_EvalCPP1 main.cpp evalCPP1.o HillClimber.cpp Solution.cpp Globals.cpp )
add_executable(Solver_GA_EvalCPP GeneticAlgorithm.cpp evalCPP.o)
add_executable(Solver_GA_EvalCPP1 GeneticAlgorithm.cpp evalCPP1.o)
