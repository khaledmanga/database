#include "metadata.h"
#include "storage/file_manager.h"

Metadata decodeMetadata(const Page &page) {
  size_t offset = 0;

  Metadata metadata;

  u64 root_page_id = 0;
  u64 next_page_id = 0;
  root_page_id |= (u64)page[offset];
  root_page_id |= (u64)page[offset + 1] << 8;
  root_page_id |= (u64)page[offset + 2] << 16;
  root_page_id |= (u64)page[offset + 3] << 24;
  root_page_id |= (u64)page[offset + 4] << 32;
  root_page_id |= (u64)page[offset + 5] << 40;
  root_page_id |= (u64)page[offset + 6] << 48;
  root_page_id |= (u64)page[offset + 7] << 56;

  offset += 8;

  next_page_id |= (u64)page[offset];
  next_page_id |= (u64)page[offset + 1] << 8;
  next_page_id |= (u64)page[offset + 2] << 16;
  next_page_id |= (u64)page[offset + 3] << 24;
  next_page_id |= (u64)page[offset + 4] << 32;
  next_page_id |= (u64)page[offset + 5] << 40;
  next_page_id |= (u64)page[offset + 6] << 48;
  next_page_id |= (u64)page[offset + 7] << 56;

  metadata.root_page_id = root_page_id;
  metadata.next_page_id = next_page_id;

  return metadata;
}

Page encodeMetadata(const Metadata &metadata) {
  Page page(PAGE_SIZE, 0);

  size_t offset = 0;

  page[offset++] = (u8)(metadata.root_page_id & 0xFF);
  page[offset++] = (u8)(metadata.root_page_id >> 8 & 0xFF);
  page[offset++] = (u8)(metadata.root_page_id >> 16 & 0xFF);
  page[offset++] = (u8)(metadata.root_page_id >> 24 & 0xFF);
  page[offset++] = (u8)(metadata.root_page_id >> 32 & 0xFF);
  page[offset++] = (u8)(metadata.root_page_id >> 40 & 0xFF);
  page[offset++] = (u8)(metadata.root_page_id >> 48 & 0xFF);
  page[offset++] = (u8)(metadata.root_page_id >> 56 & 0xFF);

  page[offset++] = (u8)(metadata.next_page_id & 0xFF);
  page[offset++] = (u8)(metadata.next_page_id >> 8 & 0xFF);
  page[offset++] = (u8)(metadata.next_page_id >> 16 & 0xFF);
  page[offset++] = (u8)(metadata.next_page_id >> 24 & 0xFF);
  page[offset++] = (u8)(metadata.next_page_id >> 32 & 0xFF);
  page[offset++] = (u8)(metadata.next_page_id >> 40 & 0xFF);
  page[offset++] = (u8)(metadata.next_page_id >> 48 & 0xFF);
  page[offset++] = (u8)(metadata.next_page_id >> 56 & 0xFF);

  return page;
}

void Metadata::updateNextPageId() {
  this->next_page_id += 1;
  FileManager::getInstance().writePage(0, encodeMetadata(*this));
}
