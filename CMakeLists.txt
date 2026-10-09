cmake_minimum_required(VERSION 3.17)

project(database)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    database
    main.cpp
    bplustree/b_plus_tree.cc
    storage/file_manager.cc
    storage/page.cc
    storage/metadata.cc
    operators/tree.cc
    query/lexer.cc
    utils/common.cc
)
