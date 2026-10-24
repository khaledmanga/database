#include "metadata.h"

Metadata::Metadata() {
  Page page = FileManager::getInstance().readPage(0);
  this->decode(page);
}

Metadata& Metadata::getInstance() {
  static Metadata instance;
  return instance;
}

void Metadata::writeMetadata() {
  this->next_page_id += 1;
  FileManager::getInstance().writePage(0, this->encode());
}

void Metadata::decode(const Page &page) {
  size_t offset = 0;
  u64 root_page_id;
  u64 next_page_id;
  root_page_id |= (u64)page[offset] << 56;
  root_page_id |= (u64)page[offset + 1] << 48;
  root_page_id |= (u64)page[offset + 2] << 40;
  root_page_id |= (u64)page[offset + 3] << 32;
  root_page_id |= (u64)page[offset + 4] << 24;
  root_page_id |= (u64)page[offset + 5] << 16;
  root_page_id |= (u64)page[offset + 6] << 8;
  root_page_id |= (u64)page[offset + 7];

  offset += 8;

  next_page_id |= (u64)page[offset] << 56;
  next_page_id |= (u64)page[offset + 1] << 48;
  next_page_id |= (u64)page[offset + 2] << 40;
  next_page_id |= (u64)page[offset + 3] << 32;
  next_page_id |= (u64)page[offset + 4] << 24;
  next_page_id |= (u64)page[offset + 5] << 16;
  next_page_id |= (u64)page[offset + 6] << 8;
  next_page_id |= (u64)page[offset + 7];

  this->root_page_id = root_page_id;
  this->next_page_id = next_page_id;
}

Page Metadata::encode() {
  Page page{};

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

  return page;
}

u64 Metadata::getRootPageId() const { return this->root_page_id; }

u64 Metadata::getNextPageId() const { return this->next_page_id; }

void Metadata::updateNextPageId() {
  this->next_page_id += 1;
  FileManager::getInstance().writePage(0, this->encode());
}
