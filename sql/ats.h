#pragma once

#include <string>
#include <variant>
#include <vector>

struct InsertStatement {
  std::string table;
  std::vector<std::string> columns;
  std::vector<std::string> values;
};

using Statement = std::variant<InsertStatement>;