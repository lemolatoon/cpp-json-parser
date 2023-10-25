cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 20)

add_subdirectory(parser)
add_subdirectory(json)
add_subdirectory(exception)

add_executable(a.out main.cpp)
target_link_libraries(a.out exception parser json)

find_package(ICU REQUIRED uc)
include_directories(${ICU_INCLUDE_DIRS})
# `dl` is required for `ICU`
target_link_libraries(a.out ${ICU_LIBRARIES} dl)

target_include_directories(a.out PUBLIC ${PROJECT_SOURCE_DIR}/include)