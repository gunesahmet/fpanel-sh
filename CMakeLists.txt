cmake_minimum_required(VERSION 3.15)
project(fsh)

set(CMAKE_CXX_STANDARD 14)
include_directories(.)
IF (WIN32)
    include_directories(I:/common/boost_1_67_0)
ELSE()
    include_directories(/usr/local/include/)
ENDIF()

add_executable(fsh main.cpp)