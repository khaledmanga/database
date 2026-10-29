#include "semantic_analysis.h"

bool validateStatement(const Catalog &catalog, const TableSchema &table_schema) {
    for (auto const &table : catalog.tables) {
        if (table.name == table_schema.name) {
            if (table.columns.size() != table_schema.columns.size()) {
                return false;
            }

            for (size_t i = 0; i < table.columns.size(); ++i) {
                if (table.columns[i].name != table_schema.columns[i].name) {
                    return false;
                }

                if (table.columns[i].type.index() != table_schema.columns[i].type.index()) {
                    return false;
                }
            }

            return true;
        }
    }

    return false;
}