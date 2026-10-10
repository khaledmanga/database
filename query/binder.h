#pragma once

#include <optional>

#include "ast.h"
#include "catalog.h"

class Binder {
private:
  Catalog catalog;

public:
  Binder(const Catalog &catalog);
  bool verify(Statement statement);
};