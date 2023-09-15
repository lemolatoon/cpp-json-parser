cmake_minimum_required(VERSION 3.16)

add_library(parser STATIC character.cpp)
target_include_directories(parser PUBLIC ${PROJECT_SOURCE_DIR}/include)