#include "parser.h"

Parser::Parser(std::vector<Token> &tokens) { this->tokens = tokens; }

/**
 * INSERT   (0)
 * INTO     (1)
 * table    (2)
 * (        (3)
 * column 1 (4)
 * ,        (5)
 * column 2 (6)
 * )        (7)
 * VALUES   (8)
 * (        (9)
 * value 1  (10)
 * ,        (11)
 * value 2  (12)
 * )        (13)
 */
Statement Parser::parse() {
  if (tokens[0].type == TokenType::INSERT) {
    InsertStatement insertStatement;

    int offset = 2;

    insertStatement.table = this->tokens[offset++].value;

    while (this->tokens[offset].value != ")") {
      insertStatement.columns.push_back(this->tokens[offset].value);
      offset += 2;
    }

    offset += 2;

    while (this->tokens[offset].value != ")") {
      insertStatement.values.push_back(this->tokens[offset].value);
      offset += 2;
    }

    return insertStatement;
  }
}