#pragma once

#include <cstring>
#include <vector>

class InsertStatement {
    std::string table;
    std::vector<std::string> columns;
    std::vector<std::string> values;
}