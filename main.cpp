
#include <iostream>
#include <string>

#include "bplustree/b_plus_tree.h"
#include "storage/file_manager.h"
#include "storage/page.h"
#include "storage/metadata.h"
#include "query/lexer.h"
#include "query/parser.h"
#include "query/binder.h"
#include "query/catalog.h"
#include "operators/tree.h"
#include "query/token.h"

int main() {
    std::string command_line;

    std::getline(std::cin, command_line);

    Catalog catalog;
    Lexer lexer(command_line);
    std::vector<Token> tokens = lexer.tokenize_insert();
    Parser parser(tokens);
    Binder binder(catalog);

    std::cout << binder.verify(parser.parse());

    return 0;
}
