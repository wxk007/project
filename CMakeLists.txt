cmake_minimum_required(VERSION 2.8)
project(project)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -g")
include_directories("${CMAKE_SOURCE_DIR}")
add_library(practicalsocket PracticalSocket.cpp)
#target_link_libraries (practicalsocket pthread rt)

set(SOURCE_FILES main.cpp app.cpp app.h PracticalSocket.cpp PracticalSocket.h)
add_executable(project ${SOURCE_FILES})

add_executable(TCPEchoServer TCPserverAndExecute.cpp app.cpp app.h)


target_link_libraries(TCPEchoServer practicalsocket)
