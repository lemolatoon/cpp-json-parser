cmake_minimum_required(VERSION 3.16)

add_library(json_test STATIC
    number_test.cpp
)

include_directories(${GTest_INCLUDE_DIRS})
include_directories(${GTest_SOURCE_DIR}/googletest/include)

# 定義されているすべての変数を表示する
get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

target_include_directories(json_test PUBLIC ${PROJECT_SOURCE_DIR}/include)