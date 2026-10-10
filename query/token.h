#pragma once

#include <string>

#include "../common/common.h"

class Token {
public:
  TokenType type;
  std::string value;
};