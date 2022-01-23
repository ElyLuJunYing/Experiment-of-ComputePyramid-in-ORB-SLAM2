cmake_minimum_required(VERSION 2.8)

project(ComputePyramid)

set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(ComputePyramid ComputePyramid.cpp)
target_link_libraries(ComputePyramid ${OpenCV_LIBS})