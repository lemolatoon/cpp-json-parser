cmake_minimum_required(VERSION 3.16)

add_library(exception STATIC exception.cpp)
target_include_directories(exception PUBLIC ${PROJECT_SOURCE_DIR}/include)