#pragma once

#include <string>
#include <vector>

#include <string>
#include <vector>

class Statement  {
public:
    std::string table;
    std::vector<std::string> columns;
    std::vector<std::string> values;
};


class InsertStatement: public Statement {

};
