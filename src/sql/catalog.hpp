#ifndef SQL_CATALOG_HPP
#define

#include <variant>

std::variant MultiType<int, std::string, bool>;

struct ColumnSchema {
  std::string name;
  MultiType type;
};

struct TableSchema {
  std::string table_name;
  std::vector<ColumnSchema> columns;
  u8 root_page_id;
};

Page convertCatalogToPage(const Catalog &catalog);
Catalog convertPageToCatalog(const Page &page);

#endif