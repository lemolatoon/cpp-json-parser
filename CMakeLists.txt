cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_VERBOSE_MAKEFILE ON)
# set(CMAKE_CXX_CLANG_TIDY clang-tidy;checks=*,readability*,-header-filter=.*)

project(cpp-json-parser CXX)

add_subdirectory(test)
add_subdirectory(src)
