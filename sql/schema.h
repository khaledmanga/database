#pragma once

#include <string>
#include <variant>

using DataType = std::variant<std::string, int, bool>;

struct Column {
    std::string name;
    DataType type;
};

struct TableSchema {
    std::string name;
    std::vector<Column> columns;
};