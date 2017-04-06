cmake_minimum_required(VERSION 2.8)
project(ti-flashstream-flasher)

set(CMAKE_CXX_FLAGS -std=c++11)

include_directories("src/filestream_parser")
include_directories("src/i2c_interface")

set(HEADERS "src/filestream_parser/filestream_parser.h"
            "src/i2c_interface/i2c_interface.h")
FILE(GLOB_RECURSE SRCS "src/*.cpp")

# Some information for the build
MESSAGE(STATUS "Current built type: ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "Current build type flags: ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}}")

add_executable( ${PROJECT_NAME} ${SRCS} ${HEADERS})
target_link_libraries( ${PROJECT_NAME})
