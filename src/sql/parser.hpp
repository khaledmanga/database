#ifndef SQL_PARSER_HPP
#define

Statement parseIntoInsertStatement(const std::vector<Token>& tokens);
Statement parseIntoUpdateStatement(const std::vector<Token>& tokens);
Statement parseIntoDeleteStatement(const std::vector<Token>& tokens);
Statement parseIntoSelectStatement(const std::vector<Token>& tokens);
Statement parseIntoStatement(const std::vector<Token>& tokens);

#endif