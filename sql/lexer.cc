#include "lexer.h"

/**
 * @section SQL Basics Cheat Sheet
 * @brief A compilation of fundamental SQL commands for Data Manipulation Language (DML) and Data Definition Language
 * (DDL).
 *
 * 1. Insert Data
 *    INSERT INTO table_name (column1, column2, column3) VALUES (value1, value2, value3);
 *
 * 2. Update Data
 *    UPDATE table_name SET column1 = value1, column2 = value2 WHERE condition;
 *
 * 3. Query Specific Columns
 *    SELECT column1, column2 FROM table_name;
 *
 * 4. Query All Data
 *    SELECT * FROM table_name;
 *
 * 5. Delete Data
 *    DELETE FROM table_name WHERE condition;
 *
 * 6. Create a New Table
 *    CREATE TABLE table_name (
 *        column1 datatype constraint,
 *        column2 datatype constraint,
 *        column3 datatype constraint
 *    );
 */

std::vector<Token> tokenizer(const std::string &raw_sql) {
  size_t start = 0;
  size_t end = 0;
  std::vector<Token> res;

  while (end < raw_sql.size()) {
    if (std::isspace((unsigned char)raw_sql[end])) {
      ++end;
      start = end;
      continue;
    }

    if (std::ispunct((unsigned char)raw_sql[end])) {
      switch (raw_sql[end]) {
      case '(':
        res.push_back({TokenType::LEFT_PAREN, "("});
        break;
      case ')':
        res.push_back({TokenType::RIGHT_PAREN, ")"});
        break;
      case ',':
        res.push_back({TokenType::COMMA, ","});
        break;
      case ';':
        res.push_back({TokenType::SEMICOLON, ";"});
        break;
      case '*':
        res.push_back({TokenType::STAR, "*"});
        break;
      case '.':
        res.push_back({TokenType::DOT, "."});
        break;
      case '\'':
        ++end;
        start = end;

        while (end < raw_sql.size() && raw_sql[end] != '\'') {
          ++end;
        }

        res.push_back({TokenType::STRING, raw_sql.substr(start, end - start)});

        if (end < raw_sql.size()) {
          ++end;
        }

        start = end;
        continue;
      default:
        ++end;
        start = end;
        continue;
      }

      ++end;
      start = end;
      continue;
    }

    if (std::isalnum((unsigned char)raw_sql[end]) || raw_sql[end] == '_') {
      start = end;

      while (end < raw_sql.size() && (std::isalnum((unsigned char)raw_sql[end]) || raw_sql[end] == '_')) {
        ++end;
      }

      std::string token = raw_sql.substr(start, end - start);

      if (token == "INSERT") {
        res.push_back({TokenType::INSERT, token});
      } else if (token == "UPDATE") {
        res.push_back({TokenType::UPDATE, token});
      } else if (token == "SELECT") {
        res.push_back({TokenType::SELECT, token});
      } else if (token == "DELETE") {
        res.push_back({TokenType::DELETE, token});
      } else if (token == "CREATE") {
        res.push_back({TokenType::CREATE, token});
      } else if (token == "INTO") {
        res.push_back({TokenType::INTO, token});
      } else if (token == "VALUES") {
        res.push_back({TokenType::VALUES, token});
      } else if (token == "WHERE") {
        res.push_back({TokenType::WHERE, token});
      } else if (token == "FROM") {
        res.push_back({TokenType::FROM, token});
      } else {
        res.push_back({TokenType::IDENTIFIER, token});
      }

      start = end;
      continue;
    }

    ++end;
    start = end;
  }

  return res;
}