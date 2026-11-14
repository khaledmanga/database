#!/usr/bin/env bash

SOURCE_CODE=$(find ./src -type f \( -name "*.cpp" -o -name "*.hpp" \))

clang-format -i $SOURCE_CODE
