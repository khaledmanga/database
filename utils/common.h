#pragma once

#include <unordered_map>
#include <string>

#include "../common/common.h"

TokenType classify_indentifer(const std::string& value);

extern std::unordered_map<std::string, TokenType> kp;