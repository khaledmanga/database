cmake_minimum_required(VERSION 3.17)

project(database)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPEND *.cc)

add_executable(database ${SOURCES})
