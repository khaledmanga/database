#pragma once

#include "../utils/common.h"
#include "token.h"

#include <string>
#include <vector>
#include <cctype>

class Lexer {
public:
  Lexer(const std::string &input);

  std::vector<Token> tokenize();

private:
  const std::string &input;
  size_t pointer;
};