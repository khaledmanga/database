#ifndef SQL_AST_HPP
#define

#include <string>
#include <variant>
#include <vector>

struct Condition {
  std::string key;
  std::string value;
}

struct InsertStatment {
  std::string table_name;
  std::vector<std::string> columns;
  std::vector<std::string> value;
};

struct UpdateStatment {
  std::string table_name;
  std::vector<std::string> columns;
  std::vector<std::string> value;
  Condition condition;
};

struct DeleteStatment {
  std::string table_name;
  Condition condition;
};

struct SelectStatment {
  std::string table_name;
  std::vector<std::string> columns;
  Condition condition;
};

using Statement = std::variant<InsertStatment, UpdateStatment, DeleteStatment, SelectStatment>;

#endif