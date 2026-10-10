#include "catalog.h"

void Catalog::add_table(const std::string &table_name, const Schema &schema) { this->tables.insert({table_name, schema}); }

std::optional<Schema> Catalog::search_schema(const std::string &table_name) const {
  const auto table = this->tables.find(table_name);
  if (table == this->tables.end()) {
    return std::nullopt;
  }
  return table->second;
}

bool Catalog::is_table_available(const std::string &table_name) const { return this->tables.find(table_name) != this->tables.end(); }

bool Catalog::is_column_available(const Schema &schema, const std::vector<std::string> &columns) const {
  const std::vector<Column> schema_columns = schema.get_column();
  for (const std::string &column : columns) {
    bool found = false;
    for (const Column &schema_column : schema_columns) {
      if (schema_column.name == column) {
        found = true;
        break;
      }
    }
    if (!found) {
      return false;
    }
  }
  return true;
}

bool Catalog::is_valid_type(const Schema &schema, const Column &column) {
  for (const Column &schema_column : schema.get_column()) {
    if (schema_column.name == column.name) {
      return schema_column.type == column.type && schema.is_valid_type(column.value, column.type);
    }
  }
  return false;
}