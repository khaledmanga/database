#include "lexer.hpp"

/**
 * INSERT INTO table_name (column1, column2, ...) VALUES (value1, value2, ...);
 *
 * SELECT * FROM table_name;
 *
 * SELECT column1, column2, ... FROM table_name;
 *
 * UPDATE table_name SET column1 = value1, column2 = value2 WHERE condition;
 *
 * DELETE FROM table_name WHERE condition;
 */

std::vector<Token> parseInsertSQL(const std::string &raw_sql) {
  std::vector<Token> result;

  size_t offset = 0;

  while (offset < raw_sql.size()) {
    if (std::isspace(static_cast<unsigned char>(raw_sql[offset]))) {
      ++offset;
      continue;
    }

    if (!result.empty() && result.back().token == Token::INTO) {

      result.push_back({Token::IDENTIFIER, std::string(1, raw_sql[offset++])});

    } else if (result.size() >= 2 && result[result.size() - 2].token == Token::IDENTIFIER) {

      result.push_back({Token::IDENTIFIER, std::string(1, raw_sql[offset++])});

    } else if (!result.empty() && result.back().token == Token::VALUES) {

      if (std::isdigit(static_cast<unsigned char>(raw_sql[offset]))) {
        result.push_back({Token::INTEGER_LITERAL, std::string(1, raw_sql[offset++])});
      } else if (raw_sql.compare(offset, 4, "true") == 0 ||
                 raw_sql.compare(offset, 5, "false") == 0) {

        size_t length = raw_sql.compare(offset, 4, "true") == 0 ? 4 : 5;

        result.push_back({Token::BOOLEAN_LITERAL, raw_sql.substr(offset, length)});

        offset += length;

      } else {
        result.push_back({Token::STRING_LITERAL, std::string(1, raw_sql[offset++])});
      }

    } else if (raw_sql.compare(offset, 6, "INSERT") == 0) {
      result.push_back({Token::INSERT, "INSERT"});
      offset += 6;

    } else if (raw_sql.compare(offset, 4, "INTO") == 0) {
      result.push_back({Token::INTO, "INTO"});
      offset += 4;

    } else if (raw_sql.compare(offset, 6, "VALUES") == 0) {
      result.push_back({Token::VALUES, "VALUES"});
      offset += 6;

    } else if (raw_sql[offset] == '(') {
      result.push_back({Token::LEFT_PAREN, "("});
      offset++;

    } else if (raw_sql[offset] == ')') {
      result.push_back({Token::RIGHT_PAREN, ")"});
      offset++;

    } else if (raw_sql[offset] == ',') {
      result.push_back({Token::COMMA, ","});
      offset++;

    } else if (raw_sql[offset] == ';') {
      result.push_back({Token::SEMICOLON, ";"});
      offset++;

    } else {
      ++offset;
    }
  }

  return result;
}

std::vector<Token> parseUpdateSQL(const std::string &raw_sql) {
  std::vector<Token> result;

  size_t offset = 0;

  while (offset < raw_sql.size()) {
    if (std::isspace(static_cast<unsigned char>(raw_sql[offset]))) {
      ++offset;
      continue;
    }

    if (raw_sql.compare(offset, 6, "UPDATE") == 0) {
      result.push_back({Token::UPDATE, "UPDATE"});
      offset += 6;

    } else if (raw_sql.compare(offset, 3, "SET") == 0) {
      result.push_back({Token::SET, "SET"});
      offset += 3;

    } else if (raw_sql.compare(offset, 5, "WHERE") == 0) {
      result.push_back({Token::WHERE, "WHERE"});
      offset += 5;

    } else if (raw_sql[offset] == '=') {
      result.push_back({Token::EQUAL, "="});
      offset++;

    } else if (raw_sql[offset] == ',') {
      result.push_back({Token::COMMA, ","});
      offset++;

    } else if (raw_sql[offset] == ';') {
      result.push_back({Token::SEMICOLON, ";"});
      offset++;

    } else if (std::isdigit(static_cast<unsigned char>(raw_sql[offset]))) {
      result.push_back({Token::INTEGER_LITERAL, std::string(1, raw_sql[offset++])});

    } else if (raw_sql.compare(offset, 4, "true") == 0 ||
               raw_sql.compare(offset, 5, "false") == 0) {

      size_t length = raw_sql.compare(offset, 4, "true") == 0 ? 4 : 5;

      result.push_back({Token::BOOLEAN_LITERAL, raw_sql.substr(offset, length)});

      offset += length;

    } else if (raw_sql[offset] == '\'') {
      size_t start = ++offset;

      while (offset < raw_sql.size() && raw_sql[offset] != '\'') {
        offset++;
      }

      result.push_back({Token::STRING_LITERAL, raw_sql.substr(start, offset - start)});

      if (offset < raw_sql.size()) {
        offset++;
      }

    } else {
      result.push_back({Token::IDENTIFIER, std::string(1, raw_sql[offset++])});
    }
  }

  return result;
}

std::vector<Token> parseDeleteSQL(const std::string &raw_sql) {
  std::vector<Token> result;

  size_t offset = 0;

  while (offset < raw_sql.size()) {
    if (std::isspace(static_cast<unsigned char>(raw_sql[offset]))) {
      ++offset;
      continue;
    }

    if (raw_sql.compare(offset, 6, "DELETE") == 0) {
      result.push_back({Token::DELETE, "DELETE"});
      offset += 6;

    } else if (raw_sql.compare(offset, 4, "FROM") == 0) {
      result.push_back({Token::FROM, "FROM"});
      offset += 4;

    } else if (raw_sql.compare(offset, 5, "WHERE") == 0) {
      result.push_back({Token::WHERE, "WHERE"});
      offset += 5;

    } else if (raw_sql[offset] == '=') {
      result.push_back({Token::EQUAL, "="});
      offset++;

    } else if (raw_sql[offset] == ';') {
      result.push_back({Token::SEMICOLON, ";"});
      offset++;

    } else if (std::isdigit(static_cast<unsigned char>(raw_sql[offset]))) {
      result.push_back({Token::INTEGER_LITERAL, std::string(1, raw_sql[offset++])});

    } else {
      result.push_back({Token::IDENTIFIER, std::string(1, raw_sql[offset++])});
    }
  }

  return result;
}

std::vector<Token> parseSelectSQL(const std::string &raw_sql) {
  std::vector<Token> result;

  size_t offset = 0;

  while (offset < raw_sql.size()) {
    if (std::isspace(static_cast<unsigned char>(raw_sql[offset]))) {
      ++offset;
      continue;
    }

    if (raw_sql[offset] == '*') {
      result.push_back({Token::ASTERISK, "*"});
      offset++;

    } else if (raw_sql.compare(offset, 6, "SELECT") == 0) {
      result.push_back({Token::SELECT, "SELECT"});
      offset += 6;

    } else if (raw_sql.compare(offset, 4, "FROM") == 0) {
      result.push_back({Token::FROM, "FROM"});
      offset += 4;

    } else if (raw_sql[offset] == ',') {
      result.push_back({Token::COMMA, ","});
      offset++;

    } else if (raw_sql[offset] == ';') {
      result.push_back({Token::SEMICOLON, ";"});
      offset++;

    } else {
      result.push_back({Token::IDENTIFIER, std::string(1, raw_sql[offset++])});
    }
  }

  return result;
}

std::vector<Token> parser(const std::string &raw_sql) {
  if (raw_sql.compare(0, 6, "INSERT") == 0)
    return parseInsertSQL(raw_sql);

  else if (raw_sql.compare(0, 6, "SELECT") == 0)
    return parseSelectSQL(raw_sql);

  else if (raw_sql.compare(0, 6, "UPDATE") == 0)
    return parseUpdateSQL(raw_sql);

  else if (raw_sql.compare(0, 6, "DELETE") == 0)
    return parseDeleteSQL(raw_sql);

  return {};
}