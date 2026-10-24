cmake_minimum_required(VERSION 3.17)

project(database)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#include(FetchContent)
#FetchContent_Declare(
#  googletest
#  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
#)

#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)

#enable_testing()

file(GLOB_RECURSE ALL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")

set(SOURCES ${ALL_SOURCES})
list(FILTER SOURCES EXCLUDE REGEX "_test\\.cc$")

add_executable(database ${SOURCES})
target_include_directories(database PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

#set(SOURCES_TEST ${ALL_SOURCES})
#list(FILTER SOURCES_TEST EXCLUDE REGEX "main\\.cc$")

#add_executable(test_database ${SOURCES_TEST})
#target_link_libraries(test_database PRIVATE gtest_main)