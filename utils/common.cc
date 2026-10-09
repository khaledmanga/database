#include "common.h"

TokenType classify_indentifer(const std::string& value) {
    if(kp.find(value) == kp.end()) {
        return TokenType::IDENTIFIER;
    }
    
    return kp[value];
}