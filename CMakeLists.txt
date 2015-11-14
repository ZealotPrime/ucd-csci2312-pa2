cmake_minimum_required(VERSION 3.3)
project(CSCI2312_PA2_Lee)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Point.h Clustering.cpp Clustering.h KMeans.h KMeans.cpp)
add_executable(CSCI2312_PA2_Lee ${SOURCE_FILES})