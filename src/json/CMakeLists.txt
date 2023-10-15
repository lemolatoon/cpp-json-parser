cmake_minimum_required(VERSION 3.16)

add_library(json STATIC
    whitespace.cpp
    number.cpp
    "string.cpp"
    value.cpp
)
target_include_directories(json PUBLIC ${PROJECT_SOURCE_DIR}/include)

find_package(ICU REQUIRED uc)
include_directories(${ICU_INCLUDE_DIRS})
# `dl` is required for `ICU`
target_link_libraries(json ${ICU_LIBRARIES} dl)