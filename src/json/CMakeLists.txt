cmake_minimum_required(VERSION 3.16)

add_library(json STATIC
    whitespace.cpp
    number.cpp
)
target_include_directories(json PUBLIC ${PROJECT_SOURCE_DIR}/include)