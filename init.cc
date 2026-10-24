#include "init.h"

/**
 * Initializes the metadata layout for paging.
 * 
 * @param rootPageId The identifier for the root page.
 * @param nextPageId The identifier for the subsequent page.
 */
void initDatabase() {
  u64 root_page_id = 1;
  u64 next_page_id = 2;

  Page page(PAGE_SIZE, 0);

  size_t offset = 0;

  page[offset++] = (u8)(root_page_id & 0xFF);
  page[offset++] = (u8)(root_page_id >> 8 & 0xFF);
  page[offset++] = (u8)(root_page_id >> 16 & 0xFF);
  page[offset++] = (u8)(root_page_id >> 24 & 0xFF);
  page[offset++] = (u8)(root_page_id >> 32 & 0xFF);
  page[offset++] = (u8)(root_page_id >> 40 & 0xFF);
  page[offset++] = (u8)(root_page_id >> 48 & 0xFF);
  page[offset++] = (u8)(root_page_id >> 56 & 0xFF);

  page[offset++] = (u8)(next_page_id & 0xFF);
  page[offset++] = (u8)(next_page_id >> 8 & 0xFF);
  page[offset++] = (u8)(next_page_id >> 16 & 0xFF);
  page[offset++] = (u8)(next_page_id >> 24 & 0xFF);
  page[offset++] = (u8)(next_page_id >> 32 & 0xFF);
  page[offset++] = (u8)(next_page_id >> 40 & 0xFF);
  page[offset++] = (u8)(next_page_id >> 48 & 0xFF);
  page[offset++] = (u8)(next_page_id >> 56 & 0xFF);

  FileManager::getInstance().writePage(0, page);
}