#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "../common/common.h"
#include "schema.h"

class Catalog {
private:
  std::unordered_map<std::string, Schema> tables;

public:
  void add_table(const std::string &table_name, const Schema &schema);
  std::optional<Schema> search_schema(const std::string &table_name) const;
  bool is_table_available(const std::string &table_name) const;
  bool is_column_available(const Schema &schema, const std::vector<std::string> &columns) const;
  bool is_valid_type(const Schema &schema, const Column &column);
  Page encode();
  Catalog decode();
};