cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(googleTest 
  character_test.cpp 
  digits_test.cpp
  skip_whitespace_test.cpp
  string_test.cpp
)


# internal
target_link_libraries(googleTest parser exception)
# external
target_link_libraries(googleTest gtest_main)
target_include_directories(googleTest PUBLIC ${PROJECT_SOURCE_DIR}/include)

enable_testing()