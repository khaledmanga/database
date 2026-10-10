#include "common.h"

std::unordered_map<std::string, TokenType> kp = {
    {"INSERT", TokenType::INSERT},
    {"INTO", TokenType::INTO},
    {"VALUES", TokenType::VALUES},
    {"SELECT", TokenType::SELECT},
    {"FROM", TokenType::FROM},
    {"WHERE", TokenType::WHERE},
};

TokenType classify_indentifer(const std::string& value) {
    if(kp.find(value) == kp.end()) {
        return TokenType::IDENTIFIER;
    }
    
    return kp[value];
}