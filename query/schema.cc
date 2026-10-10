#include "schema.h"

Column::Column(const std::string &name, const ValueType &value, DataType type) : name(name), value(value), type(type) {}

ValueType Column::getValue() const { return this->value; }

std::vector<Column> Schema::get_column() const { return this->columns; }

void Schema::add_column(const std::string &name, const ValueType &value, DataType type) { this->columns.push_back({name, value, type}); }

bool Schema::is_available_column(const std::string &name) const {
  for (const Column &column : this->columns) {
    if (column.name == name) {
      return true;
    }
  }
  return false;
}

bool Schema::is_valid_type(const ValueType &value, DataType type) const {
  switch (type) {
  case DataType::STRING:
    return std::holds_alternative<std::string>(value);
  case DataType::INTEGER:
    return std::holds_alternative<int>(value);
  case DataType::BOOL:
    return std::holds_alternative<bool>(value);
  }
  return false;
}