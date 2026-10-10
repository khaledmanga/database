#pragma once

#include "../utils/common.h"
#include "token.h"

#include <cctype>
#include <string>
#include <vector>

class Lexer {
public:
  Lexer(const std::string &input);

  std::vector<Token> tokenize_insert();

private:
  std::string input;
  size_t pointer;
};