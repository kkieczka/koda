cmake_minimum_required(VERSION 3.3)
project(koda)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Encoder.cpp Encoder.h Symbol.cpp Symbol.h Character.cpp Character.h)
add_executable(koda ${SOURCE_FILES})