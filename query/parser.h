#pragma once

#include "ast.h"
#include "token.h"

class Parser {
private:
  std::vector<Token> tokens;

public:
  Parser(std::vector<Token> &tokens);
  Statement parse();
};