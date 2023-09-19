cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 20)

# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   # Specify the commit you depend on and update it regularly.
#   URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
# )
# # For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)

find_package(GTest CONFIG REQUIRED)

add_executable(googleTest 
  character_test.cpp 
  digits_test.cpp
  skip_whitespace_test.cpp
  string_test.cpp
  map_test.cpp
  choice_test.cpp
  join_test.cpp
  many_test.cpp
  separated_test.cpp
  regex_test.cpp
  exception_test.cpp
)

target_link_libraries(googleTest GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

add_subdirectory(json)

# internal
target_link_libraries(googleTest parser exception json)
# test
target_link_libraries(googleTest json_test)

target_include_directories(googleTest PUBLIC ${PROJECT_SOURCE_DIR}/include)

enable_testing()