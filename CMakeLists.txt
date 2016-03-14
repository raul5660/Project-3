cmake_minimum_required(VERSION 3.3)
project(Project3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")

set(SOURCE_FILES shell.c)
add_executable(untitled ${SOURCE_FILES})