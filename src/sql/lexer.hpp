#ifndef SQL_PARSER_HPP
#define

#include "common.hpp"
#include <cctype>

std::vector<Token> parseInsertSQL(const std::string &raw_sql);
std::vector<Token> parseUpdateSQL(const std::string& raw_sql);
std::vector<Token> parseDeleteSQL(const std::string& raw_sql);
std::vector<Token> parseSelectSQL(const std::string& raw_sql);
std::vector<Token> parser(const std::string &raw_sql);

#endif