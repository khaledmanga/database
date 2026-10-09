#pragma once

#include <unordered_map>
#include <cstring>

#include "../common/common.h"

TokenType classify_indentifer(const std::string& value);

std::unordered_map<std::string, TokenType> kp = {
    {"INSERT", TokenType::INSERT},
    {"INTO", TokenType::INTO},
    {"VALUES", TokenType::VALUES},
    {"SELECT", TokenType::SELECT},
    {"FROM", TokenType::FROM},
    {"WHERE", TokenType::WHERE},
}