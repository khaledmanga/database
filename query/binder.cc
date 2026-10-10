#include "binder.h"

Binder::Binder(const Catalog &catalog) : catalog(catalog) {}

bool Binder::verify(Statement statement) {
  const std::optional<Schema> schema = this->catalog.search_schema(statement.table);
  if (!schema) {
    return false;
  }

  if (!this->catalog.is_column_available(*schema, statement.columns)) {
    return false;
  }

  return true;
}