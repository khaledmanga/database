cmake_minimum_required(VERSION 3.17)

project(database)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")

add_executable(database ${SOURCES})

target_include_directories(database PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})