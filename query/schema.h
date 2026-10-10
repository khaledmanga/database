#pragma once

#include <string>
#include <variant>
#include <vector>

enum class DataType {
  STRING,
  INTEGER,
  BOOL,
};

using ValueType = std::variant<std::string, int, bool>;

class Column {
public:
  std::string name;
  ValueType value;
  DataType type;

  Column(const std::string &name, const ValueType &value, DataType type);
  ValueType getValue() const;
};

class Schema {
private:
  std::vector<Column> columns;

public:
  std::vector<Column> get_column() const;
  void add_column(const std::string &name, const ValueType &value, DataType type);
  bool is_available_column(const std::string &name) const;
  bool is_valid_type(const ValueType &value, DataType type) const;
};
