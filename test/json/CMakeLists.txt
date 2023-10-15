cmake_minimum_required(VERSION 3.16)

add_library(json_test STATIC
    number_test.cpp
    string_test.cpp
    whitespace_test.cpp
    value_test.cpp
)

find_package(GTest CONFIG REQUIRED)
target_link_libraries(json_test GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

target_include_directories(json_test PUBLIC ${PROJECT_SOURCE_DIR}/include)