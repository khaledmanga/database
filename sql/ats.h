#pragma once

#include <string>
#include <variant>

struct InsertStatement {
  std::string table;
  std::vector<std::string> columns;
  std::vector<std::string> values;
};

using Statment = std::variant<InsertStatement>;