cmake_minimum_required(VERSION 4.0)
project(OOP_6_2_2)

set(CMAKE_CXX_STANDARD 17)

add_executable(OOP_6_2_2 main.cpp CStringList.h CStringList.cpp)

include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip)
FetchContent_MakeAvailable(googletest)

add_executable(test tests.cpp CStringList.h CStringList.cpp)
target_link_libraries(test gtest gtest_main)