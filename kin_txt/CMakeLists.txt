#specify the minimum version for CMake

cmake_minimum_required(VERSION 2.8)

# Project's Corrections_table

project(Corrections_table)
add_executable(CTable Corrections_table.C)
# Set the output folder where your program will be created
#set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
#set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# The following folder will be included
include_directories("$LD_LIBRARY_PATH")

