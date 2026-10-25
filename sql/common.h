#pragma once

#include <utility>
#include <string>
#include <vector>

enum class TokenType {
  // Keywords
  SELECT,
  INSERT,
  INTO,
  VALUES,
  FROM,
  WHERE,
  UPDATE,
  SET,
  DELETE,
  CREATE,
  TABLE,
  AND,
  OR,

  // Identifiers and literals
  IDENTIFIER,
  STRING,
  INTEGER,
  FLOAT,

  // Operators
  EQUAL,
  NOT_EQUAL,
  LESS,
  GREATER,
  LESS_EQUAL,
  GREATER_EQUAL,

  // Punctuation
  LEFT_PAREN,
  RIGHT_PAREN,
  COMMA,
  SEMICOLON,
  STAR,
  DOT,

  // Special
  END_OF_FILE,
  INVALID
};

using Token = std::pair<TokenType, std::string>;