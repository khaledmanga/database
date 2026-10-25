#pragma once

#include "common.h"
#include "ats.h"
#include <string>

bool isValidSyntax(const std::vector<Token> &tokens);
Statement *convertToATS(const std::vector<Token> &tokens);