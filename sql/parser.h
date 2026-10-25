#pragma once

#include "common.h"
#include <string>

bool isValidSyntax(const std::vector<Token> &tokens);
*Statement convertToATS(const std::vector<Token> &tokens);