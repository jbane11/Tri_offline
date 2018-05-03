#specify the minimum version for CMake

cmake_minimum_required(VERSION 2.8)

# Project's Corrections_table

project(Corrections_table)
# Set the output folder where your program will be created
#set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
#set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})


# The following folder will be included
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
include_directories("$LD_LIBRARY_PATH")
#include_directories("$ROOTSYS")
include_directories("$PATH")
include_directories("/home/jbane/analyzer/analyzer/src/")
include_directories("/home/jbane/analyzer/analyzer/hana_decode/")
#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS RIO Net)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
#ROOT_GENERATE_DICTIONARY(G__CTable CTable.h LINKDEF CTableLinkDef.h)

#---Create a shared library with geneated dictionary
#add_library(CTable SHARED Corrections_table.C G__Corrections_table.C)
#target_link_libraries(CTable ${ROOT_LIBRARIES})

#---Create  a main program using the library
add_executable(CTable Corrections_table.cpp)
#target_link_libraries(Main CTable)
