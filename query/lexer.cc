
#include "lexer.h"

Lexer::Lexer(const std::string &input) {
  this->input = input;
  this->pointer = 0;
}

std::vector<Token> Lexer::tokenize() {
  std::vector<Token> tokens;
  size_t size_input = this->input.size();

  while (this->pointer < size_input) {
    if (std::isspace((unsigned char)this->input[this->pointer])) {
      ++this->pointer;
      continue;
    }

    size_t start = this->pointer;
    char c = this->input[this->pointer];

    if (c == '(') {
      tokens.push_back({TokenType::LPAREN, "("});
      ++this->pointer;
    } else if (c == ')') {
      tokens.push_back({TokenType::RPAREN, ")"});
      ++this->pointer;
    } else if (c == ',') {
      tokens.push_back({TokenType::COMMA, ","});
      ++this->pointer;
    } else if (c == ';') {
      tokens.push_back({TokenType::SEMICOLON, ";"});
      ++this->pointer;
    } else if (c == '*') {
      tokens.push_back({TokenType::STAR, "*"});
      ++this->pointer;
    } else if (c == '=') {
      tokens.push_back({TokenType::EQUAL, "="});
      ++this->pointer;
    } else if (std::isalpha((unsigned char)c)) {
      while (this->pointer < size_input && std::isalpha((unsigned char)this->input[this->pointer])) {
        ++this->pointer;
      }

      std::string value = this->input.substr(start, this->pointer - start);

      tokens.push_back({classify_indentifer(value), value});
    } else if (c == '\'' || c == '"') {
      char quote = c;
      ++this->pointer;

      while (this->pointer < size_input && this->input[this->pointer] != quote) {
        ++this->pointer;
      }

      std::string value = this->input.substr(start + 1, this->pointer - start - 1);

      tokens.push_back({TokenType::STRING, value});

      if (this->pointer < size_input) {
        ++this->pointer;
      }
    } else {
      ++this->pointer;
    }
  }

  return tokens;
}