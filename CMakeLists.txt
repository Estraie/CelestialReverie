cmake_minimum_required(VERSION 3.3.0)

project(GLAXY_ENGINE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall")

include_directories(include include/simulation)

add_executable(Glaxy_Engine src/simulation/test_simulation.cpp
                            src/simulation/celestial_body.cpp
                            src/simulation/celestial_reverie.cpp
                            src/simulation/celestial_system.cpp
                            src/simulation/simulate_algorithm.cpp)
