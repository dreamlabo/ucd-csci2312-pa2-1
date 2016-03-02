cmake_minimum_required(VERSION 3.3)
project(PA2_SPRING_2016)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp  Cluster.cpp Cluster.h ClusteringTests.cpp ClusteringTests.h ErrorContext.cpp ErrorContext.h Point.cpp Point.h)
add_executable(PA2_SPRING_2016 ${SOURCE_FILES})