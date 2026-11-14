#pragma once

#include "include/common.hpp"
#include <cstdint>
#include <string>

enum class Token : u16 {

  // =====================================
  // Special
  // =====================================

  END_OF_FILE,
  UNKNOWN,

  // =====================================
  // Identifiers
  // =====================================

  IDENTIFIER,

  // =====================================
  // Literals
  // =====================================

  INTEGER_LITERAL,
  FLOAT_LITERAL,
  STRING_LITERAL,
  BOOLEAN_LITERAL,
  NULL_LITERAL,

  // =====================================
  // Data Types
  // =====================================

  TYPE_INT,
  TYPE_BIGINT,
  TYPE_FLOAT,
  TYPE_DOUBLE,
  TYPE_VARCHAR,
  TYPE_TEXT,
  TYPE_BOOLEAN,
  TYPE_BLOB,

  // =====================================
  // DDL - Data Definition Language
  // =====================================

  CREATE,
  DROP,
  ALTER,
  TABLE,
  DATABASE,
  INDEX,

  // =====================================
  // DML - Data Manipulation Language
  // =====================================

  SELECT,
  INSERT,
  INTO,
  VALUES,
  UPDATE,
  SET,
  DELETE,
  FROM,

  // =====================================
  // Clauses
  // =====================================

  WHERE,
  GROUP,
  BY,
  HAVING,
  ORDER,
  ASC,
  DESC,
  LIMIT,
  OFFSET,

  // =====================================
  // Constraints
  // =====================================

  PRIMARY,
  KEY,
  FOREIGN,
  REFERENCES,
  UNIQUE,
  NOT,
  NULL,
  DEFAULT,
  CHECK,
  AUTO_INCREMENT,

  // =====================================
  // Logical Operators
  // =====================================

  AND,
  OR,
  IS,
  IN,
  BETWEEN,
  LIKE,

  // =====================================
  // Comparison Operators
  // =====================================

  EQUAL,         // =
  NOT_EQUAL,     // != or <>
  LESS_THAN,     // <
  GREATER_THAN,  // >
  LESS_EQUAL,    // <=
  GREATER_EQUAL, // >=

  // =====================================
  // Arithmetic Operators
  // =====================================

  PLUS,     // +
  MINUS,    // -
  MULTIPLY, // *
  DIVIDE,   // /
  MODULO,   // %

  // =====================================
  // Punctuation
  // =====================================

  LEFT_PAREN,  // (
  RIGHT_PAREN, // )
  COMMA,       // ,
  DOT,         // .
  SEMICOLON,   // ;
  COLON,       // :

  // =====================================
  // Other
  // =====================================

  STAR, // * (SELECT *)
};

struct Token {
  std::string value;
  Token token;
};