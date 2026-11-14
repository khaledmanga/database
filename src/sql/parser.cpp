#include "parser.hpp"

Statement parseIntoInsertStatement(const std::vector<Token> &tokens) {
  InsertStatement statement;

  size_t offset = 2;
  statement.table_name = tokens[offset].value;
  offset += 2;

  while (offset < tokens.size() && tokens[offset].token != Token::RIGHT_PAREN) {
    statement.columns.push_back(tokens[offset].value);
    ++offset;

    if (offset < tokens.size() && tokens[offset].token == Token::COMMA) {
      ++offset;
    }
  }

  offset += 2;

  while (offset < tokens.size() && tokens[offset].token != Token::RIGHT_PAREN) {
    statement.value.push_back(tokens[offset].value);
    ++offset;

    if (offset < tokens.size() && tokens[offset].token == Token::COMMA) {
      ++offset;
    }
  }

  return statement;
}

Statement parseIntoSelectStatement(const std::vector<Token> &tokens) {
  SelectStatement statement;

  size_t offset = 1;

  if (tokens[offset].token == Token::ASTERISK) {
    statement.columns.push_back("*");
    ++offset;
  } else {
    while (offset < tokens.size() && tokens[offset].token != Token::FROM) {
      statement.columns.push_back(tokens[offset].value);
      ++offset;

      if (tokens[offset].token == Token::COMMA) {
        ++offset;
      }
    }
  }

  offset += 1;
  statement.table_name = tokens[offset].value;
  ++offset;

  if (offset < tokens.size() && tokens[offset].token == Token::WHERE) {
    ++offset;

    statement.condition.key = tokens[offset].value;
    offset += 2;

    statement.condition.value = tokens[offset].value;
  }

  return statement;
}

Statement parseIntoUpdateStatement(const std::vector<Token> &tokens) {
  UpdateStatement statement;

  size_t offset = 1;

  statement.table_name = tokens[offset].value;
  offset += 2;

  while (offset < tokens.size() && tokens[offset].token != Token::WHERE) {
    statement.columns.push_back(tokens[offset].value);
    offset += 2;

    statement.value.push_back(tokens[offset].value);
    ++offset;

    if (tokens[offset].token == Token::COMMA) {
      ++offset;
    }
  }

  if (offset < tokens.size() && tokens[offset].token == Token::WHERE) {
    ++offset;

    statement.condition.key = tokens[offset].value;
    offset += 2;

    statement.condition.value = tokens[offset].value;
  }

  return statement;
}

Statement parseIntoDeleteStatement(const std::vector<Token> &tokens) {
  DeleteStatement statement;

  size_t offset = 2;

  statement.table_name = tokens[offset].value;
  ++offset;

  if (offset < tokens.size() && tokens[offset].token == Token::WHERE) {
    ++offset;

    statement.condition.key = tokens[offset].value;
    offset += 2;

    statement.condition.value = tokens[offset].value;
  }

  return statement;
}

Statement parseIntoStatement(const std::vector<Token> &tokens) {
  switch (tokens[0].token) {
  case Token::INSERT:
    return parseIntoInsertStatement(tokens);

  case Token::SELECT:
    return parseIntoSelectStatement(tokens);

  case Token::UPDATE:
    return parseIntoUpdateStatement(tokens);

  case Token::DELETE:
    return parseIntoDeleteStatement(tokens);
  }
}