#include "catalog.hpp"

#include "catalog.hpp"
#include <cstring>

Page convertCatalogToPage(const Catalog &catalog) {
  Page page(PAGE_SIZE, 0);

  size_t offset = 0;

  u64 table_count = catalog.tables.size();

  std::memcpy(&page[offset], &table_count, sizeof(table_count));
  offset += sizeof(table_count);

  for (const auto &table : catalog.tables) {
    std::memcpy(&page[offset], &table.table_id, sizeof(table.table_id));
    offset += sizeof(table.table_id);

    u64 name_size = table.name.size();

    std::memcpy(&page[offset], &name_size, sizeof(name_size));
    offset += sizeof(name_size);

    std::memcpy(&page[offset], table.name.data(), name_size);
    offset += name_size;

    std::memcpy(&page[offset], &table.root_page_id, sizeof(table.root_page_id));
    offset += sizeof(table.root_page_id);

    u64 column_count = table.columns.size();

    std::memcpy(&page[offset], &column_count, sizeof(column_count));
    offset += sizeof(column_count);

    for (const auto &column : table.columns) {
      u64 column_name_size = column.name.size();

      std::memcpy(&page[offset], &column_name_size, sizeof(column_name_size));
      offset += sizeof(column_name_size);

      std::memcpy(&page[offset], column.name.data(), column_name_size);
      offset += column_name_size;

      std::memcpy(&page[offset], &column.type, sizeof(column.type));
      offset += sizeof(column.type);
    }
  }

  return page;
}

Catalog convertPageToCatalog(const Page &page) {
  Catalog catalog;

  size_t offset = 0;

  u64 table_count;

  std::memcpy(&table_count, &page[offset], sizeof(table_count));
  offset += sizeof(table_count);

  for (u64 i = 0; i < table_count; ++i) {
    TableSchema table;

    std::memcpy(&table.table_id, &page[offset], sizeof(table.table_id));
    offset += sizeof(table.table_id);

    u64 name_size;

    std::memcpy(&name_size, &page[offset], sizeof(name_size));
    offset += sizeof(name_size);

    table.name.assign(page.begin() + offset, page.begin() + offset + name_size);
    offset += name_size;

    std::memcpy(&table.root_page_id, &page[offset], sizeof(table.root_page_id));
    offset += sizeof(table.root_page_id);

    u64 column_count;

    std::memcpy(&column_count, &page[offset], sizeof(column_count));
    offset += sizeof(column_count);

    for (u64 j = 0; j < column_count; ++j) {
      ColumnSchema column;

      u64 column_name_size;

      std::memcpy(&column_name_size, &page[offset], sizeof(column_name_size));
      offset += sizeof(column_name_size);

      column.name.assign(page.begin() + offset, page.begin() + offset + column_name_size);
      offset += column_name_size;

      std::memcpy(&column.type, &page[offset], sizeof(column.type));
      offset += sizeof(column.type);

      table.columns.push_back(column);
    }

    catalog.tables.push_back(table);
  }

  return catalog;
}