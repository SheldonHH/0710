# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.

cmake_minimum_required(VERSION 3.12)

# Set the project name
project(CMakeProject1 VERSION 1.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source to this project's executable.
add_executable(CMakeProject1
    CMakeProject1.cpp
    Satellite.cpp
    GroundStation.cpp
)

# Add include directories
target_include_directories(CMakeProject1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# TODO: Add tests and install targets if needed.
