cmake_minimum_required(VERSION 3.16)

add_library(json_test STATIC
    number_test.cpp
)

include(FetchContent)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(${googletest_SOURCE_DIR}/googletest/include)

target_include_directories(json_test PUBLIC ${PROJECT_SOURCE_DIR}/include)