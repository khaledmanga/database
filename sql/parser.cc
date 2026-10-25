#include "parser.h"

bool isValidSyntax(const std::vector<Token> &tokens) {
  size_t pos = 0;

  if (tokens[pos].first == TokenType::INSERT) {
    ++pos;

    if (pos >= tokens.size() || tokens[pos].first != TokenType::INTO) {
      return false;
    }

    if (pos >= tokens.size() || tokens[pos].first != TokenType::IDENTIFIER) {
      return false;
    }

    if (tokens[pos].first == TokenType::LEFT_PAREN) {
      ++pos;
      while (pos < tokens.size() && tokens[pos].first != TokenType::RIGHT_PAREN) {
        if (tokens[pos].first != TokenType::IDENTIFIER) {
          return false;
        }
        ++pos;

        if (pos < tokens.size() && tokens[pos].first == TokenType::COMMA) {
          ++pos;
        }
      }

      if (pos >= tokens.size() || tokens[pos].first != TokenType::RIGHT_PAREN) {
        return false;
      }

      ++pos;
    }

    if (pos >= tokens.size() && tokens[pos].first != TokenType::VALUES) {
      return false;
    }

    if (tokens[pos].first == TokenType::LEFT_PAREN) {
      ++pos;
      while (pos < tokens.size() && tokens[pos].first != TokenType::RIGHT_PAREN) {
        if (tokens[pos].first != TokenType::STRING || tokens[pos + 1].first != TokenType::COMMA) {
          return false;
        }

        ++pos;

        if (pos < tokens.size() && tokens[pos].first == TokenType::COMMA) {
          ++pos;
        }
      }

      if (pos >= tokens.size() || tokens[pos].first != TokenType::RIGHT_PAREN) {
        return false;
      }

      ++pos;
    }

    if (pos >= tokens.size() && tokens[pos].first != TokenType::SEMICOLON) {
      return false;
    }
  }
}

*Statement convertToATS(const std::vector<Token> &tokens) {
  if (tokens[pos].first == TokenType::INSERT) {
    Statement insertStatement;

    size_t pos = 2;

    insertStatement.table = tokens[pos].second;
    pos += 2;

    while (pos < tokens.size() && tokens[pos].first != TokenType::RIGHT_PAREN) {
      insertStatement.columns.push_back(tokens[pos].second);

      ++pos;

      if (pos < tokens.size() && tokens[pos].first == TokenType::COMMA) {
        ++pos;
      }
    }

    pos += 2;

    while (pos < tokens.size() && tokens[pos].first != TokenType::RIGHT_PAREN) {
      insertStatement.values.push_back(tokens[pos].second);

      ++pos;

      if (pos < tokens.size() && tokens[pos].first == TokenType::COMMA) {
        ++pos;
      }
    }
  }
}