cmake_minimum_required(VERSION 3.7)
project(TP7)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        include/common.h
        src/deriche.c
        src/edge_detect.c
        src/pgm.c)

include_directories(include)

add_executable(TP7 ${SOURCE_FILES})